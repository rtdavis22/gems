// Author: Robert Davis

#include "structure.h"

#include <map>
#include <utility>

#include "gmml/gmml.h"

#include "atom.h"

using std::cout;
using std::endl;
using std::map;

using gmml::Structure;

using v8::Arguments;
using v8::Array;
using v8::External;
using v8::FunctionTemplate;
using v8::Handle;
using v8::HandleScope;
using v8::Local;
using v8::Object;
using v8::ObjectTemplate;
using v8::Persistent;
using v8::String;
using v8::ThrowException;
using v8::Undefined;
using v8::Value;

namespace gems {
namespace {

// This class builds the FunctionTemplate.
class StructureTemplate {
  public:
    StructureTemplate() { Init(); }

    ~StructureTemplate() {
        template_.Dispose();
        template_.Clear();
    }

    v8::Persistent<v8::FunctionTemplate> get_template() const {
        return template_;
    }

    static Handle<Value> Atoms(const Arguments& args);
    static Handle<Value> PrintTopologyFile(const Arguments& args);
    static Handle<Value> SetDihedral(const Arguments& args);

  private:
    void Init();

    v8::Persistent<v8::FunctionTemplate> template_;
};


bool kIsWrapperInitialized = false;

// This is a map to manage the objects created by the wrapper, so we don't
// create a new object for existing structures.
map<Structure*, Persistent<Object> > *kStructureObjectManager = NULL;

// This is lazily invoked by StructureWrapper::wrap.
void initialize_wrapper() {
    kStructureObjectManager = new map<Structure*, Persistent<Object> >;
    kIsWrapperInitialized = true;
}

}  // namespace


void StructureWrapper::destroy() {
    if (kIsWrapperInitialized) {
        delete kStructureObjectManager;
        kIsWrapperInitialized = false;
    }
}

Handle<Object> StructureWrapper::wrap(Structure *structure) {
    HandleScope handle_scope;

    static StructureTemplate structure_template;

    Persistent<FunctionTemplate> template_ = structure_template.get_template();

    if (!kIsWrapperInitialized)
        initialize_wrapper();

    map<Structure*, Persistent<Object> >::iterator it =
        kStructureObjectManager->lower_bound(structure);

    if (it != kStructureObjectManager->end() &&
            !kStructureObjectManager->key_comp()(structure, it->first))
        return it->second;

    Persistent<Object> instance = Persistent<Object>::New(
        template_->InstanceTemplate()->NewInstance());

    instance.MakeWeak(structure, callback);

    kStructureObjectManager->insert(it, std::make_pair(structure, instance));

    instance->SetInternalField(0, External::New(structure));

    return handle_scope.Close(instance);
}


void StructureWrapper::callback(Persistent<Value> object, void *data) {
    Structure *structure = static_cast<Structure*>(data);
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

    //NODE_SET_PROTOTYPE_METHOD(instance_template, "atoms", Atoms);

    instance_template->Set("set_dihedral",
                           FunctionTemplate::New(SetDihedral));
    //instance_template->Set("atoms",
    //                       FunctionTemplate::New(Atoms));
    instance_template->Set("print_topology_file",
                           FunctionTemplate::New(PrintTopologyFile));

    template_ = Persistent<FunctionTemplate>::New(local_template);


    NODE_SET_PROTOTYPE_METHOD(template_, "atoms", Atoms);
}

// Modify and check errors.
Handle<Value> StructureTemplate::SetDihedral(const Arguments& args) {
    if (args.Length() != 5)
        return ThrowException(String::New("Invalid parameters"));

    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Structure *structure = static_cast<Structure*>(wrap->Value());

    structure->set_dihedral(args[0]->IntegerValue(),
                            args[1]->IntegerValue(),
                            args[2]->IntegerValue(),
                            args[3]->IntegerValue(),
                            args[4]->NumberValue());

    return StructureWrapper::wrap(structure);
}

Handle<Value> StructureTemplate::Atoms(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Structure *structure = static_cast<Structure*>(wrap->Value());

    if (args.Length() == 1) {
        if (!args[0]->IsNumber())
            return Undefined();
        int int_val = args[0]->IntegerValue();
        if (int_val < 0 || int_val >= structure->size())
            return Undefined();
        Structure::AtomPtr atom = structure->atoms(int_val);
        return AtomWrapper::wrap(atom);
    } else if (args.Length() == 0) {
        Local<Array> array = Array::New(structure->size());
        for (int i = 0; i < structure->size(); i++) {
            array->Set(i, AtomWrapper::wrap(structure->atoms(i)));
        }
        return array;
    }
    return Undefined();
}

Handle<Value> StructureTemplate::PrintTopologyFile(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Structure *structure = static_cast<Structure*>(wrap->Value());
    structure->print_amber_top_file(*String::Utf8Value(args[0]));
    return Undefined();
}

Handle<Value> StructureConstructor(const Arguments& args) {
    if (!args.IsConstructCall())
        return ThrowException(
            String::New("Cannot call constructor as function"));

    HandleScope scope;

    Structure *structure = new Structure;

    return scope.Close(StructureWrapper::wrap(structure));
}

}  // namespace gems
