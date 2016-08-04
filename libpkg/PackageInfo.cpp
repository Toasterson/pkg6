//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "PackageInfo.h"
#include <Action.h>

using namespace rapidjson;

void pkg::PackageInfo::addAction(const std::string &action_string) {
    Action action = Action(action_string);
    addAction(action);
}

void pkg::PackageInfo::addAction(const Action &action) {
    if(action.type == "set"){
        attrs.push_back(action);
    } else if(action.type == "depend"){
        dependencies.push_back(action);
    }
}

pkg::PackageInfo pkg::PackageInfo::operator+=(pkg::PackageInfo &alternate) {
    if(!alternate.states.empty()){
        this->states = alternate.states;
    }

    if(!alternate.categories.empty()){
        this->categories = alternate.categories;
    }

    if(!alternate.attrs.empty()){
        this->attrs = alternate.attrs;
    }

    if(!alternate.links.empty()){
        this->links = alternate.links;
    }

    if(!alternate.files.empty()){
        this->files = alternate.files;
    }

    if(!alternate.dirs.empty()){
        this->dirs = alternate.dirs;
    }

    if(!alternate.dependencies.empty()){
        this->dependencies = alternate.dependencies;
    }

    if(alternate.signature != this->signature){
        this->signature = alternate.signature;
    }

    return *this;
}





