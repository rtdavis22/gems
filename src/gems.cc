// Author: Robert Davis

#include "gems.h"

#include <exception>

#include "gmml/gmml.h"

#include "pdb_structure.h"
#include "structure.h"

using gmml::Structure;

using v8::AccessorInfo;
using v8::Arguments;
using v8::Array;
using v8::Context;
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

void Gems::Init(v8::Handle<v8::ObjectTemplate> global) {
    global->Set(v8::String::New("Structure"),
                v8::FunctionTemplate::New(StructureConstructor));

    global->Set(v8::String::New("add_path"),
                v8::FunctionTemplate::New(AddPath));

    global->Set(v8::String::New("build_glycam"),
                v8::FunctionTemplate::New(BuildGlycam));

    global->Set(String::New("load_library_file"),
                FunctionTemplate::New(LoadLibraryFile));

    global->Set(v8::String::New("load_parameter_file"),
                v8::FunctionTemplate::New(LoadParameterFile));

    global->Set(v8::String::New("load_prep_file"),
                v8::FunctionTemplate::New(LoadPrepFile));

    global->Set(v8::String::New("load_pdb"),
                v8::FunctionTemplate::New(load_pdb));

    global->Set(v8::String::New("add_residue_mapping"),
                v8::FunctionTemplate::New(AddResidueMapping));

    global->Set(String::New("add_head_mapping"),
                v8::FunctionTemplate::New(AddHeadMapping));

    global->Set(String::New("add_tail_mapping"),
                v8::FunctionTemplate::New(AddTailMapping));
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

Handle<Value> Gems::LoadLibraryFile(const v8::Arguments& args) {
    HandleScope scope;

    String::Utf8Value file(args[0]);
    gmml::load_library_file(*file);

    return scope.Close(v8::Undefined());
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

v8::Handle<v8::Value> Gems::AddResidueMapping(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsString())
        return ThrowException(String::New("Invalid arguments"));

    gmml::add_residue_mapping(*String::Utf8Value(args[0]),
                              *String::Utf8Value(args[1]));

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> Gems::AddHeadMapping(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsString())
        return ThrowException(String::New("Invalid arguments"));

    gmml::add_head_mapping(*String::Utf8Value(args[0]),
                           *String::Utf8Value(args[1]));

    return scope.Close(v8::Undefined());

}

v8::Handle<v8::Value> Gems::AddTailMapping(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsString())
        return ThrowException(String::New("Invalid arguments"));

    gmml::add_tail_mapping(*String::Utf8Value(args[0]),
                           *String::Utf8Value(args[1]));

    return scope.Close(v8::Undefined());

}

}  // namespace gems
