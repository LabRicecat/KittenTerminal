#ifndef TERMINAL_KITTEN
#define TERMINAL_KITTEN

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "catmods/argparser/ArgParser.h"

#define KITTENTERMINAL_VERSION "1.0.0"

using catcommands = std::vector<std::string>;

catcommands workout_line(std::string confusingline);

using kittenknowledge = ArgParser;
using kittens_howto = ParsedArgs;
struct kittenability {
    kittenknowledge knowledge;
    std::string(*perform)(kittens_howto howto);
};
using kittenskills = std::map<std::string,kittenability>;

struct KittenWorker {
    kittenskills skills;

    inline bool has_skill(std::string name) { return skills.count(name) >= 1; }
    KittenWorker& learn_skill(std::string name, kittenability ability);
    kittenability& get_skill(std::string name);

    std::string playwith(catcommands line);
    std::string playwith(std::string str);
};

#endif