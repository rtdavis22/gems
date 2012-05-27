// Author: Robert Davis

#include "atom.h"

#include "gmml/gmml.h"

#include "structure.h"

using gmml::Coordinate;
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

Handle<Value> get_charge(Local<String> property, const AccessorInfo& info) {
    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    return Number::New(structure->atoms(index)->charge());
}

void set_charge(Local<String> property, Local<Value> value,
                const AccessorInfo& info) {
    if (!value->IsNumber()) {
        ThrowException(String::New("Value is not a string."));
        return;
    }

    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    structure->atoms(index)->set_charge(value->NumberValue());
}

Handle<Value> get_coordinate(Local<String> property, const AccessorInfo& info) {
    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    const Coordinate& coordinate = structure->atoms(index)->coordinate();

    Handle<Object> object = Object::New();
    object->Set(String::New("x"), Number::New(coordinate.x));
    object->Set(String::New("y"), Number::New(coordinate.y));
    object->Set(String::New("z"), Number::New(coordinate.z));

    return object;
}
 
void set_coordinate(Local<String> property, Local<Value> value,
                    const AccessorInfo& info) {
    Local<Object> coordinate = value->ToObject();
    double x = coordinate->Get(String::New("x"))->ToNumber()->Value();
    double y = coordinate->Get(String::New("y"))->ToNumber()->Value();
    double z = coordinate->Get(String::New("z"))->ToNumber()->Value();

    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    structure->atoms(index)->set_coordinate(x, y, z);
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

Handle<Value> get_type(Local<String> property, const AccessorInfo& info) {
    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    return String::New(structure->atoms(index)->type().c_str());
}

void set_type(Local<String> property, Local<Value> value,
              const AccessorInfo& info) {
    if (!value->IsString()) {
        ThrowException(String::New("Value is not a string."));
        return;
    }

    Local<Object> self = info.This();

    Structure *structure = extract_structure(self);

    int index = extract_index(self);

    structure->atoms(index)->set_type(*String::Utf8Value(value));
}

Handle<Value> get_atom_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->SetAccessor(String::New("charge"), get_charge, set_charge);
    object->SetAccessor(String::New("coordinate"), get_coordinate, set_coordinate);
    object->SetAccessor(String::New("name"), get_name, set_name);
    object->SetAccessor(String::New("type"), get_type, set_type);

    return scope.Close(object);
}

}  // namespace

void AtomModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_getAtomPrototype"),
                FunctionTemplate::New(get_atom_prototype));
}

}  // namespace gems
