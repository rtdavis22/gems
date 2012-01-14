// Author: Robert Davis

#ifndef GEMS_STRUCTURE_H_
#define GEMS_STRUCTURE_H_

#include <map>
#include <string>

#include "v8.h"

#include "gems.h"

namespace gmml {

class Structure;

}  // namespace gmml

namespace gems {

// remove this
v8::Handle<v8::Value> StructureConstructor(const v8::Arguments& args);

class GemsStructure : public Object {
  public:
    GemsStructure(gmml::Structure *structure) : structure_(structure),
                                                is_parent_tail_set_(false),
                                                parent_tail_name_("") {}

    GemsStructure(gmml::Structure *structure, const std::string& id)
            : structure_(structure), is_parent_tail_set_(false),
              parent_tail_name_("") {
        id_map_[id] = 0;
    }

    // I probably want to delete structure_ here.
    virtual ~GemsStructure() {}

    // Attach the structure to the given atom name in the same residue as
    // the tail atom.
    int attach_from_head(GemsStructure *structure, int tail_residue);
    int attach(GemsStructure *structure, const std::string& tail_name);
    int attach(GemsStructure *structure);

    int append(GemsStructure *structure);

    void set_parent_tail_name(const std::string& name) {
        parent_tail_name_ = name;
        is_parent_tail_set_ = true;
    }

    int map_id(const std::string& id) const;

    virtual gmml::Structure *structure() { return structure_; }
    bool is_parent_tail_set() const { return is_parent_tail_set_; }
    std::string parent_tail_name() const { return parent_tail_name_; }
    const std::map<std::string, int>& id_map() const { return id_map_; }

  private:
    void combine(GemsStructure *structure);

    gmml::Structure *structure_;
    bool is_parent_tail_set_;
    std::string parent_tail_name_;
    std::map<std::string, int> id_map_;
};

class StructureWrapper {
  public:
    // Returns the javascript representation of the structure.
    static v8::Handle<v8::Object> wrap(GemsStructure *structure);

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
