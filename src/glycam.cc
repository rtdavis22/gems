// Author: Robert Davis

#include "glycam.h"

#include "gmml/gmml.h"

using gmml::Structure;

using namespace v8;

namespace gems {
namespace {

Handle<Value> set_from_glycam_sequence(const Arguments& args) {
    HandleScope scope;
    Local<v8::Object> obj = args.This();
    Structure *structure = gmml::glycam_build(*String::Utf8Value(args[0]));
    obj->SetHiddenValue(String::New("pointer"), External::New(structure));
    return scope.Close(Undefined());    
}

}  // namespace

void GlycamModule::init(Handle<ObjectTemplate> global) {
    global->Set(String::New("_setFromGlycamSequence"),
                FunctionTemplate::New(set_from_glycam_sequence));
}

}  // namespace gems
