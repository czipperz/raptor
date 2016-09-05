#ifndef HEADER_GUARD_CURL_H
#define HEADER_GUARD_CURL_H

#include <curl/curl.h>

#if __cplusplus < 201103L
#define LibCurl_NOEXCEPT
#else
#define LibCurl_NOEXCEPT noexcept
#endif

namespace LibCurl {
struct GlobalCurl {
    GlobalCurl() LibCurl_NOEXCEPT { curl_global_init(CURL_GLOBAL_ALL); }
    ~GlobalCurl() LibCurl_NOEXCEPT { curl_global_cleanup(); }

private:
    GlobalCurl(const GlobalCurl&);
    GlobalCurl& operator=(const GlobalCurl&);
};

struct Easy {
    Easy() LibCurl_NOEXCEPT : raw(curl_easy_init()) {}
    ~Easy() LibCurl_NOEXCEPT { curl_easy_cleanup(raw); }
    operator CURL*() LibCurl_NOEXCEPT { return raw; }
    CURL* raw;

private:
    Easy(const Easy&);
    Easy& operator=(const Easy&);
};

typedef CURLcode Code;
}

#endif
