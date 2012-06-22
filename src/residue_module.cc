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

#include "residue_module.h"

#include "gmml/gmml.h"

#include "structure_module.h"

using gmml::Structure;

using namespace v8;

namespace gems {
namespace {

Structure *extract_structure(Handle<Object> object) {
    return StructureModule::extract_ptr(object->Get(String::New("structure")));
}

int extract_index(Handle<Object> object) {
    return object->Get(String::New("index"))->ToInteger()->Value();
}

Handle<Value> get_size(Local<String> property, const AccessorInfo& info) {
    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);
 
    return Integer::New(structure->residues(index)->size());
}

Handle<Value> get_name(Local<String> property, const AccessorInfo& info) {
    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    return String::New(structure->residues(index)->name().c_str());
}

void set_name(Local<String> property, Local<Value> value,
              const AccessorInfo& info) {
    if (!value->IsString()) {
        ThrowException(String::New("Value is not a string."));
        return;
    }

    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    structure->residues(index)->set_name(*String::Utf8Value(value));
}

Handle<Value> get_residue_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->SetAccessor(String::New("name"), get_name, set_name);

    object->SetAccessor(String::New("size"), get_size);

    return scope.Close(object);
}

}  // namespace

void ResidueModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_getResiduePrototype"),
                FunctionTemplate::New(get_residue_prototype));
}

}  // namespace gems
