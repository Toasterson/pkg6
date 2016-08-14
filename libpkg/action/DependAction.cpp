//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <vector>
#include <boost/algorithm/string.hpp>
#include "DependAction.h"
#include "Exception.h"

void pkg::action::DependAction::parseActionString(std::string action_string) {
    for(std::string token: Action::tokenize(action_string)){
        if(token != action_type){
            if(boost::contains(token, "fmri=")) {
                token.erase(0, token.find("=")+1);
                fmri = token;
            } else if (boost::contains(token, "type=")){
                token.erase(0, token.find("=")+1);
                type = token;
            } else if (boost::contains(token, "predicate=")){
                token.erase(0, token.find("=")+1);
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
