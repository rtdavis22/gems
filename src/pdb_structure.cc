// Author: Robert Davis

#include "pdb_structure.h"

#include <exception>
#include <map>
#include <string>

#include "gmml/gmml.h"

#include "atom.h"
#include "residue.h"
#include "structure.h"

using std::map;
using std::string;

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

    v8::Handle<v8::String> invalid_error(v8::String::New("Invalid arguments"));

    Handle<v8::Object> object = args[0]->ToObject();
    if (!object->Has(v8::String::New("res_num"))) {
        return v8::ThrowException(v8::String::New("No res_num property"));
    }

    int res_num = object->Get(v8::String::New("res_num"))->ToNumber()->IntegerValue();

    char chain_id = ' ';
    char i_code = ' ';

    if (object->Has(v8::String::New("chain_id"))) {
        Handle<Value> value = object->Get(v8::String::New("chain_id"));
        std::string chain_id_str(*v8::String::Utf8Value(value));
        if (chain_id_str.size() != 1)
            return v8::ThrowException(invalid_error);
        chain_id = chain_id_str[0];
    }

    if (object->Has(v8::String::New("i_code"))) {
        Handle<Value> value = object->Get(v8::String::New("i_code"));
        std::string i_code_str(*v8::String::Utf8Value(value));
        if (i_code_str.size() != 1)
            return v8::ThrowException(invalid_error);
        i_code = i_code_str[0];
    }

    int index = structure->map_residue(chain_id, res_num, i_code);

    if (index == -1)
        return v8::Undefined();

    return ResidueWrapper::wrap(new GemsResidue(structure, index));
}

/*
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
*/

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

namespace {

struct ResidueIdentifier {

    ResidueIdentifier(char chain_id, int res_num, char i_code)
        : chain_id(chain_id), res_num(res_num), i_code(i_code) {}

    char chain_id;
    int res_num;
    char i_code;
};

ResidueIdentifier *get_residue_identifier(v8::Handle<v8::Object> object) {
    if (!object->Has(v8::String::New("res_num"))) {
        return NULL;
    }

    int res_num = object->Get(v8::String::New("res_num"))->ToNumber()->Value();
    char chain_id = ' ';
    char i_code = ' ';

    if (object->Has(v8::String::New("chain_id"))) {
        v8::Local<v8::String> v8_chain_id = 
                object->Get(v8::String::New("chain_id"))->ToString();
        std::string std_chain_id(*v8::String::Utf8Value(v8_chain_id));
        chain_id = std_chain_id[0];
    }

    if (object->Has(v8::String::New("i_code"))) {
        v8::Local<v8::String> v8_i_code =
                object->Get(v8::String::New("i_code"))->ToString();
        std::string std_i_code(*v8::String::Utf8Value(v8_i_code));
        i_code = std_i_code[0];
    }

    return new ResidueIdentifier(chain_id, res_num, i_code);
}

}  // namespace



Handle<Value> load_pdb(const Arguments& args) {
    v8::HandleScope scope;

    if (args.Length() != 1 && args.Length() != 2)
        return v8::ThrowException(v8::String::New("Invalid argument"));

    if (!args[0]->IsString())
        return v8::ThrowException(v8::String::New("First arg must be a file"));

    v8::String::Utf8Value file(args[0]);

    gmml::PdbFile *pdb_file = NULL;

    try {
       pdb_file = new gmml::PdbFile(*file);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    gmml::PdbStructureBuilder builder(*pdb_file);

    if (args.Length() == 2) {
         v8::Local<v8::Array> array = v8::Array::Cast(*args[1]);
         for (int i = 0; i < array->Length(); i++) {
             v8::Local<v8::Object> mapping(array->Get(i)->ToObject());

             if (!mapping->Has(v8::String::New("to"))) {
                 return v8::ThrowException(v8::String::New("no to"));
             }

             v8::Local<v8::Value> to_v8 = mapping->Get(v8::String::New("to"));
             if (!to_v8->IsString()) {
                 return v8::ThrowException(v8::String::New("to not string"));
             }

             std::string to(*v8::String::Utf8Value(to_v8->ToString()));

             if (!mapping->Has(v8::String::New("from"))) {
                 return v8::ThrowException(v8::String::New("no from"));
             }

             v8::Handle<v8::Value> from_value =
                     mapping->Get(v8::String::New("from"));
             if (from_value->IsString()) {
                 string from(*v8::String::Utf8Value(from_value->ToString()));
                 builder.add_mapping(from, to);
             } else if (from_value->IsObject()) {
                 ResidueIdentifier *id =
                         get_residue_identifier(from_value->ToObject());
                 if (id == NULL) {
                     return v8::ThrowException(v8::String::New("bad obj"));
                 }
                 builder.add_mapping(id->chain_id, id->res_num, id->i_code,
                                     to);
             } else {
                 // error here
             }
         }
    }

    gmml::PdbFileStructure *structure = builder.build();

    return scope.Close(PdbStructureWrapper::wrap(new GemsPDBStructure(structure)));
}

}
