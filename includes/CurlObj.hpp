#ifndef CURLOBJ_HPP
#define CURLOBJ_HPP

#include <curl/curl.h>
#include <regex>
#include <iostream>

class CurlObj {

public:
    CurlObj (std::string& url);
    static int CurlWriter(char *data, int size, int nmemb, std::string *buffer);
    std::string GetData() const { return curl_buffer_; };
    
private:
    CURL* curl_;
    std::string curl_buffer_;
};

#endif