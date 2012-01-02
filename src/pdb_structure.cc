// Author: Robert Davis

#include "pdb_structure.h"

#include <exception>

#include "gmml/gmml.h"

#include "atom.h"
#include "structure.h"

using std::map;

using gmml::Atom;
using gmml::PdbFileStructure;

using v8::AccessorInfo;
using v8::Arguments;
using v8::Array;
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

Handle<Object> PdbStructureWrapper::wrap(PdbFileStructure *structure) {
    HandleScope handle_scope;

    static PdbStructureTemplate structure_template;
    Persistent<FunctionTemplate> template_ = structure_template.get_template();

    Persistent<Object> instance = Persistent<Object>::New(
        template_->InstanceTemplate()->NewInstance());

    instance.MakeWeak(structure, callback);

    instance->SetInternalField(0, External::New(structure));

    return handle_scope.Close(instance);
}

void PdbStructureWrapper::callback(Persistent<Value> object, void *data) {
    object.Dispose();
    object.Clear();
}

struct PdbStructureTemplate::Impl {
    static Handle<Value> GetPdbThing(Local<String> property,
                                     const AccessorInfo& info);
};

Handle<Value> PdbStructureTemplate::Impl::GetPdbThing(
        Local<String> property,
        const AccessorInfo& info) {
    return v8::Integer::New(123123);
}

void PdbStructureTemplate::Init() {
    HandleScope scope;

    Handle<FunctionTemplate> local_template = FunctionTemplate::New();
    Handle<ObjectTemplate> instance_template =
        local_template->InstanceTemplate();
    instance_template->SetInternalFieldCount(1);

    instance_template->SetAccessor(String::New("get_pdb_thing"),
                                   Impl::GetPdbThing);

    StructureTemplate structure_template;
    local_template->Inherit(structure_template.get_template());

    template_ = Persistent<FunctionTemplate>::New(local_template);
}

Handle<Value> load_pdb(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString())
        return ThrowException(String::New("Invalid argument"));

    String::Utf8Value file(args[0]);

    gmml::PdbFileStructure *structure = NULL;

    try {
        structure = PdbFileStructure::build(*file);
    } catch(const std::exception& e) {
        return ThrowException(v8::String::New(e.what()));
    }

    return scope.Close(PdbStructureWrapper::wrap(structure));
}

}
