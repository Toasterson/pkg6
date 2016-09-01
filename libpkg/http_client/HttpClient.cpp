//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "HttpClient.h"
#include "HTTPException.h"
#include <restbed>
#include <iostream>

using namespace std;
using namespace restbed;

void HttpClient::getVersion_0() {
    auto request = make_shared< Request >( Uri( "http://www.golem.de" ) );
    request->set_header( "Accept", "*/*" );
    /*
    request->set_header( "Cache-Control", "no-cache" );
    request->set_header("Accept-Encoding", "gzip,deflate");
    request->set_header("User-Agent", "PKG6/0.0.1");
     */
    request->set_header("Host", "www.golem.de");
    //request->set_method("GET");

    auto response = Http::sync( request );

    int code = response->get_status_code();
/*
    if(code != 200){
        throw pkg::exception::HTTPBadResponseException(code);
    }
*/
    print(response);

    /*
    auto future = Http::async( request, [ ]( const shared_ptr< Request >, const shared_ptr< Response > response )
    {

    } );

    future.wait( );

    */
}

HttpClient::HttpClient(const std::string &base_url, const std::string& publisher):
        base_url(base_url),
        publisher(publisher)
{
    getVersion_0();
}

void HttpClient::print(const std::shared_ptr<restbed::Response> &response) {
    fprintf( stderr, "\n*** Response ***\n" );
    fprintf( stderr, "Status Code:    %i\n", response->get_status_code( ) );
    fprintf( stderr, "Status Message: %s\n", response->get_status_message( ).data( ) );
    fprintf( stderr, "HTTP Version:   %.1f\n", response->get_version( ) );
    fprintf( stderr, "HTTP Protocol:  %s\n", response->get_protocol( ).data( ) );

    for ( const auto header : response->get_headers( ) )
    {
        fprintf( stderr, "Header '%s' > '%s'\n", header.first.data( ), header.second.data( ) );
    }

    auto length = 0;
    response->get_header( "Content-Length", length );

    Http::fetch( length, response );

    fprintf( stderr, "Body:           %.*s...\n\n", 25, response->get_body( ).data( ) );
}

