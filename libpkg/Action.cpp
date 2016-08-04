//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <sstream>
#include "Action.h"

void pkg::Action::parseActionString(const std::string &action_string) {
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(action_string, boost::escaped_list_separator<char>("\\", " ", "\""));
    std::string tmpkey, tmpval;
    auto iter = std::begin(tokens);
    for(std::string token: tokens){
        if(iter == std::begin(tokens)){
            type = token;
        } else {
            data.push_back(token);
        }
        iter++;
    }
}

std::string pkg::Action::toActionString() {
    std::ostringstream stringStream;
    stringStream << type;
    return stringStream.str();
}













