// Author: Robert Davis

#include "pdb.h"

#include "gmml/gmml.h"

using gmml::Structure;

using namespace v8;

namespace gems {
namespace {

Handle<Value> init_pdb_file_structure(const Arguments& args) {
    HandleScope scope;
    Local<Object> obj = args.This();
    obj->SetHiddenValue(String::New("pointer"), External::New(new Structure));
    return scope.Close(Undefined());
}

Handle<Value> get_pdb_structure_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    return scope.Close(object);
}

}  // namespace

void PdbModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_getPdbStructurePrototype"),
                FunctionTemplate::New(get_pdb_structure_prototype));

    global->Set(String::New("_initPdbFileStructure"),
                FunctionTemplate::New(init_pdb_file_structure));
}

}  // namespace gems
