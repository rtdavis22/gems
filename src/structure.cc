// Author: Robert Davis

#include "structure.h"

#include <map>
#include <utility>

#include "gmml/gmml.h"

#include "atom.h"
#include "residue.h"

using std::map;

using gmml::Atom;
using gmml::Structure;

using v8::AccessorInfo;
using v8::Arguments;
using v8::Array;
using v8::External;
using v8::FunctionTemplate;
using v8::Handle;
using v8::HandleScope;
using v8::Local;
//using v8::Object;
using v8::ObjectTemplate;
using v8::Persistent;
using v8::String;
using v8::ThrowException;
using v8::Undefined;
using v8::Value;

namespace gems {

int GemsStructure::attach_from_head(GemsStructure *structure,
                                    int tail_residue) {
    combine(structure);
    return structure_->attach_from_head(structure->structure(), tail_residue,
                                        structure->parent_tail_name());
}

int GemsStructure::attach(GemsStructure *structure) {
    combine(structure);
    return structure_->attach(structure->structure());
}

int GemsStructure::append(GemsStructure *structure) {
    combine(structure);
    return structure_->append(structure->structure());
}

void GemsStructure::combine(GemsStructure *structure) {
    int cur_size = structure_->residue_count();
    const std::map<std::string, int>& id_map = structure->id_map();
    for (std::map<std::string, int>::const_iterator it = id_map.begin();
            it != id_map.end(); ++it) {
        id_map_[it->first] = it->second + cur_size;
    }
}

int GemsStructure::map_id(const std::string& id) const {
    std::map<std::string, int>::const_iterator it;
    if ((it = id_map_.find(id)) != id_map_.end())
        return it->second;
    return -1;
}

struct StructureTemplate::Impl {
    static Handle<Value> Atoms(const Arguments& args);
    static Handle<Value> Minimize(const Arguments& args);
    static Handle<Value> PrintCoordinateFile(const Arguments& args);
    static Handle<Value> PrintTopologyFile(const Arguments& args);
    static Handle<Value> Residues(const Arguments& args);
    static Handle<Value> SetDihedral(const Arguments& args);
    static Handle<Value> Solvate(const Arguments& args);
    static Handle<Value> GetSize(Local<String> property,
                                 const AccessorInfo& info);
};

namespace {

bool kIsWrapperInitialized = false;

// This is a map to manage the objects created by the wrapper, so we don't
// create a new object for existing structures.
map<GemsStructure*, Persistent<v8::Object> > *kStructureObjectManager = NULL;

// This is lazily invoked by StructureWrapper::wrap.
void initialize_wrapper() {
    kStructureObjectManager = new map<GemsStructure*, Persistent<v8::Object> >;
    kIsWrapperInitialized = true;
}

}  // namespace


void StructureWrapper::destroy() {
    if (kIsWrapperInitialized) {
        delete kStructureObjectManager;
        kIsWrapperInitialized = false;
    }
}

Handle<v8::Object> StructureWrapper::wrap(GemsStructure *structure) {
    HandleScope handle_scope;

    static StructureTemplate structure_template;

    Persistent<FunctionTemplate> template_ = structure_template.get_template();

    if (!kIsWrapperInitialized)
        initialize_wrapper();

    map<GemsStructure*, Persistent<v8::Object> >::iterator it =
        kStructureObjectManager->lower_bound(structure);

    if (it != kStructureObjectManager->end() &&
            !kStructureObjectManager->key_comp()(structure, it->first))
        return it->second;

    Persistent<v8::Object> instance = Persistent<v8::Object>::New(
        template_->InstanceTemplate()->NewInstance());

    instance.MakeWeak(structure, callback);

    kStructureObjectManager->insert(it, std::make_pair(structure, instance));

    instance->SetInternalField(0, External::New(structure));

    return handle_scope.Close(instance);
}


void StructureWrapper::callback(Persistent<Value> object, void *data) {
    GemsStructure *structure = static_cast<GemsStructure*>(data);
    kStructureObjectManager->erase(structure);
    delete structure;
    object.Dispose();
    object.Clear();
}

#define NODE_SET_PROTOTYPE_METHOD(templ, name, callback)                  \
do {                                                                      \
  v8::Local<v8::Signature> __callback##_SIG = v8::Signature::New(templ);  \
  v8::Local<v8::FunctionTemplate> __callback##_TEM =                      \
    v8::FunctionTemplate::New(callback, v8::Handle<v8::Value>(),          \
                          __callback##_SIG);                              \
  templ->PrototypeTemplate()->Set(v8::String::NewSymbol(name),            \
                                  __callback##_TEM);                      \
} while (0)

void StructureTemplate::Init() {
    HandleScope handle_scope;

    Handle<FunctionTemplate> local_template = FunctionTemplate::New();
    Handle<ObjectTemplate> instance_template =
        local_template->InstanceTemplate();
    instance_template->SetInternalFieldCount(1);

    instance_template->SetAccessor(String::New("size"), Impl::GetSize);

    template_ = Persistent<FunctionTemplate>::New(local_template);

    NODE_SET_PROTOTYPE_METHOD(template_, "minimize", Impl::Minimize);

    NODE_SET_PROTOTYPE_METHOD(template_, "print_coordinate_file",
                              Impl::PrintCoordinateFile);

    NODE_SET_PROTOTYPE_METHOD(template_, "print_topology_file",
                              Impl::PrintTopologyFile);

    NODE_SET_PROTOTYPE_METHOD(template_, "set_dihedral", Impl::SetDihedral);

    NODE_SET_PROTOTYPE_METHOD(template_, "solvate", Impl::Solvate);

    NODE_SET_PROTOTYPE_METHOD(template_, "atoms", Impl::Atoms);
    NODE_SET_PROTOTYPE_METHOD(template_, "residues", Impl::Residues);
}

Handle<Value> StructureTemplate::Impl::Minimize(const Arguments& args) {
    if (args.Length() != 1 || !args[0]->IsString())
        return v8::ThrowException(v8::String::New("Invalid argument"));

    std::string file(*v8::String::Utf8Value(args[0]));

    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *gems_structure = static_cast<GemsStructure*>(wrap->Value());

    gmml::Structure *structure = gems_structure->structure();
    gmml::MinimizationResults *results = structure->minimize(file);

    if (results == NULL)
        return v8::ThrowException(v8::String::New("Error in minimization"));

    return v8::Undefined();
}

Handle<Value> StructureTemplate::Impl::SetDihedral(const Arguments& args) {
    if (args.Length() != 9)
        return ThrowException(String::New("Invalid parameters"));

    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *structure = static_cast<GemsStructure*>(wrap->Value());

    int residues[4];
    for (int i = 0; i < 4; i ++) {
        if (args[2*i]->IsNumber()) {
            residues[i] = args[2*i]->IntegerValue();
        } else if (args[2*i]->IsString()) {
            int index = structure->map_id(*v8::String::Utf8Value(args[2*i]));
            if (index == -1)
                return v8::ThrowException(v8::String::New("Id not found"));
            residues[i] = index;
        } else {
            return v8::ThrowException(v8::String::New("Invalid arguments"));
        }
        if (!args[2*i + 1]->IsString())
            return v8::ThrowException(v8::String::New("Invalid arguments"));
    }

    if (!args[8]->IsNumber())
        return v8::ThrowException(v8::String::New("Invalid angle measure."));

    structure->structure()->set_dihedral(residues[0],
                                         *v8::String::Utf8Value(args[1]),
                                         residues[1],
                                         *v8::String::Utf8Value(args[3]),
                                         residues[2],
                                         *v8::String::Utf8Value(args[5]),
                                         residues[3],
                                         *v8::String::Utf8Value(args[7]),
                                         args[8]->NumberValue());

    return StructureWrapper::wrap(structure);
}

Handle<Value> StructureTemplate::Impl::Solvate(const Arguments& args) {
    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *gems_structure = static_cast<GemsStructure*>(wrap->Value());
    gmml::Structure *structure = gems_structure->structure();

    if (!args[0]->IsString()) {
        return v8::ThrowException(String::New("Invalid argument 1"));
    }

    std::string solvent_name = *v8::String::Utf8Value(args[0]);
    gmml::Structure *solvent = gmml::build(solvent_name);
    if (solvent == NULL) {
        return v8::ThrowException(String::New("Invalid solvent"));
    }

    if (!args[1]->IsNumber()) {
        return v8::ThrowException(v8::String::New("Invalid argument 2"));
    }

    double distance = args[1]->NumberValue();

    double closeness = 1.0;

    if (args.Length() > 2) {
        if (!args[2]->IsNumber())
            return v8::ThrowException(v8::String::New("Invalid argument 3"));
        closeness = args[2]->NumberValue();
    }

    gmml::SolvatedStructure *solvated = gmml::solvate(*structure, *solvent,
                                                      distance, closeness);

    return StructureWrapper::wrap(new GemsStructure(solvated));
}

Handle<Value> StructureTemplate::Impl::Atoms(const Arguments& args) {
    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *gems_structure = static_cast<GemsStructure*>(wrap->Value());
    gmml::Structure *structure = gems_structure->structure();

    if (args.Length() == 1) {
        if (!args[0]->IsNumber())
            return Undefined();
        int int_val = args[0]->IntegerValue();
        if (int_val < 0 || int_val >= structure->size())
            return Undefined();
        return AtomWrapper::wrap(new GemsAtom(structure, int_val));
    } else if (args.Length() == 0) {
        Local<Array> array = Array::New(structure->size());
        for (int i = 0; i < structure->size(); i++) {
            array->Set(i, AtomWrapper::wrap(new GemsAtom(structure, i)));
        }
        return array;
    }
    return Undefined();
}

Handle<Value> StructureTemplate::Impl::PrintCoordinateFile(
        const Arguments& args) {
    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *structure = static_cast<GemsStructure*>(wrap->Value());

    try {
        structure->structure()->print_coordinate_file(
                *String::Utf8Value(args[0]));
    } catch(const std::exception& e) {
        return ThrowException(String::New(e.what()));
    }

    return Undefined();
}

Handle<Value> StructureTemplate::Impl::PrintTopologyFile(
        const Arguments& args) {
    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *structure = static_cast<GemsStructure*>(wrap->Value());

    try {
        structure->structure()->print_amber_top_file(
                *String::Utf8Value(args[0]));
    } catch(const std::exception& e) {
        return ThrowException(String::New(e.what()));
    }

    return Undefined();
}

Handle<Value> StructureTemplate::Impl::GetSize(Local<String> property,
                                               const AccessorInfo& info) {
    HandleScope scope;

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *structure = static_cast<GemsStructure*>(wrap->Value());
    return scope.Close(v8::Integer::New(structure->structure()->size()));
}

Handle<Value> StructureTemplate::Impl::Residues(const Arguments& args) {
    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsStructure *gems_structure = static_cast<GemsStructure*>(wrap->Value());
    gmml::Structure *structure = gems_structure->structure();

    if (args.Length() != 1) {
        return v8::ThrowException(
                v8::String::New("Function takes 1 argument"));
    }

    if (args[0]->IsNumber()) {
        int index = args[0]->IntegerValue();
        if (index < 0 || index > structure->residue_count())
            return v8::Undefined();
        return ResidueWrapper::wrap(new GemsResidue(structure, index));
    } else if (args[0]->IsString()) {
        std::string id(*v8::String::Utf8Value(args[0]));
        int index = gems_structure->map_id(id);
        if (index == -1) {
            std::string error = "Id " + id + " not found.";
            return v8::ThrowException(v8::String::New(error.c_str()));
        }
        return ResidueWrapper::wrap(new GemsResidue(structure, index));
    }

    return v8::Undefined();
}

Handle<Value> StructureConstructor(const Arguments& args) {
    if (!args.IsConstructCall())
        return ThrowException(
            String::New("Cannot call constructor as function"));

    HandleScope scope;

    Structure *structure = new Structure;

    return scope.Close(StructureWrapper::wrap(new GemsStructure(structure)));
}

}  // namespace gems
