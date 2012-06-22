// Copyright (c) 2012 The University of Georgia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Author: Robert Davis

#include "pdb_module.h"

#include <string>

#include "gmml/gmml.h"

#include "structure_module.h"

using std::string;

using gmml::PdbFile;
using gmml::PdbFileBuilder;
using gmml::PdbFileStructure;
using gmml::PdbResidueId;
using gmml::PdbStructureBuilder;
using gmml::Structure;

using namespace v8;

namespace gems {
namespace {

class ExtractPdbResidueId {
  public:
    explicit ExtractPdbResidueId(Handle<Object> object) : object_(object) {
    }

    // Returns NULL if the object doesn't have residue id information.
    PdbResidueId *operator()() {
        int res_num = get_res_num();
        if (res_num == -1)
            return NULL;
        char chain_id = get_chain_id();
        char i_code = get_i_code();
        return new PdbResidueId(chain_id, res_num, i_code);
    }

  private:
    int get_res_num() {
        Local<String> prop_name(String::New("resNum"));
        if (!object_->Has(prop_name))
            return -1;
        return object_->Get(prop_name)->IntegerValue();
    }

    char get_chain_id() {
        return get_char_property("chainId");
    }

    char get_i_code() {
        return get_char_property("iCode");
    }

    // Returns ' ' if the property is not on the object.
    char get_char_property(const string& property) {
        Local<String> prop_name(String::New(property.c_str()));
        if (object_->Has(prop_name)) {
            string value(*String::Utf8Value(object_->Get(prop_name)));
            return value.empty()?' ':value[0];
        } else {
            return ' ';
        }
    }

    Handle<Object> object_;
};

class GetPdbStructureBuilder {
  public:
    explicit GetPdbStructureBuilder(Handle<Object> options)
            : builder_(NULL), options_(options) {
    }

    PdbStructureBuilder *operator()() {
        builder_ = new PdbStructureBuilder;
        check_for_use_residue_mappings();
        check_for_mappings();
        return builder_;
    }

  private:
    void check_for_use_residue_mappings() {
        Local<String> prop_name(String::New("useResidueMap"));
        if (options_->Has(prop_name)) {
            if (options_->Get(prop_name)->BooleanValue()) {
                builder_->use_residue_map();
            } else {
                builder_->ignore_residue_map();
            }
        }
    }

    void check_for_mappings() {
        check_for_residue_mappings();
    }

    void check_for_residue_mappings() {
        Local<String> prop_name(String::New("residueMappings"));
        if (options_->Has(prop_name)) {
            Local<Array> array = Array::Cast(*options_->Get(prop_name));
            for (int i = 0; i < array->Length(); i++) {
                Local<Object> mapping(array->Get(i)->ToObject());
                add_residue_mapping(mapping);
            }    
        }
    }

    void add_residue_mapping(Handle<Object> object) {
        string mapped_name = get_string_property(object, "to");
        Handle<Value> from_value = object->Get(String::New("from"));
        if (from_value->IsString()) {
            builder_->add_mapping(*String::Utf8Value(from_value), mapped_name);
        } else {
            PdbResidueId *pdb_residue_id =
                    ExtractPdbResidueId(from_value->ToObject())();
            builder_->add_mapping(*pdb_residue_id, mapped_name);
            delete pdb_residue_id;
        }
    }

    static string get_string_property(Handle<Object> object,
                                      const string& property) {
        return *String::Utf8Value(object->Get(String::New(property.c_str())));
    }

    void add_head_mappings() {
        Local<String> prop_name(String::New("headMappings"));
        if (options_->Has(prop_name)) {
            Local<Array> array = Array::Cast(*options_->Get(prop_name));
            for (int i = 0; i < array->Length(); i++) {
                Local<Object> object(array->Get(i)->ToObject());
                builder_->add_head_mapping(get_string_property(object, "from"),
                                           get_string_property(object, "to"));
            }
        }
    }

    PdbStructureBuilder *builder_;
    Handle<Object> options_;
};

// Preconditions:
// - args.This() is a PdbFileStructure
// - args[0] is the file name and is a string
// - args[1] is an object with options
Handle<Value> init_pdb_file_structure(const Arguments& args) {
    HandleScope scope;
    Local<Object> obj = args.This();

    PdbStructureBuilder *builder =
            GetPdbStructureBuilder(args[1]->ToObject())();

    gmml::File pdb_file(*String::Utf8Value(args[0]));
    PdbFileStructure *structure = builder->build(pdb_file);
    delete builder;

    obj->SetHiddenValue(String::New("pointer"), External::New(structure));

    return scope.Close(Undefined());
}

Handle<Value> get_residue_index(const Arguments& args) {
    PdbFileStructure *structure =
      static_cast<PdbFileStructure*>(StructureModule::extract_ptr(args.This()));

    Local<Object> residue = args[0]->ToObject();
    PdbResidueId *id = ExtractPdbResidueId(residue)();
    int index = structure->map_residue(*id);
    delete id;

    return Integer::New(index);
}

Handle<Value> get_pdb_structure_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->Set(String::New("_getResidueIndex"),
                FunctionTemplate::New(get_residue_index)->GetFunction());

    return scope.Close(object);
}

class GetPdbFileBuilder {
  public:
    explicit GetPdbFileBuilder(Handle<Object> options)
            : builder_(NULL), options_(options) {
    }

    PdbFileBuilder *operator()() {
        builder_ = new PdbFileBuilder;
        builder_->include_hydrogens();
        check_for_hydrogens_included_property();
        return builder_;
    }

  private:
    void check_for_hydrogens_included_property() {
        Local<String> prop_name(String::New("includeHydrogens"));
        if (options_->Has(prop_name)) {
            if (options_->Get(prop_name)->BooleanValue()) {
                builder_->include_hydrogens();
            } else {
                builder_->dont_include_hydrogens();
            }
        }
    }

    PdbFileBuilder *builder_;
    Handle<Object> options_;
};

// Preconditions:
// - args.This() is a Structure
// - args[0] is the file name and is a string
// - args[1] is an object with options
Handle<Value> print_pdb_file(const Arguments& args) {
    HandleScope scope;

    Structure *structure = StructureModule::extract_ptr(args.This());

    string file_name(*String::Utf8Value(args[0]));

    PdbFileBuilder *builder = GetPdbFileBuilder(args[1]->ToObject())();

    PdbFile *pdb_file = builder->build(*structure);
    delete builder;

    pdb_file->print(file_name);
    delete pdb_file;

    return scope.Close(Undefined());
}

}  // namespace

void PdbModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_getPdbStructurePrototype"),
                FunctionTemplate::New(get_pdb_structure_prototype));

    global->Set(String::New("_initPdbFileStructure"),
                FunctionTemplate::New(init_pdb_file_structure));

    global->Set(String::New("_printPdbFile"),
                FunctionTemplate::New(print_pdb_file));
}

}  // namespace gems
