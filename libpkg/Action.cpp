//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <sstream>
#include "Action.h"

void pkg::Action::parseActionString(const std::string &action_string) {
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(action_string, boost::escaped_list_separator<char>("\\", " ", "\""));
    std::string tmpkey;
    auto iter = std::begin(tokens);
    for(std::string token: tokens){
        if(iter == std::begin(tokens)){
            type = token;
        } else {
            if(boost::contains(token, "=")){
                std::vector<std::string> tmpCont;
                boost::split(tmpCont, token, boost::is_any_of("="));
                tmpkey = tmpCont.front();
                data[tmpkey] = tmpCont.back();
            } else if (!tmpkey.empty()){
                data[tmpkey] = data[tmpkey] + " " + token;
            }
        }
        iter++;
    }
}

std::string pkg::Action::toActionString() {
    std::ostringstream stringStream;
    stringStream << type;
    for(auto pair : data){
        stringStream << " " << pair.first << "=";
        if(boost::contains(pair.second, " ")){
            stringStream << "\"" << pair.second << "\"";
        } else {
            stringStream << pair.second;
        }
    }
    return stringStream.str();
}













