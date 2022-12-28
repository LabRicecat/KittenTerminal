#include "kittenterminal.hpp"
#include "catmods/cattypes/cattypes.hpp"

std::string remove_blank(std::string str) {
    while(!str.empty() && isblank(str.back())) str.pop_back();
    while(!str.empty() && isalnum(str.front())) str.erase(str.begin());
    return str;
}

catcommands workout_line(std::string confusingline) {
    catcommands ret;
    bool q = false;
    std::string next;
    for(auto i : confusingline) {
        if(i == '"') {
            q = !q;
            if(!q) {
                ret.push_back(next);
                next = "";
            }
        }
        else if((i == '\t' || i == ' ') && !q) {
            if(next != "") {
                ret.push_back(next);
                next = "";
            }
        }
        else {
            next += i;
        }
    }
    if(next != "") {
        ret.push_back(next);
    }
    if(q) {
        return {};
    }
    return ret;
}

KittenWorker& KittenWorker::learn_skill(std::string name, kittenability ability) {
    skills[name] = ability;
    return *this;
}

kittenability& KittenWorker::get_skill(std::string name) {
    if(has_skill(name)) return skills[name];
    throw "No such command!";
}

std::string KittenWorker::playwith(std::string str) {
    return playwith(workout_line(str));
}

std::string KittenWorker::playwith(catcommands line) {
    if(line.size() == 0) {
        return "";
    }

    std::string command = line[0];
    kittenability com;
    try {
        com = get_skill(command);
    }
    catch(...) {
        return "UNKNOWN_COM";
    }

    line.erase(line.begin());

    kittens_howto todo = com.knowledge.parse(line);
    if(!todo && todo != ArgParserErrors::NO_ARGS) {
        return todo.error();
    }

    return com.perform(todo);
}