// Author: Robert Davis`

#include "residue.h"

#include "gmml/gmml.h"

namespace gems {

gmml::Residue *GemsResidue::residue() {
    if (index < 0 || index >= structure->residue_count())
        return NULL;
    return structure->residues(index);
}

namespace {

// This class builds the function template.
class ResidueTemplate {
  public:
    ResidueTemplate() { Init(); }

    v8::Persistent<v8::FunctionTemplate> get_template() const {
        return template_;
    }

    ~ResidueTemplate() {
        template_.Dispose();
        template_.Clear();
    }

    static v8::Handle<v8::Value> GetName(v8::Local<v8::String> property,
                                         const v8::AccessorInfo& info);
    static void SetName(v8::Local<v8::String> property,
                        v8::Local<v8::Value> value,
                        const v8::AccessorInfo& info);

  private:
    void Init();

    v8::Persistent<v8::FunctionTemplate> template_;
};

}  // namespace

v8::Handle<v8::Object> ResidueWrapper::wrap(GemsResidue *residue) {
    v8::HandleScope handle_scope;

    static ResidueTemplate residue_template;
    v8::Persistent<v8::FunctionTemplate> template_ =
            residue_template.get_template();

    v8::Persistent<v8::Object> instance = v8::Persistent<v8::Object>::New(
            template_->InstanceTemplate()->NewInstance());

    instance.MakeWeak(residue, callback);

    instance->SetInternalField(0, v8::External::New(residue));

    return handle_scope.Close(instance);
}

void ResidueWrapper::callback(v8::Persistent<v8::Value> object, void *data) {
    object.Dispose();
    object.Clear();
}








void ResidueTemplate::Init() {
    v8::HandleScope handle_scope;

    v8::Handle<v8::FunctionTemplate> local_template =
            v8::FunctionTemplate::New();
    v8::Handle<v8::ObjectTemplate> instance_template =
            local_template->InstanceTemplate();
    instance_template->SetInternalFieldCount(1);

    instance_template->SetAccessor(v8::String::New("name"),
                                   GetName, SetName);

    template_ = v8::Persistent<v8::FunctionTemplate>::New(local_template);
}

v8::Handle<v8::Value> ResidueTemplate::GetName(v8::Local<v8::String> property,
                                               const v8::AccessorInfo& info) {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap =
            v8::Local<v8::External>::Cast(self->GetInternalField(0));
    GemsResidue *residue = static_cast<GemsResidue*>(wrap->Value());
    return v8::String::New(residue->residue()->name().c_str());
}

void ResidueTemplate::SetName(v8::Local<v8::String> property,
                              v8::Local<v8::Value> value,
                              const v8::AccessorInfo& info) {
    if (!value->IsString()) {
        v8::ThrowException(v8::String::New("Value is not a string"));
        return;
    }

    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap =
            v8::Local<v8::External>::Cast(self->GetInternalField(0));
    GemsResidue *residue = static_cast<GemsResidue*>(wrap->Value());
    residue->residue()->set_name(*v8::String::Utf8Value(value));
}

}  // namespace gems
