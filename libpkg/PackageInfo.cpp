//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "PackageInfo.h"
#include <Action.h>

void pkg::PackageInfo::addAction(const std::string &action_string) {
    Action action = Action(action_string);
    if(action.type == "set"){
        attrs.push_back(action);
    } else if(action.type == "depend"){
        dependencies.push_back(action);
    }
}



