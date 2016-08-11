//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <boost/tokenizer.hpp>
#include <vector>
#include "DirectoryAction.h"
#include "Exception.h"

void pkg::action::DirectoryAction::parseActionString(std::string action_string) {
    Action::tokenize(action_string);
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(action_string, boost::escaped_list_separator<char>("\\", " ", "\"\'"));
    for(std::string token: tokens){
        if(token != action_type){
            if(boost::contains(token, "path=")) {
                boost::algorithm::erase_first_copy(token, "path=");
                path = token;
            } else if (boost::contains(token, "owner=")){
                boost::algorithm::erase_first_copy(token, "owner=");
                owner = token;
            } else if (boost::contains(token, "group=")){
                boost::algorithm::erase_first_copy(token, "group=");
                group = token;
            } else if (boost::contains(token, "mode=")){
                boost::algorithm::erase_first_copy(token, "mode=");
                mode = token;
            } else if (boost::contains(token, "facet.")){
                std::vector<std::string> tmp;
                boost::split(tmp, token, boost::is_any_of("="));
                facets.insert(std::pair<std::string,std::string>(tmp.front(), tmp.back()));
            } else {
                throw exception::InvalidActionException("Value " + token + " not recognized");
            }
        }
    }
}

std::string pkg::action::DirectoryAction::toActionString() {
    return "";
}