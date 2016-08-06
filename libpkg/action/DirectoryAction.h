//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_DIRECTORYACTION_H
#define PKG6_DIRECTORYACTION_H

#include <string>
#include <map>
#include <boost/algorithm/string.hpp>
#include <document.h>

using namespace rapidjson;

namespace pkg{
    namespace action{
        class DirectoryAction{
        public:
            DirectoryAction(): action_type("dir"){}
            DirectoryAction(const std::string &action_string): action_type("dir") {
                parseActionString(action_string);
            }

            std::string action_type;
            std::string path;
            std::string mode;
            std::string owner;
            std::string group;

            std::map<std::string,std::string> facets;

            void parseActionString(std::string action_string);

            std::string toActionString();

            template <typename Writer>
            void Serialize(Writer& writer) const{
                writer.StartObject();
                writer.String("path");
                writer.String(path.c_str());
                writer.String("mode");
                writer.String(mode.c_str());
                writer.String("owner");
                writer.String(owner.c_str());
                writer.String("group");
                writer.String(group.c_str());
                for(auto pair : facets){
                    writer.String(pair.first.c_str());
                    writer.String(pair.second.c_str());
                }
                writer.EndObject();
            }

            void Deserialize(const Value& rootValue){
                if(rootValue.IsObject()){
                    for(Value::ConstMemberIterator itr = rootValue.MemberBegin(); itr == rootValue.MemberEnd(); ++itr){
                        if(boost::contains(itr->name.GetString(), "facet.")){
                            facets.insert(std::pair<std::string,std::string>(itr->name.GetString(), itr->value.GetString()));
                        } else if(itr->name.GetString() == "path"){
                            path = itr->value.GetString();
                        } else if(itr->name.GetString() == "mode"){
                            mode = itr->value.GetString();
                        } else if(itr->name.GetString() == "owner"){
                            owner = itr->value.GetString();
                        } else if(itr->name.GetString() == "group"){
                            group = itr->value.GetString();
                        }
                    }
                }
            }
        };
    }
}


#endif //PKG6_DIRECTORYACTION_H
