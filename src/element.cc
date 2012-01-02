#include "element.h"

#include "gmml/gmml.h"

using std::string;

string ElementMap::get(gmml::Element element) {
    if (element == gmml::kElementH)
        return "H";
    else if (element == gmml::kElementC)
        return "C";
    else if (element == gmml::kElementO)
        return "O";
    return "";
}
