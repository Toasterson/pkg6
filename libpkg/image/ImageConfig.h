//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGECONFIG_H
#define PKG6_IMAGECONFIG_H


#include <publisher/Publisher.h>
#include <string>
#include <map>
#include <vector>


namespace pkg {
    class ImageConfig {
    private:
        const std::string IMAGE_CONFIG_OLDFILENAME = "pkg5.image";
        const std::string IMAGE_CONFIG_FILENAME = "pkg6.image";
        int VERSION = 4;
        string IMAGE_ROOT;
        map<string, Publisher> publishers;
        map<string, string> properties;
        map<string, string> variants;
    public:
        int getVersion() { return VERSION; };


        std::string getVariant(const std::string& name);
        pkg::Publisher getPublisher(const std::string& name);
        pkg::Publisher getFirstPublisher();
        std::string getImageProperty(const std::string& name);

        void setVariant(const std::string& name, const std::string& value);
        void setImageProperty(const std::string& name, const std::string& value);
        void setPublisher(const std::string& name, const Publisher& value);

        ImageConfig()= default;
        explicit ImageConfig(const std::string& root);
        void load(std::istream& config);
        void save(std::ostream& output);
        void upgrade_format(const std::string &newRoot);
        void importpkg5(std::istream& oldconfig);

        template <typename Writer>
        void Serialize(Writer& writer) const{
            writer.StartObject();
            writer.String("version");
            writer.Int(VERSION);

            //Variants
            writer.String("variants");
            writer.StartObject();
            for(auto pair : variants){
                writer.String(pair.first.c_str());
                writer.String(pair.second.c_str());
            }
            writer.EndObject();

            //Properties
            writer.String("properties");
            writer.StartObject();
            for(auto pair : properties){
                writer.String(pair.first.c_str());
                writer.String(pair.second.c_str());
            }
            writer.EndObject();

            //Publishers
            writer.String("publishers");
            writer.StartObject();
            for(auto pair : publishers){
                writer.String(pair.first.c_str());
                pair.second.Serialize(writer);
            }
            writer.EndObject();
            writer.EndObject();
        }

        void Deserialize(const Value& rootValue){
            VERSION = rootValue["version"].GetInt();
            if(rootValue.HasMember("variants")) {
                for (auto itr = rootValue["variants"].MemberBegin(); itr != rootValue["variants"].MemberEnd(); ++itr) {
                    variants.insert(pair<string,string>(itr->name.GetString(), itr->value.GetString()));
                }
            }
            if(rootValue.HasMember("properties")) {
                for (auto itr = rootValue["properties"].MemberBegin(); itr != rootValue["properties"].MemberEnd(); ++itr) {
                    properties.insert(pair<string,string>(itr->name.GetString(), itr->value.GetString()));
                }
            }
            if(rootValue.HasMember("publishers")) {
                for (auto itr = rootValue["publishers"].MemberBegin(); itr != rootValue["publishers"].MemberEnd(); ++itr) {
                    Publisher pub;
                    pub.Deserialize(itr->value);
                    publishers.insert(pair<string,Publisher>(itr->name.GetString(), pub));
                }
            }
        }

    };
};


#endif //PKG6_IMAGECONFIG_H
