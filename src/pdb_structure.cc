// Author: Robert Davis

#include "pdb_structure.h"

#include <exception>

#include "gmml/gmml.h"

#include "atom.h"
#include "residue.h"
#include "structure.h"

using std::map;

using gmml::Atom;
using gmml::PdbFileStructure;

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

Handle<v8::Object> PdbStructureWrapper::wrap(GemsPDBStructure *structure) {
    HandleScope handle_scope;

    static PdbStructureTemplate structure_template;
    Persistent<FunctionTemplate> template_ = structure_template.get_template();

    Persistent<v8::Object> instance = Persistent<v8::Object>::New(
        template_->InstanceTemplate()->NewInstance());

    instance.MakeWeak(structure, callback);

    instance->SetInternalField(0, External::New(structure));

    return handle_scope.Close(instance);
}

void PdbStructureWrapper::callback(Persistent<Value> object, void *data) {
    object.Dispose();
    object.Clear();
}

struct PdbStructureTemplate::Impl {
    static Handle<Value> GetPdbThing(Local<String> property,
                                     const AccessorInfo& info);
    static Handle<Value> PdbResidue(const Arguments& args);
};

Handle<Value> PdbStructureTemplate::Impl::GetPdbThing(
        Local<String> property,
        const AccessorInfo& info) {
    return v8::Integer::New(123123);
}

Handle<Value> PdbStructureTemplate::Impl::PdbResidue(const Arguments& args) {
    if (args.Length() < 1 || args.Length() > 3)
        return v8::ThrowException(
                v8::String::New("Function takes 1-3 arguments"));

    Local<v8::Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsPDBStructure *gems_structure =
            static_cast<GemsPDBStructure*>(wrap->Value());
    gmml::PdbFileStructure *structure = gems_structure->structure();

    int index = -1;

    v8::Handle<v8::String> invalid_error(v8::String::New("Invalid arguments"));

    if (args.Length() == 1) {
        if (!args[0]->IsNumber()) {
            return v8::ThrowException(invalid_error);
        }
        int int_val = args[0]->IntegerValue();
        index = structure->map_residue(int_val);
        //return ResidueWrapper::wrap(new GemsResidue(structure, residue_index));
    } else {
        if (!args[0]->IsString())
            return v8::ThrowException(invalid_error);
        std::string chain_id(*v8::String::Utf8Value(args[0]));
        if (chain_id.size() != 1)
            return v8::ThrowException(invalid_error);
        char chain_id_char = chain_id[0];

        if (!args[1]->IsNumber())
            return v8::ThrowException(invalid_error);

        int residue_index = args[1]->IntegerValue();

        if (args.Length() == 2) {
            index = structure->map_residue(chain_id_char, residue_index);
        } else {
            if (!args[2]->IsString())
                return v8::ThrowException(invalid_error);
            std::string i_code(*v8::String::Utf8Value(args[2]));
            if (i_code.size() != 1)
                return v8::ThrowException(invalid_error);
            char i_code_char = i_code[0];
            index = structure->map_residue(chain_id_char, residue_index,
                                           i_code_char);
        }
    }

    if (index == -1)
        return v8::Undefined();

    return ResidueWrapper::wrap(new GemsResidue(structure, index));
    
}

void PdbStructureTemplate::Init() {
    HandleScope scope;

    Handle<FunctionTemplate> local_template = FunctionTemplate::New();
    Handle<ObjectTemplate> instance_template =
        local_template->InstanceTemplate();
    instance_template->SetInternalFieldCount(1);

    instance_template->SetAccessor(String::New("get_pdb_thing"),
                                   Impl::GetPdbThing);

    StructureTemplate structure_template;
    local_template->Inherit(structure_template.get_template());

    template_ = Persistent<FunctionTemplate>::New(local_template);

    NODE_SET_PROTOTYPE_METHOD(template_, "pdb_residue", Impl::PdbResidue);
}

Handle<Value> load_pdb(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString())
        return ThrowException(String::New("Invalid argument"));

    String::Utf8Value file(args[0]);

    gmml::PdbFileStructure *structure = NULL;

    try {
        structure = PdbFileStructure::build(*file);
    } catch(const std::exception& e) {
        return ThrowException(v8::String::New(e.what()));
    }

    return scope.Close(
            PdbStructureWrapper::wrap(new GemsPDBStructure(structure)));
}

}
