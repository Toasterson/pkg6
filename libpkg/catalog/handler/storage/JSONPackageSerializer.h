//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_JSONPACKAGESERIALIZER_H
#define PKG6_JSONPACKAGESERIALIZER_H


#include <package/PackageInfo.h>

class JSONPackageSerializer {
public:
    void Serialize(const pkg::PackageInfo &pkg, Value &val);
    void SerializeAttributeAction(const pkg::PackageInfo &pkg, Value &val);
    void SerializeDependencyAction(const pkg::PackageInfo &pkg, Value &val);
    pkg::PackageInfo Deserialize(Value &rootValue);
};


#endif //PKG6_JSONPACKAGESERIALIZER_H
