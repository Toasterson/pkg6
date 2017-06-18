#include <interfaces/ICatalogStorage.h>
#include "gmock/gmock.h"

namespace pkg {

    class MockICatalogStorage : public ICatalogStorage {
    public:
        MOCK_METHOD0(open,
                     FILE());

        MOCK_METHOD0(close,
                     bool());

        MOCK_METHOD0(create,
                     bool());

        MOCK_METHOD0(does_apply,
                     bool());

        MOCK_METHOD0(getPackageCount,
                     int());

        MOCK_METHOD0(getPackageVersionCount,
                     int());
        MOCK_METHOD1(packageExists,
                     bool(
                             const string &fmri));
        MOCK_METHOD1(hasPublisher,
                     bool(
                             const string &publisher));

        MOCK_METHOD0(getPublishers,
                     vector<string>());

        MOCK_METHOD0(getAllPackageNames,
                     vector<string>());
        MOCK_METHOD1(getNewestPackage,
                     pkg::PackageInfo(
                             const string &fmri));

        MOCK_METHOD1(addPackage,
                     bool(
                             const pkg::PackageInfo &pkg));

        MOCK_METHOD1(savePackage,
                     bool(
                             const pkg::PackageInfo &pkg));

        MOCK_METHOD1(updatePackage,
                     bool(
                             const pkg::PackageInfo &updatePkg));

        MOCK_METHOD1(addOrUpdatePackage,
                     bool(
                             const pkg::PackageInfo &pkg));

        MOCK_METHOD1(removePackage,
                     bool(
                             const pkg::PackageInfo &pkg));
        MOCK_METHOD1(loadPackage,
                     pkg::PackageInfo(
                             const string &fmri));
        MOCK_METHOD1(loadNewestPackageVersion,
                     pkg::PackageInfo(
                             const string &partialFmri));

        MOCK_METHOD1(transferPackages,
                     bool(ICatalogStorage
                             &targetInterface));
    };

}  // namespace pkg
