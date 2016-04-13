//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <History.h>
#include <tinyxml2.h>
#include <pkgdefs.h>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"
#include <iostream>
#include <iomanip>

namespace fs = boost::filesystem;

std::string pkg::history::History::path() {
 return root_dir + "/history";
}

std::string pkg::history::History::pathname() {
 if(!(operation.name == "")){
     return "";
 }
 else{
     char buf[sizeof "20160324T215420Z"];
     strftime(buf, sizeof "20160324T215420Z", ISO8601_PARSE_STRING, operation.start_time);
     return path()+"/"+buf+"-01.xml";
 }
}

std::string pkg::history::History::notes() {
    if (operation.release_notes == ""){
        return "";
    }
    fs::path full_path = fs::system_complete((root_dir+"/notes/"+operation.release_notes).c_str());
    if(!fs::is_regular_file(full_path)){
        return "";
    }
    fs::ifstream tfs(full_path);
    std::string release_notes_content;
    //Use filestream to allocate memory of release notes pre writing to it
    //Faster?
    tfs.seekg(0, std::ios_base::end);
    release_notes_content.reserve((unsigned long) tfs.tellg());
    tfs.seekg(0, std::ios_base::beg);
    release_notes_content.assign((std::istreambuf_iterator<char>(tfs)), std::istreambuf_iterator<char>());
    return release_notes_content;
}

void pkg::history::History::clear() {
    client_name = "";
    client_version = "";
    client_args = "";
    operation = HistoryOperation();
}

void pkg::history::History::purge(const std::string &be_name = "", const std::string &be_uuid = "") {
    operation.name = API_OP::PURGE_HISTORY;
    operation.be = be_name;
    operation.be_uuid = be_uuid;
    fs::remove_all(fs::system_complete(path().c_str()));
    operation.result = RESULT_SUCCEEDED;
}

void pkg::history::History::abort(const std::string &result) {
    operation.result = result;
    operation.error = RESULT_FAILED_UNKNOWN;
    save();
}

void pkg::history::History::logStart(const std::string &name, const std::string &be_name, const std::string &be_uuid) {
    operation.name=name;
    operation.be=be_name;
    operation.be_uuid=be_uuid;
}

void pkg::history::History::logEnd(const std::string &error = "", const std::string &result = "",
                                   const std::string &release_notes = "") {
    if(result=="" and error==""){
        operation.result = RESULT_SUCCEEDED;
    } else if(result == "" and error != ""){
        logError(error);
    } else {
        operation.result = result;
    }

    if(release_notes != ""){
        operation.release_notes = release_notes;
    }
}

void pkg::history::History::logError(const std::string &error = "") {
    operation.error = error;
}

void pkg::history::History::load(const std::string &filename) {
    using namespace tinyxml2;
    XMLDocument doc;
    doc.LoadFile((root_dir + "/" + filename).c_str());
    XMLElement *history = doc.FirstChildElement("history");
    XMLElement *client = history->FirstChildElement("client");
    client_name = client->Attribute("name");
    client_version = client->Attribute("version");
    for (XMLElement *element = client->FirstChildElement("args")->FirstChildElement("arg");
         element != NULL;
         element = element->NextSiblingElement("arg")) {
        client_args.append(element->GetText());
        client_args.append(" ");
    }
    ltrim(client_args);
    operation = HistoryOperation();
    XMLElement* oper = history->FirstChildElement("operation");
    operation.be = oper->Attribute("be");
    operation.be_uuid = oper->Attribute("be_uuid");
    strptime(oper->Attribute("start_time"), ISO8601_PARSE_STRING, &operation.start_time);
    strptime(oper->Attribute("end_time"), ISO8601_PARSE_STRING, &operation.end_time);
    strptime(oper->Attribute("snapshot"), SNAPSHOT_PARSE_STRING, &operation.snapshot);
    operation.be_uuid = oper->Attribute("be_uuid");
    operation.name = oper->Attribute("name");
    operation.result = oper->Attribute("result");
    operation.userid = oper->Attribute("userid");
    operation.username = oper->Attribute("username");
    operation.start_state = oper->FirstChildElement("start_state")->GetText();
    operation.end_state = oper->FirstChildElement("end_state")->GetText();
}

void pkg::history::History::save() {
    using namespace tinyxml2;
    XMLDocument doc;
    auto Xhist = doc.NewElement("history");
    auto Xclient = doc.NewElement("client");
    auto Xargs = doc.NewElement("args");
    auto Xstart_state = doc.NewElement("start_state");
    auto Xend_state = doc.NewElement("end_state");
    auto Xoper = doc.NewElement("operation");

    Xclient->Attribute("name", client_name.c_str());
    Xclient->Attribute("version", client_version.c_str());

    std::vector<std::string> argstrs;
    boost::split(argstrs, client_args, " ");
    for(auto &argstr : argstrs){
        auto arg = doc.NewElement("arg");
        arg->SetValue(argstr.c_str());
        Xargs->InsertFirstChild(arg);
    }

    Xoper->Attribute("be", operation.be.c_str());
    Xoper->Attribute("be_uuid", operation.be_uuid.c_str());
    char start_time[80] = "", end_time[80] = "", snapshot[80] = "";
    strftime(start_time, 80, ISO8601_PARSE_STRING, operation.start_time);
    strftime(end_time, 80, ISO8601_PARSE_STRING, operation.end_time);
    strftime(snapshot, 80, SNAPSHOT_PARSE_STRING, operation.snapshot);
    Xoper->Attribute("start_time", start_time);
    Xoper->Attribute("end_time", end_time);
    Xoper->Attribute("snapshot", snapshot);
    Xoper->Attribute("name", operation.name.c_str());
    Xoper->Attribute("result", operation.result.c_str());
    Xoper->Attribute("userid", operation.userid.c_str());
    Xoper->Attribute("username", operation.username.c_str());

    Xstart_state->SetValue(operation.start_state.c_str());
    Xend_state->SetValue(operation.end_state.c_str());

    Xoper->InsertFirstChild(Xstart_state);
    Xoper->InsertFirstChild(Xend_state);

    Xclient->InsertFirstChild(Xargs);

    Xhist->InsertFirstChild(Xclient);
    Xhist->InsertFirstChild(Xoper);

    doc.SaveFile(pathname().c_str());
}





