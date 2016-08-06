//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "PackageInfo.h"
#include <action/Action.h>
#include <algorithm>

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
        for(int state : alternate.states){
            if(std::find(states.begin(), states.end(), state) == states.end()) {
                states.push_back(state);
            }
        }
    }

    if(!alternate.categories.empty()){
        this->categories.insert(std::end(this->categories), std::begin(alternate.categories), std::end(alternate.categories));
    }

    if(!alternate.attrs.empty()){
        this->attrs.insert(std::end(this->attrs), std::begin(alternate.attrs), std::end(alternate.attrs));
    }

    if(!alternate.links.empty()){
        this->links.insert(std::end(this->links), std::begin(alternate.links), std::end(alternate.links));
    }

    if(!alternate.files.empty()){
        this->files.insert(std::end(this->files), std::begin(alternate.files), std::end(alternate.files));
    }

    if(!alternate.dirs.empty()){
        this->dirs.insert(std::end(this->dirs), std::begin(alternate.dirs), std::end(alternate.dirs));
    }

    if(!alternate.dependencies.empty()){
        this->dependencies.insert(std::end(this->dependencies), std::begin(alternate.dependencies), std::end(alternate.dependencies));
    }

    if(!alternate.signature.empty()){
        this->signature = alternate.signature;
    }

    return *this;
}





