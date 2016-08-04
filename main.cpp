#include <iostream>
#include <Image.h>

using namespace std;
using namespace pkg;

int main() {
    Image img = Image("/home/toast/workspace/illumos/ips/pkg5/");
    img.upgrade_format("/home/toast/workspace/illumos/ips/pkg6");

    cout << "All Done!" << endl;
    return 0;
}