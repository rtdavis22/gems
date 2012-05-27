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

#include "environment_module.h"

#include "gmml/gmml.h"

using namespace v8;

namespace gems {
namespace {

Handle<Value> add_path(const Arguments& args) {
    HandleScope scope;

    gmml::add_path(*String::Utf8Value(args[0]));

    return scope.Close(Undefined());
}

Handle<Value> load_prep_file(const Arguments& args) {
    HandleScope scope;

    try {
        gmml::load_prep_file(*String::Utf8Value(args[0]));
    } catch(const std::exception& e) {
        return scope.Close(ThrowException(String::New(e.what())));
    }

    return scope.Close(Undefined());
}

Handle<Value> load_library_file(const Arguments& args) {
    HandleScope scope;

    try {
        gmml::load_library_file(*String::Utf8Value(args[0]));
    } catch(const std::exception& e) {
        return scope.Close(ThrowException(String::New(e.what())));
    }

    return scope.Close(Undefined());
}

Handle<Value> load_parameter_file(const Arguments& args) {
    HandleScope scope;

    try {
        gmml::load_parameter_file(*String::Utf8Value(args[0]));
    } catch(const std::exception& e) {
        return scope.Close(ThrowException(String::New(e.what())));
    }

    return scope.Close(Undefined());
}

Handle<Value> add_residue_mapping(const Arguments& args) {
    HandleScope scope;

    gmml::add_residue_mapping(*String::Utf8Value(args[0]),
                              *String::Utf8Value(args[1]));

    return scope.Close(Undefined());
}

Handle<Value> add_head_mapping(const Arguments& args) {
    HandleScope scope;

    gmml::add_head_mapping(*String::Utf8Value(args[0]),
                           *String::Utf8Value(args[1]));

    return scope.Close(Undefined());

}

Handle<Value> add_tail_mapping(const Arguments& args) {
    HandleScope scope;

    gmml::add_tail_mapping(*String::Utf8Value(args[0]),
                           *String::Utf8Value(args[1]));

    return scope.Close(Undefined());
}

}  // namespace

void EnvironmentModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_addPath"),
                FunctionTemplate::New(add_path));

    global->Set(String::New("_loadPrepFile"),
                FunctionTemplate::New(load_prep_file));

    global->Set(String::New("_loadLibraryFile"),
                FunctionTemplate::New(load_library_file));

    global->Set(String::New("_loadParameterFile"),
                FunctionTemplate::New(load_parameter_file));

    global->Set(String::New("_addResidueMapping"),
                FunctionTemplate::New(add_residue_mapping));

    global->Set(String::New("_addHeadMapping"),
                FunctionTemplate::New(add_head_mapping));

    global->Set(String::New("_addTailMapping"),
                FunctionTemplate::New(add_tail_mapping));
}

}  // namespace gems
