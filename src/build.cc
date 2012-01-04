#include "build.h"

#include "gmml/gmml.h"

#include "structure.h"

#include <string>

namespace gems {

v8::Handle<v8::Value> Build::JSBuild(const v8::Arguments& args) {
    if (args.Length() != 1) {
        return v8::ThrowException(v8::String::New("Function takes 1 argument"));
    }

    gmml::Structure *structure = NULL;

    try {
        structure = Build()(args[0]);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    if (structure == NULL)
        return v8::ThrowException(v8::String::New("Invalid argument"));

    return StructureWrapper::wrap(structure);
}

gmml::Structure *Build::operator()(v8::Handle<v8::Value> value) {
    if (value->IsArray()) {
        return operator()(v8::Array::Cast(*value));
    } else if (value->IsObject()) {
        return operator()(value->ToObject());
    } else if (value->IsString()) {
        //HANDLE
    }
        
    return NULL;
}

gmml::Structure *Build::operator()(v8::Array *array) {
    gmml::Structure *structure = new gmml::Structure;
    structure->append(operator()(array->Get(0)));

    for (int i = 1; i < array->Length(); i++) {
        
    } 

    return structure;
}

// Check if it has an internal pointer or not to see if its a structure
// or a regular json object
gmml::Structure *Build::operator()(v8::Handle<v8::Object> object) {
    if (!object->Has(v8::String::New("code")))
        return NULL;

    std::string code = *v8::String::Utf8Value(
            object->Get(v8::String::New("code")));

    gmml::Structure *structure = gmml::build(code);

    return structure;
}

}  // namespace gems
