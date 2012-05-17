// Author: Robert Davis

#include "structure.h"

#include "gmml/gmml.h"

using gmml::Structure;

using namespace v8;

namespace gems {
namespace {

Handle<Value> get_size(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    Structure *structure = StructureModule::extract_ptr(info.This());
    return scope.Close(Integer::New(structure->size()));
}

Handle<Value> get_structure_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->SetAccessor(String::New("size"), get_size);

    return scope.Close(object);
}

}  // namespace

void StructureModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_getStructurePrototype"),
                FunctionTemplate::New(get_structure_prototype));
}

Structure *StructureModule::extract_ptr(Handle<Value> value) {
    Local<Object> object = value->ToObject();
    Local<Value> external_value =
            object->GetHiddenValue(String::New("pointer"));
    Local<External> external = Local<External>::Cast(external_value);
    return static_cast<Structure*>(external->Value());
}

}  // namespace gems
