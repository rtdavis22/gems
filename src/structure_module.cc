// Copyright (c) 2012 The University of Georgia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Author: Robert Davis

#include "structure_module.h"

#include <string>

#include "gmml/gmml.h"

using std::string;

using gmml::SolvatedStructure;
using gmml::Structure;

using namespace v8;

namespace gems {
namespace {

Handle<Value> init(const Arguments& args) {
    HandleScope scope;
    Local<Object> obj = args.This();
    Structure *structure = new Structure;
    obj->SetHiddenValue(String::New("pointer"), External::New(structure));
    return scope.Close(Undefined());
}

Handle<Value> init_solvated_structure(const Arguments& args) {
    HandleScope scope;
    Local<Object> obj = args.This();
    Local<Object> options = args[0]->ToObject();

    Structure *solute =
            StructureModule::extract_ptr(options->Get(String::New("solute")));
    Structure *solvent =
            StructureModule::extract_ptr(options->Get(String::New("solvent")));
    double buffer = options->Get(String::New("buffer"))->NumberValue();
    double closeness = options->Get(String::New("closeness"))->NumberValue();

    SolvatedStructure *solvated = gmml::solvate(*solute, *solvent,
                                                buffer, closeness);
    obj->SetHiddenValue(String::New("pointer"), External::New(solvated));
    return scope.Close(Undefined());
}

Handle<Value> get_size(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    Structure *structure = StructureModule::extract_ptr(info.This());
    return scope.Close(Integer::New(structure->size()));
}

Handle<Value> get_residue_count(Local<String> property,
                                const AccessorInfo& info) {
    HandleScope scope;
    Structure *structure = StructureModule::extract_ptr(info.This());
    return scope.Close(Integer::New(structure->residue_count()));
}

Handle<Value> attach(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    Structure *rhs = StructureModule::extract_ptr(args[0]);
    structure->attach(rhs);
    return Undefined();
}

Handle<Value> append(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    Structure *rhs = StructureModule::extract_ptr(args[0]);
    structure->append(rhs);
    return Undefined();
}

Handle<Value> get_atom_index(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    int residue_index = args[0]->IntegerValue();
    if (args[1]->IsString()) {
        string atom_name(*String::Utf8Value(args[1]));
        int index = structure->get_atom_index(residue_index, atom_name);
        return Integer::New(index);
    } else {
        int index = structure->get_atom_index(residue_index,
                                              args[1]->IntegerValue());
        return Integer::New(index);
    }
}


Handle<Value> get_bonded_atoms(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    int index = args[0]->IntegerValue();
    const Structure::AdjList& adj_atoms = structure->bonds(index);

    Local<Array> array = Array::New(adj_atoms.size());
    for (int i = 0; i < adj_atoms.size(); i++) {
        array->Set(i, Number::New(adj_atoms[i]));
    }

    return array;
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

Handle<Value> remove_residue(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    structure->remove_residue(args[0]->IntegerValue());
    return Undefined();
}

Handle<Value> set_dihedral(const Arguments& args) {
    Structure *structure = StructureModule::extract_ptr(args.This());
    structure->set_dihedral(args[0]->IntegerValue(),
                            *String::Utf8Value(args[1]),
                            args[2]->IntegerValue(),
                            *String::Utf8Value(args[3]),
                            args[4]->IntegerValue(),
                            *String::Utf8Value(args[5]),
                            args[6]->IntegerValue(),
                            *String::Utf8Value(args[7]),
                            args[8]->NumberValue());
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

Handle<Value> init_from_string(const Arguments& args) {
    HandleScope scope;
    Local<v8::Object> obj = args.This();
    try {
        Structure *structure = gmml::build(*String::Utf8Value(args[0]));
        obj->SetHiddenValue(String::New("pointer"), External::New(structure));
    } catch(const std::exception& e) {
        return scope.Close(ThrowException(String::New(e.what())));
    }
    return scope.Close(Undefined());
}


Handle<Value> get_structure_prototype(const Arguments& args) {
    HandleScope scope;

    Local<Object> object = Object::New();

    object->Set(String::New("init"),
                FunctionTemplate::New(init)->GetFunction());

    object->Set(String::New("_attach"),
                FunctionTemplate::New(attach)->GetFunction());

    object->Set(String::New("append"),
                FunctionTemplate::New(append)->GetFunction());

    object->Set(String::New("getAtomIndex"),
                FunctionTemplate::New(get_atom_index)->GetFunction());

    object->Set(String::New("_getBondedAtoms"),
                FunctionTemplate::New(get_bonded_atoms)->GetFunction());

    object->Set(String::New("printPdbFile"),
                FunctionTemplate::New(print_pdb_file)->GetFunction());

    object->Set(String::New("printRestartFile"),
                FunctionTemplate::New(print_restart_file)->GetFunction());

    object->Set(String::New("printTopologyFile"),
                FunctionTemplate::New(print_topology_file)->GetFunction());

    object->Set(String::New("removeResidue"),
                FunctionTemplate::New(remove_residue)->GetFunction());

    object->Set(String::New("_setDihedral"),
                FunctionTemplate::New(set_dihedral)->GetFunction());

    object->Set(String::New("_initSolvatedStructure"),
                FunctionTemplate::New(init_solvated_structure)->GetFunction());

    object->Set(String::New("setHead"),
                FunctionTemplate::New(set_head)->GetFunction());

    object->Set(String::New("setTail"),
                FunctionTemplate::New(set_tail)->GetFunction());

    object->SetAccessor(String::New("residueCount"), get_residue_count);

    object->SetAccessor(String::New("size"), get_size);

    object->Set(String::New("initFromString"),
                FunctionTemplate::New(init_from_string)->GetFunction());

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
