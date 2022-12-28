#ifndef TERMINAL_KITTEN
#define TERMINAL_KITTEN

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "catmods/argparser/ArgParser.h"
#include "catmods/kittenlexer/kittenlexer.hpp"

#define KITTENTERMINAL_VERSION "1.0.0"

using catcommands = std::vector<std::string>;

enum class worried_kitten {
    OK,
    UNKNOWN_COMMAND,
    INVALID_ARGUMENTS,
    COMMAND_ERROR,
};

using kittenknowledge = ArgParser;
using kittens_howto = ParsedArgs;
struct kittenability {
    kittenknowledge knowledge;
    std::string(*perform)(kittens_howto howto);
};
using kittenskills = std::map<std::string,kittenability>;

struct KittenWorker {
    KittenLexer lexer;
    kittenskills skills;

    KittenWorker() {
        lexer = KittenLexer()
            .add_stringq('"')
            .add_stringq('\'')
            .erase_empty()
            .add_ignore(' ')
            .add_ignore('\t')
            .add_ignore('\n')
            .add_backslashopt('t','\t')
            .add_backslashopt('n','\n')
            .add_backslashopt('r','\r')
            ;
    }

    inline bool has_skill(std::string name) { return skills.count(name) >= 1; }
    KittenWorker& learn_skill(std::string name, kittenability ability) {
        skills[name] = ability;
        return *this;
    }
    kittenability* get_skill(std::string name) {
        if(has_skill(name)) return &skills[name];
        return nullptr;
    }

    catcommands workout_line(std::string confusingline) {
        catcommands ret;
        auto lexed = lexer.lex(confusingline);
        for(auto i : lexed) ret.push_back(i.src);
        return ret;
    }

    worried_kitten playwith(std::string str) {
        return playwith(workout_line(str));
    }
    worried_kitten playwith(catcommands line) {
        this->current_error = "";
        if(line.empty()) return worried_kitten::OK;

        std::string command = line[0];
        kittenability com = get_skill(command);
        if(!com) return worried_kitten::UNKNOWN_COMMAND;

        line.erase(line.begin());
        kittens_howto todo = com.knowledge.parse(line);
        if(!todo && todo != ArgParserErrors::NO_ARGS) {
            current_error = todo.error();
            return worried_kitten::INVALID_ARGUMENTS;
        }

        current_error = com.perform(todo);
        if(current_error == "") return worried_kitten::OK;
        return worried_kitten::COMMAND_ERROR; 
    }

    // empty if no error occured
    std::string current_error = "";
};

#endif