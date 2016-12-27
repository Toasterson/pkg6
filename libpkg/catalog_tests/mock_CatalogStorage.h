#include <interfaces/ICatalogStorage.h>
#include "gmock/gmock.h"
class MockICatalogStorage : public pkg::ICatalogStorage {
 public:
  MOCK_METHOD0(open, FILE());
  MOCK_METHOD0(close, bool());
  MOCK_METHOD0(create, bool());
  MOCK_METHOD0(does_apply, bool());
  MOCK_METHOD0(getPackageCount, int());
  MOCK_METHOD0(getPackageVersionCount, int());
  MOCK_METHOD1(packageExists, bool(const string &fmri));
  MOCK_METHOD1(addPackage, bool(const pkg::PackageInfo &pkg));
  MOCK_METHOD1(savePackage, bool(const pkg::PackageInfo &pkg));
  MOCK_METHOD1(updatePackage, bool(const pkg::PackageInfo &updatePkg));
  MOCK_METHOD1(addOrUpdatePackage, bool(const pkg::PackageInfo &pkg));
  MOCK_METHOD1(removePackage, bool(const pkg::PackageInfo &pkg));
  MOCK_METHOD1(loadPackage, pkg::PackageInfo(const string &fmri));
  MOCK_METHOD1(transferPackages, bool(ICatalogStorage &targetInterface));
};
