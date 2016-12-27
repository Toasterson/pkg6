//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ATTRIBUTEACTION_H
#define PKG6_ATTRIBUTEACTION_H


#include <rapidjson/rapidjson.h>
#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <map>
#include "Action.h"

using namespace rapidjson;

namespace pkg{
    namespace action {
        class AttributeAction: public Action{
        public:
            AttributeAction(): Action("set"){}
            explicit AttributeAction(const std::string &action_string): Action("set") {
                parseActionString(action_string);
            }

            std::string name;
            std::vector<std::string> values;
            std::map<std::string,std::string> optionals;
            void parseActionString(std::string action_string);

            std::string toActionString();

            void install(){}

            bool validate(){ return true; }

            bool hasValue(const std::string &value);
        };
    }
}

#endif //PKG6_ATTRIBUTEACTION_H
