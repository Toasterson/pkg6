//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "PackageInfo.h"
#include <algorithm>
#include <action/DirectoryAction.h>

using namespace rapidjson;

void pkg::PackageInfo::addAction(const std::string &action_string) {
    if(boost::algorithm::starts_with(action_string, "set")){
        attrs.push_back(AttributeAction(action_string));
    } else if(boost::algorithm::starts_with(action_string, "depend")){
        dependencies.push_back(DependAction(action_string));
    } else if(boost::algorithm::starts_with(action_string, "dir")){
        dirs.push_back(DirectoryAction(action_string));
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
        this->categories.insert(this->categories.end(), std::make_move_iterator(alternate.categories.begin()), std::make_move_iterator(alternate.categories.end()));
    }

    if(!alternate.attrs.empty()){
        this->attrs.insert(this->attrs.end(), std::make_move_iterator(alternate.attrs.begin()), std::make_move_iterator(alternate.attrs.end()));
    }

    if(!alternate.links.empty()){
        this->links.insert(this->links.end(), std::make_move_iterator(alternate.links.begin()), std::make_move_iterator(alternate.links.end()));
    }

    if(!alternate.files.empty()){
        this->files.insert(this->files.end(), std::make_move_iterator(alternate.files.begin()), std::make_move_iterator(alternate.files.end()));
    }

    if(!alternate.dirs.empty()){
        this->dirs.insert(this->dirs.end(), std::make_move_iterator(alternate.dirs.begin()), std::make_move_iterator(alternate.dirs.end()));
    }

    if(!alternate.dependencies.empty()){
        this->dependencies.insert(this->dependencies.end(), std::make_move_iterator(alternate.dependencies.begin()), std::make_move_iterator(alternate.dependencies.end()));
    }

    if(!alternate.signature.empty()){
        this->signature = alternate.signature;
    }

    return *this;
}





