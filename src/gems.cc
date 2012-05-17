// Author: Robert Davis

#include "gems.h"

#include <fstream>
#include <stdexcept>

#include "gmml/gmml.h"

#include "atom.h"
#include "environment.h"
#include "glycam.h"
#include "pdb.h"
#include "shell.h"
#include "structure.h"

using std::string;

using namespace v8;

namespace gems {
namespace {

bool file_exists(const std::string& file_name) {
    std::ifstream stream(file_name.c_str());
    return stream.good();
}

string get_data_directory() {
    if (file_exists("/usr/local/share/gems")) {

    } else {
        throw std::runtime_error("Could not find data directory.");
    }

    return "/usr/local/share/gems";
}

Handle<Value> get_data_path(Local<String> property, const AccessorInfo& info) {
    return String::New(get_data_directory().c_str());
}

}  // namespace

void Gems::init(Handle<ObjectTemplate> global) {
    StructureModule::init(global);
    AtomModule::init(global);
    PdbModule::init(global);
    EnvironmentModule::init(global);

    GlycamModule::init(global);

    global->SetAccessor(String::New("_data_path"), get_data_path);
}

void Gems::load_files() {
    string data_directory = get_data_directory();
    string bootstrap_script = data_directory + "/src/js/core.js";
    ExecFile(bootstrap_script.c_str());
}

}  // namespace gems
