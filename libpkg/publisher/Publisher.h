//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PUBLISHER_H
#define PKG6_PUBLISHER_H

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
        map<string, string> origins;
        map<string, string> properties;

        //TODO Proxy support If needed.

    public:
        Publisher(){}

        string getSSLProperty(const string& key){ return ssl[key]; }
        string getRepoProperty(const string& key){ return repo[key]; }
        string getMirrorProperty(const string& key){ return mirrors[key]; }
        string getOriginProperty(const string& key){ return origins[key]; }
        string getGeneralProperty(const string& key){ return properties[key]; }

        void setSSLProperty(const string& key, const string& value){ ssl[key] = value; }
        void setRepoProperty(const string& key, const string& value){ repo[key] = value; }
        void setMirrorProperty(const string& key, const string& value){ mirrors[key] = value; }
        void setOriginProperty(const string& key, const string& value){ origins[key] = value; }
        void setGeneralProperty(const string& key, const string& value){ properties[key] = value; }

        template <typename Writer>
        void Serialize(){

        }

        void Deserialize(const Value& rootValue){

        }
    };
}
#endif //PKG6_PUBLISHER_H
