//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_HTTPCLIENT_H
#define PKG6_HTTPCLIENT_H

#include <string>
#include <map>
#include <image/Image.h>
#include <beast/core.hpp>
#include <beast/http.hpp>

class HttpClient {
private:
    std::map<std::string, int> SERVER_VERSION;
    const static int CLIENT_VERSION = 3;

    const std::string USER_AGENT;

    std::string protocol;
    std::string host;
    std::string base_path;
    std::string publisher;

    std::string url_encode(const std::string& value);

    beast::http::response<beast::http::string_body> makeStringHTTPRequest(const std::string &url);

    beast::http::response<beast::http::streambuf_body > makeStreamHTTPRequest(const std::string &url);

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

    void getManifest_0(const std::string &manifest, std::ostream& savePath);

    void getManifest_1(const std::string &manifest, std::ostream&);

    void getFile_0(const std::string& sha1, std::ostream& savePath);

    void getFile_1(const std::string& sha1, std::ostream& savePath);



public:

    HttpClient(const std::string& protocol, const std::string& host, const std::string& base_path, const std::string& publisher);

    void getInfo();

    void getIndex();

    void search();

    void getPublisher();

    void getCatalog();

    void getManifest(const std::string& manifest, std::ostream& savePath);

    //TODO Rewrite to use FileAction once that exists
    void getFile(const std::string& sha1, std::ostream& savePath);

    std::map<std::string,int> getVersion(){
        return SERVER_VERSION;
    }

};


#endif //PKG6_HTTPCLIENT_H
