//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "DependAction.h"
#include "Exception.h"

void pkg::action::DependAction::parseActionString(std::string action_string) {
    Action::clearActionString(action_string);
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(action_string, boost::escaped_list_separator<char>("\\", " ", "\"\'"));
    for(std::string token: tokens){
        if(token != action_type){
            if(boost::contains(token, "fmri=")) {
                boost::algorithm::erase_first_copy(token, "fmri=");
                fmri = token;
            } else if (boost::contains(token, "type=")){
                boost::algorithm::erase_first_copy(token, "type=");
                type = token;
            } else if (boost::contains(token, "predicate=")){
                boost::algorithm::erase_first_copy(token, "predicate=");
                predicate = token;
            } else if (boost::contains(token, "=")){
                std::vector<std::string> tmp;
                boost::split(tmp, token, boost::is_any_of("="));
                optional.insert(std::pair<std::string,std::string>(tmp.front(),tmp.back()));
            } else {
                throw exception::InvalidActionException("Value " + token + " not recognized");
            }
        }
    }
    if(fmri.empty()){
        throw exception::InvalidActionException("fmri missing");
    }
    if(type.empty()){
        throw exception::InvalidActionException("type missing");
    }
}

std::string pkg::action::DependAction::toActionString() {
    return "";
}
