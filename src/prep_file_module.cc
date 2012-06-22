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

#include "prep_file_module.h"

#include <string>

#include "gmml/gmml.h"

#include "structure_module.h"

using std::string;

using namespace v8;

using gmml::PrepFile;
using gmml::Structure;

namespace gems {
namespace {

Structure *extract_structure(Handle<Object> object) {
    return StructureModule::extract_ptr(object->Get(String::New("structure")));
}

int extract_index(Handle<Object> object) {
    return object->Get(String::New("index"))->ToInteger()->Value();
}

PrepFile *extract_ptr(Handle<Value> value) {
    Local<Object> object = value->ToObject();
    Local<Value> external_value =
            object->GetHiddenValue(String::New("pointer"));
    Local<External> external = Local<External>::Cast(external_value);
    return static_cast<PrepFile*>(external->Value());
}

Handle<Value> init(const Arguments& args) {
    PrepFile *prep_file = new PrepFile;
    Local<Object> obj = args.This();
    obj->SetHiddenValue(String::New("pointer"), External::New(prep_file));
    return Undefined();
}

Handle<Value> add_residue(const Arguments& args) {
    PrepFile *prep_file = extract_ptr(args.This());

    Structure *structure = extract_structure(args[0]->ToObject());

    int index = extract_index(args[0]->ToObject());

    //gmml::Residue *residue = structure->residues(index);

    //prep_file->add_residue(residue);
}

Handle<Value> print(const Arguments& args) {
    PrepFile *prep_file = extract_ptr(args.This());
    prep_file->print(*String::Utf8Value(args[0]));
}

Handle<Value> get_prep_file_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->Set(String::New("_init"),
                FunctionTemplate::New(init)->GetFunction());

    object->Set(String::New("addResidue"),
                FunctionTemplate::New(add_residue)->GetFunction());

    object->Set(String::New("print"),
                FunctionTemplate::New(print)->GetFunction());

    return scope.Close(object);
}

}  // namespace

void PrepFileModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_getPrepFilePrototype"),
                FunctionTemplate::New(get_prep_file_prototype));
}

}  // namespace gems
