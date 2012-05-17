// Author: Robert Davis

#include "environment.h"

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
