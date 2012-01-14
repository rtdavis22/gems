// Author: Robert Davis

#include "atom.h"

#include <iostream>

#include "gmml/gmml.h"

#include "element.h"

using gmml::Atom;
using gmml::Structure;

using v8::AccessorInfo;
using v8::Arguments;
using v8::Array;
using v8::External;
using v8::FunctionTemplate;
using v8::Handle;
using v8::HandleScope;
using v8::Local;
using v8::Number;
// Don't try to but v8::Object here. It conflicts with gems::Object.
using v8::ObjectTemplate;
using v8::Persistent;
using v8::String;
using v8::ThrowException;
using v8::Undefined;
using v8::Value;

namespace gems {

gmml::Atom *GemsAtom::atom() {
    if (index < 0 || index >= structure->size())
        return NULL;
    return structure->atoms(index);
}

namespace {

// This class builds the function template.
class AtomTemplate {
  public:
    AtomTemplate() { Init(); }

    v8::Persistent<v8::FunctionTemplate> get_template() const {
        return template_;
    }

    ~AtomTemplate() {
        template_.Dispose();
        template_.Clear();
    }

    static Handle<Value> GetCharge(Local<String> property,
                                   const AccessorInfo& info);
    static void SetCharge(Local<String> property, Local<Value> value,
                          const AccessorInfo& accessor_info);

    static Handle<Value> GetIndex(Local<String> property,
                                  const AccessorInfo& info);

    static Handle<Value> GetName(Local<String> property,
                                 const AccessorInfo& info);
    static void SetName(Local<String> property, Local<Value> value,
                        const AccessorInfo& info);

    static Handle<Value> GetCoordinate(Local<String> property,
                                       const AccessorInfo& info);
    static void SetCoordinate(Local<String> property, Local<Value> value,
                              const AccessorInfo& info);

    static Handle<Value> GetElement(Local<String> property,
                                    const AccessorInfo& info);
    static void SetElement(Local<String> property, Local<Value> value,
                           const AccessorInfo& info);

    static Handle<Value> GetType(Local<String> property,
                                 const AccessorInfo& info);
    static void SetType(Local<String> property, Local<Value> value,
                        const AccessorInfo& info);
  private:
    void Init();

    v8::Persistent<v8::FunctionTemplate> template_;
};

}  // namespace

Handle<v8::Object> AtomWrapper::wrap(GemsAtom *atom) {
    HandleScope handle_scope;

    static AtomTemplate atom_template;
    Persistent<FunctionTemplate> template_ = atom_template.get_template();

    Persistent<v8::Object> instance = Persistent<v8::Object>::New(
        template_->InstanceTemplate()->NewInstance());

    //Structure::AtomPtr *wrappable_atom = new Structure::AtomPtr(atom);

    instance.MakeWeak(atom, callback);

    instance->SetInternalField(0, External::New(atom));

    return handle_scope.Close(instance);
}

void AtomWrapper::callback(Persistent<Value> object, void *data) {
    //Structure::AtomPtr *atom = static_cast<Structure::AtomPtr*>(data);
    //delete atom;
    object.Dispose();
    object.Clear();
}

Handle<Value> AtomTemplate::GetCharge(Local<String> property,
                                      const AccessorInfo& info) {
    HandleScope scope;

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());

    return scope.Close(Number::New(atom->atom()->charge()));

}

void AtomTemplate::SetCharge(Local<String> property, Local<Value> value,
                             const AccessorInfo& info) {
    if (!value->IsNumber()) {
        ThrowException(String::New("Value is not a number"));
        return;
    }

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());
    atom->atom()->set_charge(value->NumberValue());
}

Handle<Value> AtomTemplate::GetIndex(Local<String> property,
                                     const AccessorInfo& info) {
    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());
    return v8::Integer::New(atom->index);
}

Handle<Value> AtomTemplate::GetName(Local<String> property,
                                    const AccessorInfo& info) {
    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());
    return String::New(atom->atom()->name().c_str());
}

void AtomTemplate::SetName(Local<String> property, Local<Value> value,
                           const AccessorInfo& info) {
    if (!value->IsString()) {
        ThrowException(String::New("Value is not a string"));
        return;
    }

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());
    atom->atom()->set_name(*String::Utf8Value(value));
}

Handle<Value> AtomTemplate::GetCoordinate(Local<String> property,
                                          const AccessorInfo& info) {
    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());
    Local<Array> array = Array::New(3);
    array->Set(0, Number::New(atom->atom()->coordinate().x));
    array->Set(1, Number::New(atom->atom()->coordinate().y));
    array->Set(2, Number::New(atom->atom()->coordinate().z));
    return array;
}

void AtomTemplate::SetCoordinate(Local<String> property, Local<Value> value,
                                 const AccessorInfo& info) {
    if (!value->IsArray()) {
        ThrowException(String::New("Parameter is not an array"));
        return;
    }

    Local<Array> array = Array::Cast(*value);

    if (array->Length() != 3) {
        ThrowException(String::New("Array must have size 3"));
        return;
    }

    if (!array->Get(0)->IsNumber() || !array->Get(1)->IsNumber() ||
            !array->Get(2)->IsNumber()) {
        ThrowException(String::New("Coordinate values must be numeric."));
        return;
    }

    double x = array->Get(0)->ToNumber()->Value();
    double y = array->Get(1)->ToNumber()->Value();
    double z = array->Get(2)->ToNumber()->Value();

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());

    atom->atom()->set_coordinate(x, y, z);
}

Handle<Value> AtomTemplate::GetElement(Local<String> property,
                                       const AccessorInfo& info) {
    HandleScope scope;

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());

    Local<v8::Object> global(v8::Context::GetCurrent()->Global());
    Local<v8::Object> obj = global->Get(String::New("Element"))->ToObject();

    gmml::Element element = atom->atom()->element();

    return scope.Close(obj->Get(String::New(ElementMap::get(element).c_str())));

}

void AtomTemplate::SetElement(Local<String> property, Local<Value> value,
                              const AccessorInfo& info) {

}

Handle<Value> AtomTemplate::GetType(Local<String> property,
                                    const AccessorInfo& info) {
    HandleScope scope;

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());

    return scope.Close(String::New(atom->atom()->type().c_str()));

}

void AtomTemplate::SetType(Local<String> property, Local<Value> value,
                           const AccessorInfo& info) {
    if (!value->IsString()) {
        ThrowException(String::New("Value is not a string"));
        return;
    }

    Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    GemsAtom *atom = static_cast<GemsAtom*>(wrap->Value());
    atom->atom()->set_type(*String::Utf8Value(value));
}

void AtomTemplate::Init() {
    HandleScope handle_scope;

    Handle<FunctionTemplate> local_template = FunctionTemplate::New();
    Handle<ObjectTemplate> instance_template =
        local_template->InstanceTemplate();
    instance_template->SetInternalFieldCount(1);

    instance_template->SetAccessor(String::New("charge"),
                                   GetCharge, SetCharge);

    instance_template->SetAccessor(String::New("index"), GetIndex);

    instance_template->SetAccessor(String::New("name"),
                                   GetName, SetName);

    instance_template->SetAccessor(String::New("coordinate"),
                                   GetCoordinate, SetCoordinate);

    instance_template->SetAccessor(String::New("element"),
                                   GetElement, SetElement);

    instance_template->SetAccessor(String::New("type"),
                                   GetType, SetType);

    template_ = Persistent<FunctionTemplate>::New(local_template);
}

}  // namespace gems
