#pragma once
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <cstdio>
#include <cstdlib>
#include <curl/curl.h>

class HttpRequest {
private:
    static size_t write_file(void* ptr, size_t size, size_t nmemb, void* stream) {
        return std::fwrite(ptr, size, nmemb, static_cast<std::FILE*>(stream));
    }

public:
    static bool download(const std::string& url,
        const std::string& out_path,
        long connect_timeout_s = 10,
        long total_timeout_s = 60) {
        CURL* curl = curl_easy_init();
        if (!curl) return false;

        std::FILE* fp = nullptr;

#ifdef _MSC_VER
        // Visual Studio: usar fopen_s
        if (fopen_s(&fp, out_path.c_str(), "wb") != 0 || !fp) {
            curl_easy_cleanup(curl);
            return false;
        }
#else
        // Otros compiladores: fopen normal
        fp = std::fopen(out_path.c_str(), "wb");
        if (!fp) {
            curl_easy_cleanup(curl);
            return false;
        }
#endif

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_file);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "simple-downloader/1.0");
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout_s);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, total_timeout_s);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        CURLcode res = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        std::fclose(fp);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK || http_code < 200 || http_code >= 300) {
            std::remove(out_path.c_str());
            return false;
        }
        return true;
    }
};

#endif // HTTPREQUEST_H
