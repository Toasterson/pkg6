//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "DependAction.h"
#include "Exception.h"

void pkg::action::DependAction::parseActionString(const std::string &action_string) {
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(action_string, boost::escaped_list_separator<char>("\\", " ", "\""));
    for(std::string token: tokens){
        if(token != action_type){
            boost::algorithm::erase_all(token, "\"");
            if(boost::contains(token, "fmri=")) {
                boost::algorithm::erase_first_copy(token, "fmri=");
                fmri = token;
            } else if (boost::contains(token, "type=")){
                boost::algorithm::erase_first_copy(token, "type=");
                type = token;
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
    return Action::toActionString();
}
