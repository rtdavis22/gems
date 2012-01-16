// Author: Robert Davis`

#include "atom.h"
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

    static v8::Handle<v8::Value> GetIndex(v8::Local<v8::String> property,
                                          const v8::AccessorInfo& info);

    static v8::Handle<v8::Value> GetName(v8::Local<v8::String> property,
                                         const v8::AccessorInfo& info);
    static void SetName(v8::Local<v8::String> property,
                        v8::Local<v8::Value> value,
                        const v8::AccessorInfo& info);

    static v8::Handle<v8::Value> Atoms(const v8::Arguments& args);

    static v8::Handle<v8::Value> GetSize(v8::Local<v8::String> property,
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

    instance_template->SetAccessor(v8::String::New("index"), GetIndex);

    instance_template->SetAccessor(v8::String::New("name"),
                                   GetName, SetName);

    instance_template->SetAccessor(v8::String::New("size"), GetSize);

    template_ = v8::Persistent<v8::FunctionTemplate>::New(local_template);

    NODE_SET_PROTOTYPE_METHOD(template_, "atoms", Atoms);
}

v8::Handle<v8::Value> ResidueTemplate::GetSize(v8::Local<v8::String> property,
                                               const v8::AccessorInfo& info) {
    v8::HandleScope scope;

    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap =
            v8::Local<v8::External>::Cast(self->GetInternalField(0));
    GemsResidue *residue = static_cast<GemsResidue*>(wrap->Value());
    return scope.Close(v8::Integer::New(residue->residue()->size()));
}


v8::Handle<v8::Value> ResidueTemplate::Atoms(const v8::Arguments& args) {
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::External> wrap =
            v8::Local<v8::External>::Cast(self->GetInternalField(0));
    GemsResidue *gems_residue = static_cast<GemsResidue*>(wrap->Value());
    gmml::Residue *residue = gems_residue->residue();

    int index = -1;
    if (args[0]->IsString()) {
        v8::Local<v8::Value> atom_name_v8 = args[0]->ToString();
        std::string atom_name(*v8::String::Utf8Value(atom_name_v8));
        index = residue->get_index(atom_name);
    } else if (args[0]->IsNumber()) {
        index = args[0]->ToNumber()->Value();
    }

    if (index < 0 || index >= residue->size()) {
        return v8::ThrowException(v8::String::New("Invalid argument"));
    }

    int structure_index =
            gems_residue->structure->get_atom_index(gems_residue->index,
                                                      index);

    return AtomWrapper::wrap(new GemsAtom(gems_residue->structure,
                                          structure_index));
}

v8::Handle<v8::Value> ResidueTemplate::GetIndex(v8::Local<v8::String> property,
                                                const v8::AccessorInfo& info) {
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap =
            v8::Local<v8::External>::Cast(self->GetInternalField(0));
    GemsResidue *residue = static_cast<GemsResidue*>(wrap->Value());
    return v8::Integer::New(residue->index);
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
