//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "AttributeAction.h"
#include "Exception.h"

void pkg::action::AttributeAction::parseActionString(std::string action_string) {
    for(std::string token: Action::tokenize(action_string)){
        if(token != action_type){
            if(boost::contains(token, "name=")) {
                token.erase(0, token.find("=")+1);
                name = token;
            } else if (boost::contains(token, "value=")){
                token.erase(0, token.find("=")+1);
                values.push_back(token);
            } else if (boost::contains(token, "=")){
                std::vector<std::string> tmp;
                boost::split(tmp, token, boost::is_any_of("="));
                optionals.insert(std::pair<std::string,std::string>(tmp.front(), tmp.back()));
                continue;
            }
        }
    }
    if(name.empty() or values.empty()){
        throw exception::InvalidActionException("No name or value token");
    }
}

std::string pkg::action::AttributeAction::toActionString() {
    std::ostringstream stringStream;
    stringStream << action_type << " name=" << name;
    for(auto value : values){
        stringStream << " value=" << value;
    }
    return stringStream.str();
}
