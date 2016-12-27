//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_DEPENDACTION_H
#define PKG6_DEPENDACTION_H

#include <rapidjson/document.h>
#include <map>
#include <string>
#include "Action.h"

using namespace rapidjson;

namespace pkg{
    namespace action{
        class DependAction: public Action{
        public:
            DependAction(): Action("depend"){}
            explicit DependAction(const std::string &action_string): Action("depend") {
                    parseActionString(action_string);
            }

            std::string fmri;
            std::string type;
            std::string predicate;
            std::map<std::string,std::string> optional;
            void parseActionString(std::string action_string);

            std::string toActionString();

            void install(){}

            bool validate(){ return true; }
        };
    }
}



#endif //PKG6_DEPENDACTION_H
