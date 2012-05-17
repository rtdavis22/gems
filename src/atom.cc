// Author: Robert Davis

#include "atom.h"

#include "gmml/gmml.h"

#include "structure.h"

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

Handle<Value> get_name(Local<String> property, const AccessorInfo& info) {
    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    return String::New(structure->atoms(index)->name().c_str());
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

    structure->atoms(index)->set_name(*String::Utf8Value(value));
}

Handle<Value> get_atom_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->SetAccessor(String::New("name"), get_name, set_name);

    return scope.Close(object);
}

}  // namespace

void AtomModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_getAtomPrototype"),
                FunctionTemplate::New(get_atom_prototype));
}

}  // namespace gems
