// Author: Robert Davis

#ifndef GEMS_STRUCTURE_H_
#define GEMS_STRUCTURE_H_

#include "v8.h"

namespace gmml {

class Structure;

}

namespace gems {

v8::Handle<v8::Value> StructureConstructor(const v8::Arguments& args);

class StructureWrapper {
  public:
    // Returns the javascript representation of the structure.
    static v8::Handle<v8::Object> wrap(gmml::Structure *structure);

    // This function frees memory that's allocated lazily by wrap().
    // Since the wrapper will likely be used until the end of the program, it
    // probably isn't necessary to clean up this memory, but this is
    // included for thoroughness. 
    static void destroy();

  private:
    // This function is called by the garbage collector when only weak handles
    // refer to the object.
    static void callback(v8::Persistent<v8::Value> object, void *data);
};

// This class builds the FunctionTemplate.
class StructureTemplate {
  public:
    StructureTemplate() { Init(); }

    ~StructureTemplate() {
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

#endif  // GEMS_STRUCTURE_H_
