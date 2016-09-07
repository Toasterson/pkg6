//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PUBLISHER_H
#define PKG6_PUBLISHER_H


#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <map>
#include <string>
#include <vector>

using namespace rapidjson;
using namespace std;

namespace pkg{
    class Publisher{
    private:
        map<string, string> ssl;
        map<string, string> repo;
        map<string, string> mirrors;
        vector<string> origins;
        map<string, string> properties;

        //TODO Proxy support If needed.

    public:
        Publisher(){}
        Publisher(const std::string& uri){
            origins.push_back(uri);
        }

        string getSSLProperty(const string& key){ return ssl[key]; }
        string getRepoProperty(const string& key){ return repo[key]; }
        string getMirrorProperty(const string& key){ return mirrors[key]; }
        vector<string> getOrigins(){ return origins; }
        string getGeneralProperty(const string& key){ return properties[key]; }

        void setSSLProperty(const string& key, const string& value){ ssl[key] = value; }
        void setRepoProperty(const string& key, const string& value){ repo[key] = value; }
        void setMirrorProperty(const string& key, const string& value){ mirrors[key] = value; }
        void addOrigin(const string& value){ origins.push_back(value); }
        void setOrigins(const vector<string>& newOrigins){ origins = newOrigins; }
        void setGeneralProperty(const string& key, const string& value){ properties[key] = value; }

        template <typename Writer>
        void Serialize(Writer& writer) const{
            writer.StartObject();


            //SSL Properties
            writer.String("ssl");
            writer.StartObject();
            for(auto pair : ssl){
                writer.String(pair.first.c_str());
                writer.String(pair.second.c_str());
            }
            writer.EndObject();

            //Repo Properties
            writer.String("repo");
            writer.StartObject();
            for(auto pair : repo){
                writer.String(pair.first.c_str());
                writer.String(pair.second.c_str());
            }
            writer.EndObject();

            //Mirror Properties
            writer.String("mirrors");
            writer.StartObject();
            for(auto pair : mirrors){
                writer.String(pair.first.c_str());
                writer.String(pair.second.c_str());
            }
            writer.EndObject();

            //Origin Properties
            writer.String("origins");
            writer.StartArray();
            for(auto origin : origins){
                writer.String(origin.c_str());
            }
            writer.EndArray();

            //Properties
            writer.String("properties");
            writer.StartObject();
            for(auto pair : properties){
                writer.String(pair.first.c_str());
                writer.String(pair.second.c_str());
            }
            writer.EndObject();
            writer.EndObject();
        }

        void Deserialize(const Value& rootValue){
            if(rootValue.HasMember("ssl")) {
                for (auto itr = rootValue["ssl"].MemberBegin(); itr != rootValue["ssl"].MemberEnd(); ++itr) {
                    ssl.insert(pair<string,string>(itr->name.GetString(), itr->value.GetString()));
                }
            }
            if(rootValue.HasMember("repo")) {
                for (auto itr = rootValue["repo"].MemberBegin(); itr != rootValue["repo"].MemberEnd(); ++itr) {
                    repo.insert(pair<string,string>(itr->name.GetString(), itr->value.GetString()));
                }
            }
            if(rootValue.HasMember("mirrors")) {
                for (auto itr = rootValue["mirrors"].MemberBegin(); itr != rootValue["mirrors"].MemberEnd(); ++itr) {
                    mirrors.insert(pair<string,string>(itr->name.GetString(), itr->value.GetString()));
                }
            }
            if(rootValue.HasMember("origins")) {
                auto &Vorigins = rootValue["origins"];
                for (rapidjson::SizeType i = 0; i < Vorigins.Size(); i++)
                {
                    origins.push_back(Vorigins[i].GetString());
                }
            }
            if(rootValue.HasMember("properties")) {
                for (auto itr = rootValue["properties"].MemberBegin(); itr != rootValue["properties"].MemberEnd(); ++itr) {
                    properties.insert(pair<string,string>(itr->name.GetString(), itr->value.GetString()));
                }
            }
        }
    };
}
#endif //PKG6_PUBLISHER_H
