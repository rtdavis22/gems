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

Handle<Value> print_pdb_file(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    structure->print_pdb_file(*String::Utf8Value(args[0]));
    return Undefined();
}

Handle<Value> print_restart_file(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    structure->print_coordinate_file(*String::Utf8Value(args[0]));
    return Undefined();
}

Handle<Value> print_topology_file(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    try {
        structure->print_amber_top_file(*String::Utf8Value(args[0]));
    } catch(const std::exception& e) {
        return ThrowException(String::New(e.what()));
    }
    return Undefined();
}

Handle<Value> set_head(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    structure->set_head(args[0]->IntegerValue(),
                        *String::Utf8Value(args[1]));
    return Undefined();
}

Handle<Value> set_tail(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    structure->set_tail(args[0]->IntegerValue(),
                        *String::Utf8Value(args[1]));
    return Undefined();
}

Handle<Value> get_structure_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->Set(String::New("printPdbFile"),
                FunctionTemplate::New(print_pdb_file)->GetFunction());

    object->Set(String::New("printRestartFile"),
                FunctionTemplate::New(print_restart_file)->GetFunction());

    object->Set(String::New("printTopologyFile"),
                FunctionTemplate::New(print_topology_file)->GetFunction());

    object->Set(String::New("setHead"),
                FunctionTemplate::New(set_head)->GetFunction());

    object->Set(String::New("setTail"),
                FunctionTemplate::New(set_tail)->GetFunction());

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
