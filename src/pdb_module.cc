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
using gmml::Structure;

using namespace v8;

namespace gems {
namespace {

Handle<Value> init_pdb_file_structure(const Arguments& args) {
    HandleScope scope;
    Local<Object> obj = args.This();
    obj->SetHiddenValue(String::New("pointer"), External::New(new Structure));
    return scope.Close(Undefined());
}

Handle<Value> get_pdb_structure_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    return scope.Close(object);
}

class GetPdbFileBuilder {
  public:
    explicit GetPdbFileBuilder(Handle<Object> options)
            : builder_(NULL), options_(options) {
    }

    PdbFileBuilder *operator()() {
        builder_ = new PdbFileBuilder;
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

// This function assumes the following:
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
