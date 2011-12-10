// Author: Robert Davis

#include "gems.h"

#include <exception>

#include "gmml/gmml.h"

#include "structure.h"

using gmml::Structure;

namespace gems {

void Gems::Init(v8::Handle<v8::ObjectTemplate> global) {
    global->Set(v8::String::New("Structure"),
                v8::FunctionTemplate::New(StructureConstructor));

    global->Set(v8::String::New("add_path"),
                v8::FunctionTemplate::New(AddPath));

    global->Set(v8::String::New("build_glycam"),
                v8::FunctionTemplate::New(BuildGlycam));

    global->Set(v8::String::New("load_parameter_file"),
                v8::FunctionTemplate::New(LoadParameterFile));

    global->Set(v8::String::New("load_prep_file"),
                v8::FunctionTemplate::New(LoadPrepFile));
}

v8::Handle<v8::Value> Gems::BuildGlycam(const v8::Arguments& args) {
    v8::HandleScope scope;

    Structure *structure = NULL;

    v8::String::Utf8Value glycam_string(args[0]);
    try {
        structure = gmml::glycam_build(*glycam_string);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    return scope.Close(StructureWrapper::wrap(structure));
}

v8::Handle<v8::Value> Gems::LoadParameterFile(const v8::Arguments& args) {
    v8::HandleScope scope;

    v8::String::Utf8Value file(args[0]);
    gmml::load_parameter_file(*file);

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Gems::LoadPrepFile(const v8::Arguments& args) {
    v8::HandleScope scope;

    v8::String::Utf8Value file(args[0]);
    gmml::load_prep_file(*file);

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Gems::AddPath(const v8::Arguments& args) {
    v8::HandleScope scope;

    v8::String::Utf8Value path(args[0]);
    gmml::add_path(*path);

    return scope.Close(v8::Undefined());
}

}  // namespace gems
