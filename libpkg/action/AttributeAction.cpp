//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "AttributeAction.h"
#include <sstream>
#include <boost/algorithm/string/erase.hpp>
#include "Exception.h"

void pkg::action::AttributeAction::parseActionString(const std::string &action_string) {
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(action_string, boost::escaped_list_separator<char>("\\", " ", "\""));
    for(std::string token: tokens){
        if(token != action_type){
            boost::algorithm::erase_all(token, "\"");
            if(boost::contains(token, "name=")) {
                boost::algorithm::erase_first_copy(token, "name=");
                name = token;
            } else if (boost::contains(token, "value=")){
                boost::algorithm::erase_first_copy(token, "value=");
                values.push_back(token);
            } else if (boost::contains(token, "=")){
                std::vector<std::string> tmp;
                boost::split(tmp, token, boost::is_any_of("="));
                name = tmp.front();
                values.push_back(tmp.back());
            } else {
                throw exception::InvalidActionException("No name or value token");
            }
        }
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