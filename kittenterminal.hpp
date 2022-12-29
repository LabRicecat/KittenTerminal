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
    // The lexer used, can be modified
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
        kittenability* com = get_skill(command);
        if(!com) return worried_kitten::UNKNOWN_COMMAND;

        line.erase(line.begin());
        kittens_howto todo = com->knowledge.parse(line);
        if(!todo && todo != ArgParserErrors::NO_ARGS) {
            current_error = todo.error();
            return worried_kitten::INVALID_ARGUMENTS;
        }

        current_error = com->perform(todo);
        if(current_error == "") return worried_kitten::OK;
        return worried_kitten::COMMAND_ERROR; 
    }

    // empty if no error occured
    std::string current_error = "";
};

struct MeowingPromt {
    struct Memory {
        std::string user_input;
        std::map<std::string,std::string*> var_register;
        bool repeat_cycle = false;
        bool exit = false;
    }memory;

    struct PlanEntry {
        void(*proc)(MeowingPromt*) = nullptr;

        PlanEntry(void(*p)(MeowingPromt*)) : proc(p) {}
        void operator=(void(*p)(MeowingPromt*)) { proc = p; }
    };

    std::vector<PlanEntry> working_plan;
    KittenWorker worker_cat;
    std::string promt_layout;

    void(*panic)(worried_kitten,std::string) = nullptr;

    MeowingPromt& set_look(std::string look) { promt_layout = look; return *this; }
    MeowingPromt& plan_work(std::vector<PlanEntry> plan) { working_plan = plan; return *this; }
    MeowingPromt& learn_value(std::string name, std::string* var) { memory.var_register[name] = var; return *this; }
    MeowingPromt& on_panic(void(*on_panic)(worried_kitten,std::string)) { panic = on_panic; return *this; }
    MeowingPromt& employ_worker(KittenWorker worker) { worker_cat = worker; return *this; }

    std::string look2string() {
        std::string ret;
        std::string tmp;
        bool in_br = false;
        for(auto i : promt_layout) {
            if(i == '{') {
                if(tmp != "") ret += "{" + tmp;
                in_br = true;
            }
            else if(i == '}') {
                if(memory.var_register.count(tmp) == 0) ret += "{" + tmp + "}";
                else ret += *memory.var_register[tmp];
                in_br = false;
                tmp = "";
            }
            else if(in_br) tmp += i;
            else ret += i;
        }
        if(tmp != "") ret += "{" + tmp;
        return ret;
    }

    void workdown_plan() {
        for(auto i : working_plan) i.proc(this);
    }

    void do_your_thing() {
        do { workdown_plan(); } while(memory.repeat_cycle && !memory.exit);
    }
};

const MeowingPromt::PlanEntry Cat_PrintLayout     = MeowingPromt::PlanEntry([](MeowingPromt* p){ std::cout << p->look2string(); });
const MeowingPromt::PlanEntry Cat_GetUserInput    = MeowingPromt::PlanEntry([](MeowingPromt* p){ std::string i; std::getline(std::cin,i); p->memory.user_input = i; });
const MeowingPromt::PlanEntry Cat_Repeat          = MeowingPromt::PlanEntry([](MeowingPromt* p){ p->memory.repeat_cycle = true; });
const MeowingPromt::PlanEntry Cat_ParseLine       = MeowingPromt::PlanEntry([](MeowingPromt* p){ 
    std::cout << "uinp: " << p->memory.user_input << "\n";
    auto err = p->worker_cat.playwith(p->memory.user_input); 
    if(err != worried_kitten::OK && p->panic) p->panic(err,p->worker_cat.current_error); 
});

#define Cat_CustomFunction(f) MeowingPromt::PlanEntry(f)

#endif