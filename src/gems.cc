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

#include "gems.h"

#include <cstdlib>

#include <fstream>
#include <stdexcept>

#include "gmml/gmml.h"

#include "atom_module.h"
#include "environment_module.h"
#include "glycam_module.h"
#include "pdb_module.h"
#include "prep_file_module.h"
#include "residue_module.h"
#include "shell.h"
#include "structure_module.h"

using std::string;

using namespace v8;

namespace gems {
namespace {

bool file_exists(const std::string& file_name) {
    std::ifstream stream(file_name.c_str());
    return stream.good();
}

string get_data_directory() {
    char *gems_home = getenv("GEMS_HOME");
    if (gems_home != NULL) {
        string data_directory(gems_home);
        if (data_directory[data_directory.size() - 1] == '/')
            data_directory.erase(data_directory.end() - 1);
        return data_directory;
    }

    string public_data_dir = "/usr/local/share/gems";
    if (file_exists(public_data_dir)) {

    } else {
        throw std::runtime_error("Could not find data directory.");
    }

    return public_data_dir;
}

Handle<Value> get_data_path(Local<String> property, const AccessorInfo& info) {
    return String::New(get_data_directory().c_str());
}

}  // namespace

bool Gems::kIsGlycamLoaded = true;

void Gems::init(Handle<ObjectTemplate> global) {
    StructureModule::init(global);
    ResidueModule::init(global);
    AtomModule::init(global);
    PdbModule::init(global);
    PrepFileModule::init(global);
    EnvironmentModule::init(global);
    GlycamModule::init(global);

    global->SetAccessor(String::New("_data_path"), get_data_path);
    global->SetAccessor(String::New("_isGlycamLoaded"), is_glycam_loaded);
}

void Gems::load_files() {
    string data_directory = get_data_directory();
    string bootstrap_script = data_directory + "/src/js/core.js";
    ExecFile(bootstrap_script.c_str());
}

}  // namespace gems
