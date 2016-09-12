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
        class FileAction : public Action {
        public:
            FileAction(): Action("file"){}
            explicit FileAction(const std::string &action_string): Action("file"){
                parseActionString(action_string);
            }

            string name;
            string path;
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
                writer.StartObject();
                //Name
                writer.String("sha1");
                writer.String(name.c_str());
                //Sizes
                writer.String("size");
                writer.Int(size);

                writer.String("csize");
                writer.Int(csize);

                //Path
                writer.String("path");
                writer.String(path.c_str());

                //Attributes
                for(auto attr : attributes){
                    writer.String(attr.first.c_str());
                    writer.String(attr.second.c_str());
                }

                writer.EndObject();
            }

            void Deserialize(const Value& rootValue){
                for(rapidjson::Value::ConstMemberIterator itr = rootValue.MemberBegin(); itr != rootValue.MemberEnd(); ++itr){
                    if(itr->name.GetString() == string("sha1")){
                        name = itr->value.GetString();
                    } else if(itr->name.GetString() == string("size")){
                        size = itr->value.GetInt();
                    } else if(itr->name.GetString() == string("csize")){
                        csize = itr->value.GetInt();
                    } else if(itr->name.GetString() == string("path")){
                        path = itr->value.GetString();
                    } else {
                        attributes[itr->name.GetString()] = itr->value.GetString();
                    }
                }
            }
        };
    }
}

#endif //PKG6_FILEACTION_H
