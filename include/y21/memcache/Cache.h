#ifndef __MEMCACHE_CACHE_H__
#define __MEMCACHE_CACHE_H__

#include <map>
#include <string>
#include <stdexcept>
#include <time.h>

namespace memcache {
    template<typename T>
    class Cache {
        private:
            std::map<std::string, T> internal_cache;
            uint16_t cacheLimit;
            bool ignoreQuery;
            time_t expires;
        public:
            Cache(uint16_t cacheLimit, bool ignoreQuery, time_t expires): cacheLimit(cacheLimit), ignoreQuery(ignoreQuery), expires(expires) {

            }

            bool has(std::string key) {
                return this->internal_cache.count(key) > 0;
            }

            T& add(std::string key, T val) {
                if (this->cacheLimit == 0 || this->internal_cache.size() <= this->cacheLimit) {
                    this->internal_cache[key] = val;
                } else {
                    throw new std::length_error("Cache limit exceeded");
                }
            }

            static std::string removeQuery(std::string query) {
                for (int i = 0; i < query.length(); ++i)
                    if (query.at(i) == '?')
                        return query.substr(0, i);
            }
    };
}

#endif