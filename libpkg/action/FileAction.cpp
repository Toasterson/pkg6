//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <boost/algorithm/string.hpp>
#include <sstream>
#include "FileAction.h"
#include "ActionException.h"

void pkg::action::FileAction::parseActionString(std::string action_string) {
    for(std::string token: Action::tokenize(action_string)){
        //The first token will contain a SHA-1 hash and the first parameter.
        //Thus we remove the SHA-1 Hash from the token into the name variable.
        if(boost::contains(token, " ")){
            std::stringstream strstr;
            strstr << action_string.substr(0, token.find(" ")+1);
            name = strstr.str();
            token.erase(0, token.find(" ")+1);
        }
        if(token != action_type){
            if(boost::contains(token, "csize=")) {
                token.erase(0, token.find("=")+1);
                csize = stoi(token);
            } else if(boost::contains(token, "size=")) {
                token.erase(0, token.find("=")+1);
                size = stoi(token);
            }  else if (boost::contains(token, "=")){
                std::vector<std::string> tmp;
                boost::split(tmp, token, boost::is_any_of("="));
                attributes.insert(std::pair<std::string,std::string>(tmp.front(), tmp.back()));
                continue;
            }
        }
    }
    if(name.empty()){
        throw exception::InvalidActionException("No filename");
    }
}

std::string pkg::action::FileAction::toActionString() {
    return std::__cxx11::string();
}

void pkg::action::FileAction::install() {

}

bool pkg::action::FileAction::validate() {
    return false;
}
