//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_HTTPCLIENT_H
#define PKG6_HTTPCLIENT_H

#include <string>
#include <map>
#include <restbed>


class HttpClient {
private:
    std::map<std::string, int> SERVER_VERSION;
    const static int CLIENT_VERSION = 3;

    std::string base_url;
    std::string publisher;

    void print(const std::shared_ptr< restbed::Response >& response );


    /*
     * Actual REST functions called by the Public functions
     */
    void getVersion_0();

    void getInfo_0();

    void getIndex_0();

    void getSearch_0();

    void getSearch_1();

    void getP5I_0();

    void getPublisher_0();

    void getPublisher_1();

    void getCatalog_0();

    void getCatalog_1();

    void getFileList_0();

    void getManifest_0();

    void getManifest_1();

    void getFile_0();

    void getFile_1();



public:

    HttpClient(const std::string& base_url, const std::string& publisher);

    void getInfo();

    void getIndex();

    void search();

    void getPublisher();

    void getCatalog();

    void getManifest();

    void getFile();

};


#endif //PKG6_HTTPCLIENT_H
