#include "ApiClient.hpp"

using json = nlohmann::json;

// Constructor to initialize the server address
ApiClient::ApiClient(const std::string& serverAddress) {
    // Determine whether to use HTTP or HTTPS client
    if (serverAddress.find("https://") == 0) {
        sslClient = std::make_unique<httplib::SSLClient>(serverAddress.substr(8)); // Strip "https://"
    } else if (serverAddress.find("http://") == 0) {
        client = std::make_unique<httplib::Client>(serverAddress.substr(7)); // Strip "http://"
    } else {
        throw std::invalid_argument("Invalid server address. Must start with http:// or https://");
    }
}

void ApiClient::setEndpoint(const std::string& endpoint) {
    this->endpoint = endpoint;
}

void ApiClient::setParameter(const std::string& parameter) {
    this->parameter = parameter;
}

void ApiClient::setPayload(const json& payload) {
    this->payload = payload;
}

std::string ApiClient::sendGETRequest() {
    const std::string requestEndpoint = endpoint.empty() ? "/" : endpoint;
    const std::string requestParameter = parameter.empty() ? "" : parameter;
    std::string requestCombined = requestEndpoint + requestParameter;

    httplib::Result res;
    
    if (sslClient) {
        res = sslClient->Get(requestCombined.c_str());
    } else if (client) {
        res = client->Get(requestCombined.c_str());
    } else {
        std::cout << "Neither client nor sslClient is initialized." << std::endl;
    }
    // auto res = client->Get(requestCombined.c_str());

    if (res) {
        if (res->status == 200) {
            return "200";
            // return res->body; // Return the response body
        } else {
            return "Server returned error: " + std::to_string(res->status);
        }
    }
    return "Error: " + errorToString(res.error());
}

std::string ApiClient::sendPOSTRequest() {
    const std::string requestEndpoint = endpoint.empty() ? "/" : endpoint;
    const std::string requestParameter = parameter.empty() ? "" : parameter;
    std::string requestCombined = requestEndpoint + requestParameter;

    if (endpoint.empty()) {
        return "Error: Endpoint is not set.";
    }
    if (payload.empty()) {
        return "Error: Payload is not set.";
    }

    httplib::Result res;
    
    if (sslClient) {
        res = sslClient->Post(requestCombined.c_str(), payload.dump(), "application/json");
    } else if (client) {
        res = client->Post(requestCombined.c_str(), payload.dump(), "application/json");
    } else {
        std::cout << "Neither client nor sslClient is initialized." << std::endl;
    }

    if (res) {
        if (res->status == 200) {
            return "200"; // Return the response body
        } else {
            return "Server returned error: " + std::to_string(res->status);
        }
    }
    return "Error: " + errorToString(res.error());
}

std::string ApiClient::errorToString(httplib::Error err) {
    switch (err) {
        case httplib::Error::Success:
            return "Success";
        case httplib::Error::Connection:
            return "Connection error";
        case httplib::Error::BindIPAddress:
            return "Bind IP address error";
        case httplib::Error::Read:
            return "Read error";
        case httplib::Error::Write:
            return "Write error";
        case httplib::Error::ExceedRedirectCount:
            return "Exceeded redirect count";
        case httplib::Error::Canceled:
            return "Request canceled";
        case httplib::Error::SSLConnection:
            return "SSL connection error";
        case httplib::Error::SSLLoadingCerts:
            return "SSL loading certificates error";
        case httplib::Error::SSLServerVerification:
            return "SSL server verification error";
        default:
            return "Unknown error";
    }
}
