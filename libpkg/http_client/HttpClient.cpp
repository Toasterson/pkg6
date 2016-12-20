//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "HttpClient.h"
#include "HTTPException.h"
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::asio;
using namespace beast;

void HttpClient::getVersion_0() {
    http::response<beast::http::string_body> resp = makeStringHTTPRequest(base_path + "/versions/0");
    string body = resp.body;
    vector<string> v_body;
    boost::split(v_body, body, boost::is_any_of("\n"));
    for(auto line: v_body) {
        vector<string> v_line;
        boost::split(v_line, line, boost::is_any_of(" "));
        if(v_line[0] != "pkg-server"){
            string name = v_line[0];
            v_line.erase(v_line.begin());
            int value = 0;
            for(auto val: v_line){
                value = stoi(val);
            }
            SERVER_VERSION.insert(pair<string,int>(name, value));
        }
    }

}

HttpClient::HttpClient(const std::string &protocol, const std::string &host, const std::string &base_path,
                       const std::string &publisher) :
                        protocol(protocol),
                        host(host),
                        base_path(base_path),
                        publisher(publisher),
                        USER_AGENT("PKG6/0.0.1"){
    getVersion_0();
}

void HttpClient::getManifest(const std::string &manifest, std::ostream& savePath) {
    getManifest_0(manifest, savePath);
}

void HttpClient::getManifest_0(const std::string &manifest, std::ostream& savePath) {
    savePath << makeStringHTTPRequest(base_path + "/" + publisher + "/manifest/0/" + url_encode(manifest)).body;
}

std::string HttpClient::url_encode(const std::string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char) c);
        escaped << nouppercase;
    }

    return escaped.str();
}

beast::http::response<http::string_body> HttpClient::makeStringHTTPRequest(const std::string &url) {
    io_service ios;
    ip::tcp::resolver r{ios};
    ip::tcp::socket sock{ios};
    connect(sock, r.resolve(ip::tcp::resolver::query{host, protocol}));
    http::request<beast::http::empty_body> request;
    request.method = "GET";
    request.url = url;
    request.version = 11;
    request.fields.replace("Host", host + ":" + std::to_string(sock.remote_endpoint().port()));
    request.fields.replace("User-Agent", USER_AGENT);
    http::prepare(request);
    http::write(sock, request);

    // Receive HTTP response using beast
    beast::streambuf sb;
    http::response<beast::http::string_body> resp;
    http::read(sock, sb, resp);
    return resp;
}

beast::http::response<beast::http::streambuf_body> HttpClient::makeStreamHTTPRequest(const std::string &url) {
    io_service ios;
    ip::tcp::resolver r{ios};
    ip::tcp::socket sock{ios};
    connect(sock, r.resolve(ip::tcp::resolver::query{host, protocol}));
    http::request<beast::http::empty_body> request;
    request.method = "GET";
    request.url = url;
    request.version = 11;
    request.fields.replace("Host", host + ":" + std::to_string(sock.remote_endpoint().port()));
    request.fields.replace("User-Agent", USER_AGENT);
    http::prepare(request);
    http::write(sock, request);

    // Receive HTTP response using beast
    beast::streambuf sb;
    http::response<beast::http::streambuf_body> resp;
    http::read(sock, sb, resp);
    return resp;
}

void HttpClient::getFile(const std::string &sha1, std::ostream &savePath) {
    if(SERVER_VERSION["file"] == 1){
        getFile_1(sha1, savePath);
    } else {
        getFile_0(sha1, savePath);
    }
}

void HttpClient::getFile_1(const std::string &sha1, std::ostream &savePath) {
    savePath << makeStringHTTPRequest(base_path + "/" + publisher + "/file/1/" + sha1).body;
}

void HttpClient::getFile_0(const std::string &sha1, std::ostream &savePath) {
    savePath << makeStringHTTPRequest(base_path + "/" + publisher + "/file/0/" + sha1).body;
}
