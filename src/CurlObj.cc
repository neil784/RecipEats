#include "../includes/CurlObj.hpp"

CurlObj::CurlObj(std::string& url) {
    curl_ = curl_easy_init();

    if (curl_ == nullptr) {
        throw std::runtime_error("Curl did not initialize.");
    }

    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &CurlWriter);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &curl_buffer_);
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_perform(curl_);
}

int CurlObj::CurlWriter(char *data, int size, int nmemb, std::string *buffer) {
    int result = 0;
    if (buffer != nullptr) {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    
    return result;
}
