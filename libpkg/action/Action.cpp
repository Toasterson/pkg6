//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <regex>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "Action.h"


void pkg::Action::clearActionString(std::string& action_string) {
    std::regex pattern1("\'\""), pattern2("\"\'") ;
    action_string = std::regex_replace(action_string, pattern1, "\"");
    action_string = std::regex_replace(action_string, pattern2, "\"");
}
