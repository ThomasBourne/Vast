#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>

void ConnectToServer() {
	std::ostringstream encodedVal;
	encodedVal << "uid=test" << "&" << "pid=123";
	sf::Http http("http://localhost");
	sf::Http::Request request("index.php", sf::Http::Request::Post);
	request.setHttpVersion(1, 1); // HTTP 1.1
	request.setField("From", "me");
	request.setField("Content-Type", "application/x-www-form-urlencoded");
	request.setBody(encodedVal.str());
	sf::Http::Response response = http.sendRequest(request);
	std::cout << "status: " << response.getStatus() << std::endl;
	std::cout << "HTTP version: " << response.getMajorHttpVersion() << "." << response.getMinorHttpVersion() << std::endl;
	std::cout << "Content-Type header:" << response.getField("Content-Type") << std::endl;
	std::cout << "body: " << response.getBody() << std::endl;
}

#endif