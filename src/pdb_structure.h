// Author: Robert Davis

#ifndef GEMS_PDB_STRUCTURE_H_
#define GEMS_PDB_STRUCTURE_H_

#include "v8.h"

namespace gmml {

class PdbFileStructure;

}

namespace gems {

v8::Handle<v8::Value> load_pdb(const v8::Arguments& args);

class PdbStructureWrapper {
  public:
    // Returns the javascript representation of the structure.
    static v8::Handle<v8::Object> wrap(gmml::PdbFileStructure *structure);

  private:
    // This function is called by the garbage collector when only weak handles
    // refer to the object.
    static void callback(v8::Persistent<v8::Value> object, void *data);
};

// This class builds the FunctionTemplate.
class PdbStructureTemplate {
  public:
    PdbStructureTemplate() { Init(); }

    ~PdbStructureTemplate() {
        template_.Dispose();
        template_.Clear();
    }

    v8::Persistent<v8::FunctionTemplate> get_template() const {
        return template_;
    }

  private:
    struct Impl;

    void Init();

    v8::Persistent<v8::FunctionTemplate> template_;
};

}  // namespace gems

#endif  // GEMS_PDB_STRUCTURE_H_
