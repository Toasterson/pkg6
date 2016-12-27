//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_JSONPACKAGESERIALIZER_H
#define PKG6_JSONPACKAGESERIALIZER_H


#include <package/PackageInfo.h>

class JSONPackageSerializer {
    MemoryPoolAllocator<CrtAllocator> alloc;
public:
    void Serialize(const pkg::PackageInfo &pkg, Value &val);
    void SerializeAttributeAction(const AttributeAction &attr, Value &val);
    void SerializeDependencyAction(const DependAction &dep, Value &val);
    pkg::PackageInfo Deserialize(Value &rootValue);
    pkg::action::AttributeAction DeserializeAttributeAction(const Value &rootValue);
    pkg::action::DependAction DeserializeDependAction(const Value &rootValue);
};


#endif //PKG6_JSONPACKAGESERIALIZER_H
