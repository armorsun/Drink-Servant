
// basic-http-client.cpp
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
											// Creates an HTTP request and prints the length of the response stream.


											// Builds an HTTP request that uses custom header values.
pplx::task<void> HTTPRequestCustomQueryAsync(float d, float a)
{
	http_client client(U("http://192.168.43.170:54321/"));

	uri_builder builder;

	builder.append_query(U("distance"), d);
	builder.append_query(U("angle"), a);


	//request.set_request_uri(L"requestpath");
	return client.request(methods::GET, builder.to_string()).then([](http_response response)
	{
		// Print the status code.
		std::wostringstream ss;
		ss << L"Server returned returned status code " << response.status_code() << L"." << std::endl;
		std::wcout << ss.str();
	});

	/* Sample output:
	Server returned returned status code 200.
	*/
}

