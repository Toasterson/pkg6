//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_DEPENDACTION_H
#define PKG6_DEPENDACTION_H

#include "Action.h"

namespace pkg{
    namespace action{
        class DependAction : Action{
        public:
            DependAction(): action_type("depend"){}
            DependAction(const std::string &action_string): action_type("depend") {
                parseActionString(action_string);
            }

            const std::string action_type;
            std::string fmri;
            std::string type;
            void parseActionString(const std::string &action_string);

            std::string toActionString();

            template <typename Writer>
            void Serialize(Writer& writer) const{
                writer.StartObject();
                writer.String(fmri.c_str());
                writer.String(type.c_str());
                writer.EndObject();
            }

            void Deserialize(const Value& rootValue){
                if(rootValue.IsObject()){
                    for(Value::ConstMemberIterator itr = rootValue.MemberBegin(); itr == rootValue.MemberEnd(); ++itr){
                        fmri = itr->name.GetString();
                        type = itr->value.GetString();
                    }
                }
            }
        };
    }
}



#endif //PKG6_DEPENDACTION_H
