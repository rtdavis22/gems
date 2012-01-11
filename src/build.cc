#include "build.h"

#include "gmml/gmml.h"

#include "structure.h"

#include <iostream> // RMEOVE
#include <string>

using std::cout; // remove
using std::endl; // remove

namespace gems {

v8::Handle<v8::Value> Build::JSBuild(const v8::Arguments& args) {
    if (args.Length() != 1) {
        return v8::ThrowException(v8::String::New("Function takes 1 argument"));
    }

    GemsStructure *structure = NULL;

    try {
        structure = Build()(args[0]);
    } catch(const std::exception& e) {
        return v8::ThrowException(v8::String::New(e.what()));
    }

    if (structure == NULL)
        return v8::ThrowException(v8::String::New("Invalid argument"));

    return StructureWrapper::wrap(structure);
}

GemsStructure *Build::operator()(v8::Handle<v8::Value> value) {
    if (value->IsArray()) {
        return operator()(v8::Array::Cast(*value));
    } else if (value->IsObject()) {
        return operator()(value->ToObject());
    } else if (value->IsString()) {
        return operator()(value->ToString());
    }
        
    return NULL;
}

GemsStructure *Build::operator()(v8::Handle<v8::String> v8_string) {
    std::string code = *v8::String::Utf8Value(v8_string);
    gmml::Structure *structure = gmml::build(code);
    return new GemsStructure(structure);
}

GemsStructure *Build::operator()(v8::Array *array) {
    GemsStructure *gems_structure = new GemsStructure(new gmml::Structure);
    gems_structure->append(operator()(array->Get(0)));
    gmml::Structure *structure = gems_structure->structure();

    int tail_residue = structure->get_residue_index(structure->tail());

    for (int i = 1; i < array->Length(); i++) {
        int old_tail = structure->tail();
        GemsStructure *substructure = operator()(array->Get(i));
        int ret_val = -1;
        if (substructure->is_parent_tail_set()) {
            ret_val = gems_structure->attach_from_head(substructure,
                                                       tail_residue);
        } else {
            ret_val = gems_structure->attach(substructure);
        }
        if (ret_val == -1)
            return NULL;

        // DO OTHER STUFF

        // We don't want to change the tail when attaching a branch.
        if (array->Get(i)->IsArray()) {
            structure->set_tail(old_tail);
        } else {
            tail_residue = structure->get_residue_index(structure->tail());
        }
    } 

    //return new GemsStructure(structure);
    return gems_structure;
}

GemsStructure *Build::operator()(v8::Handle<v8::Object> object) {
    if (object->InternalFieldCount() > 0) {
        v8::Local<v8::External> wrap =
                v8::Local<v8::External>::Cast(object->GetInternalField(0));
        Object *internal_object = static_cast<Object*>(wrap->Value());
        GemsStructure *gems_structure =
                dynamic_cast<GemsStructure*>(internal_object);
        if (gems_structure == NULL) {
            // The internal object is not a structure. Dont know what to do.
            return NULL;
        }
        return gems_structure;
    }

    if (!object->Has(v8::String::New("code")))
        return NULL;

    std::string code = *v8::String::Utf8Value(
            object->Get(v8::String::New("code")));

    gmml::Structure *structure = gmml::build(code);

 
    GemsStructure *gems_structure = NULL;

    if (object->Has(v8::String::New("id"))) {
        std::string id = *v8::String::Utf8Value(
                object->Get(v8::String::New("id")));
        gems_structure = new GemsStructure(structure, id);
    } else {
        gems_structure = new GemsStructure(structure);
    }

    if (object->Has(v8::String::New("at"))) {
        std::string atom_name = *v8::String::Utf8Value(
                object->Get(v8::String::New("at")));
        gems_structure->set_parent_tail_name(atom_name);
    }


    return gems_structure;
}

}  // namespace gems
