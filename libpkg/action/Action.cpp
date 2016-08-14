//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <vector>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <regex>
#include "Action.h"


std::vector<std::string> pkg::Action::tokenize(std::string action_string) {
    std::vector<std::string> tokens;
    action_string.erase(0, action_string.find(" ")+1);
    while(boost::contains(action_string, "=")){
        //First copy up until the = character into strstr
        std::string str;
        std::stringstream strstr(str);
        size_t firstequalspos = action_string.find("=");
        strstr << action_string.substr(0, firstequalspos+1);
        //Then move that out of action_string
        action_string.erase(0, firstequalspos+1);
        if(boost::contains(action_string, "=") and boost::contains(action_string, " ")){
            //If we have still a = in the string then find the = of the next pair.
            //and copy everything up until the space char just before into strstr
            size_t secondequalpos = action_string.find("=");
            size_t spacepos = action_string.rfind(" ", secondequalpos);
            strstr << action_string.substr(0, spacepos);
            action_string.erase(0, spacepos+1);
        } else {
            //If we do not have a = char anymore in action_string then copy the rest of action_string into strstr and be done with it
            strstr << action_string;
            action_string = action_string.empty();
        }
        str = strstr.str();
        //Now clean the strings from any \" " or ' sequences
        boost::erase_all(str, "\\\"");
        boost::erase_all(str, "\"");
        boost::erase_all(str, "\'");
        tokens.push_back(str);
    }
    return tokens;
}
