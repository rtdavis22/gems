#include "file_io.h"

#include "gmml/gmml.h"

#include <exception>
#include <string>

namespace gems {

v8::Handle<v8::Value> FileIO::LoadLibraryFile(const v8::Arguments& args) {
    v8::HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString())
        return v8::ThrowException(v8::String::New("Invalid argument"));

    v8::String::Utf8Value file(args[0]);

    try {
        gmml::load_library_file(*file);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> FileIO::LoadParameterFile(const v8::Arguments& args) {
    v8::HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString())
        return ThrowException(v8::String::New("Invalid argument"));

    v8::String::Utf8Value file(args[0]);

    try {
        gmml::load_parameter_file(*file);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> FileIO::LoadPrepFile(const v8::Arguments& args) {
    v8::HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString())
        return v8::ThrowException(v8::String::New("Invalid argument"));

    v8::String::Utf8Value file(args[0]);

    try {
        gmml::load_prep_file(*file);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    return scope.Close(v8::Undefined());
}

}  // namespace gems
