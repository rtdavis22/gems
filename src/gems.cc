// Author: Robert Davis

#include "gems.h"

#include <exception>
#include <vector>

#include "gmml/gmml.h"

#include "pdb_structure.h"
#include "structure.h"

#include "atom.h"
#include "build.h"
#include "coordinate.h"
#include "file_io.h"

using std::vector;

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

    global->Set(v8::String::New("build"),
                v8::FunctionTemplate::New(Build::JSBuild));

    global->Set(v8::String::New("build_glycam"),
                v8::FunctionTemplate::New(BuildGlycam));

    global->Set(String::New("load_library_file"),
                FunctionTemplate::New(FileIO::LoadLibraryFile));

    global->Set(v8::String::New("load_parameter_file"),
                v8::FunctionTemplate::New(FileIO::LoadParameterFile));

    global->Set(v8::String::New("load_prep_file"),
                v8::FunctionTemplate::New(FileIO::LoadPrepFile));

    global->Set(v8::String::New("load_pdb"),
                v8::FunctionTemplate::New(load_pdb));

    global->Set(v8::String::New("add_residue_mapping"),
                v8::FunctionTemplate::New(AddResidueMapping));

    global->Set(String::New("add_head_mapping"),
                v8::FunctionTemplate::New(AddHeadMapping));

    global->Set(String::New("add_tail_mapping"),
                v8::FunctionTemplate::New(AddTailMapping));

    global->Set(String::New("measure"), v8::FunctionTemplate::New(Measure));
}

v8::Handle<v8::Value> Gems::BuildGlycam(const v8::Arguments& args) {
    v8::HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString())
        return ThrowException(String::New("Invalid argument"));

    Structure *structure = NULL;

    v8::String::Utf8Value glycam_string(args[0]);
    try {
        structure = gmml::glycam_build(*glycam_string);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    return scope.Close(StructureWrapper::wrap(new GemsStructure(structure)));
}

v8::Handle<v8::Value> Gems::AddPath(const v8::Arguments& args) {
    v8::HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString())
        return ThrowException(String::New("Invalid argument"));

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

Handle<Value> Gems::Measure(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 2 || args.Length() > 4) {
        return ThrowException(String::New("Function requires 2-4 arguments."));
    }

    vector<gmml::Coordinate*> coordinates(args.Length(), NULL);
                                          
    for (int i = 0; i < args.Length(); i++) {
        gmml::Coordinate *coordinate = ExtractCoordinate()(args[i]);
        if (coordinate == NULL) {
            for (int j = 0; j < i; j++)
                delete coordinates[j];
            return ThrowException(String::New("Invalid arguments."));
        }
        coordinates[i] = coordinate;
    }

    double measure = 0.0;

    if (coordinates.size() == 2) {
        measure = gmml::measure(*coordinates[0], *coordinates[1]);
    } else if (coordinates.size() == 3) {
        measure = gmml::measure(*coordinates[0], *coordinates[1],
                                *coordinates[2]);
        measure = gmml::to_degrees(measure);
    } else if (coordinates.size() == 4) {
        measure = gmml::measure(*coordinates[0], *coordinates[1],
                                *coordinates[2], *coordinates[3]);
        measure = gmml::to_degrees(measure);
    }

    for (int i = 0; i < coordinates.size(); i++)
        delete coordinates[i];

    return scope.Close(v8::Number::New(measure));
}

}  // namespace gems
