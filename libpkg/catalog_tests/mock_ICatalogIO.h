//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_MOCK_ICATALOGIO_H
#define PKG6_MOCK_ICATALOGIO_H
#include "gmock/gmock.h"
#include <interfaces/ICatalogStorage.h>
class MockICatalogIO : public ICatalogIO {
    public:
        MockICatalogIO(const std::string &statePath) : ICatalogIO(statePath) {};
        MOCK_METHOD0(createStatePath, bool());
        MOCK_METHOD3(importLegacy, bool(const string& origin, const int& version, pkg::Catalog &catalog));
        MOCK_METHOD0(isLegacy, bool());
        MOCK_METHOD1(packageExists, bool(const string& fmri));
        MOCK_METHOD1(addPackage, bool(const pkg::PackageInfo& pkg));
        MOCK_METHOD1(savePackage, bool(const pkg::PackageInfo& pkg));
        MOCK_METHOD1(updatePackage, bool(const pkg::PackageInfo& updatePkg));
        MOCK_METHOD1(addOrUpdatePackage, bool(const pkg::PackageInfo& pkg));
        MOCK_METHOD1(removePackage, bool(const pkg::PackageInfo& pkg));
        MOCK_METHOD1(loadPackage, pkg::PackageInfo(const string& fmri));
        MOCK_METHOD1(DownloadCatalogData, Document(const string& url));
        MOCK_METHOD1(DownloadCatalogUpdateData, Document(const string& url));
    };
#endif //PKG6_MOCK_ICATALOGIO_H
