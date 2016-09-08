//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_FILEACTION_H
#define PKG6_FILEACTION_H


#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <map>
#include "Action.h"

using namespace std;
using namespace rapidjson;

namespace pkg {
    namespace action{
        class FileAction : Action {
        public:
            FileAction(): action_type("file"){}
            FileAction(const std::string &action_string): action_type("set") {
                parseActionString(action_string);
            }

            string name;
            int size = 0, csize = 0;
            //The following attributes are contained for a file
            //chash, owner, group, mode, path, preserve, overlay, original_name, release_note, revert_tag, elfarch, elfbits, elfhash;
            map<string,string> attributes;


            void parseActionString(std::string action_string);

            std::string toActionString();

            void install();

            bool validate();

            template <typename Writer>
            void Serialize(Writer& writer) const {

            }

            void Deserialize(const Value& rootValue){

            }
        };
    }
}

#endif //PKG6_FILEACTION_H
