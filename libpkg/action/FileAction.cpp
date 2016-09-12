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
            strstr << token.substr(0, token.find(" "));
            name = strstr.str();
            token.erase(0, token.find(" ")+1);
        }
        if(token != action_type){
            if(boost::contains(token, "csize=")) {
                token.erase(0, token.find("=")+1);
                csize = stoi(token);
            } else if(boost::contains(token, "size=")) {
                //TODO Check if the attribute is now named pkg.size instead of size or whats the difference there. Same for csize.
                token.erase(0, token.find("=")+1);
                size = stoi(token);
            } else if(boost::contains(token, "path=")) {
                token.erase(0, token.find("=")+1);
                path = token;
            }  else if (boost::contains(token, "=")){
                std::vector<std::string> tmp;
                boost::split(tmp, token, boost::is_any_of("="));
                attributes.insert(std::pair<std::string,std::string>(tmp.front(), tmp.back()));
            }
        }
    }
    if(attributes.count("pkg.size")){
        size = stoi(attributes["pkg.size"]);
        attributes.erase("pkg.size");
    }

    if(attributes.count("pkg.csize")){
        size = stoi(attributes["pkg.csize"]);
        attributes.erase("pkg.csize");
    }

    if(name.empty()){
        throw exception::InvalidActionException("No filename");
    }
}

std::string pkg::action::FileAction::toActionString() {
    stringstream strstr;
    strstr << action_type << " " << name << " path=" << path << " pkg.size=" << to_string(size) << " pkg.csize=" << to_string(csize);
    for(auto attr : attributes){
        strstr << " " << attr.first << "=" << attr.second;
    }
    return strstr.str();
}

void pkg::action::FileAction::install() {

}

bool pkg::action::FileAction::validate() {
    return false;
}
