//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ACTION_H
#define PKG6_ACTION_H

#include <string>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <map>
#include <writer.h>
#include <document.h>
#include "Exception.h"

using namespace rapidjson;

namespace pkg {
    namespace action {
        class Action {
        public:

            const std::string action_type;

            Action() {}

            Action(const std::string &action_string) {
                parseActionString(action_string);
            }

            virtual void parseActionString(const std::string &action_string){}

            virtual std::string toActionString(){ return ""; }

            template <typename Writer>
            void Serialize(Writer& writer) const{}

            virtual void Deserialize(const Value& rootValue){}

            virtual bool validate() { return true; }

        };
    }
};
#endif //PKG6_ACTION_H
