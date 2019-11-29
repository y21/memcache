#ifndef __MEMCACHE_CACHE_H__
#define __MEMCACHE_CACHE_H__

#include <map>
#include <string>
#include <stdexcept>
#include <time.h>

namespace memcache {
    template<typename T>
    struct CacheEntry {
        time_t cachedAt;
        T value;

        CacheEntry(T val): cachedAt(time(NULL)), value(val) {

        }
        CacheEntry(): cachedAt(time(NULL)) {

        }
    };

    template<typename T>
    class Cache {
        private:
            std::map<std::string, CacheEntry<T>> internal_cache;
            uint16_t cacheLimit;
            bool ignoreQuery;
            time_t expires;
        public:
            Cache(uint16_t cacheLimit, bool ignoreQuery, time_t expires): cacheLimit(cacheLimit), ignoreQuery(ignoreQuery), expires(expires) {

            }

            bool has(std::string key) {
                return this->internal_cache.count(key) > 0;
            }

            CacheEntry<T> add(std::string key, T val) {
                std::string rKey = this->ignoreQuery ? this->removeQuery(key) : key;
                if (this->cacheLimit == 0 || this->internal_cache.size() <= this->cacheLimit) {
                    CacheEntry<T> temp(val);
                    return this->internal_cache[rKey] = temp;
                } else {
                    throw new std::length_error("Cache limit exceeded");
                }
            }

            CacheEntry<T>& get(std::string key) {
                std::string rKey = this->ignoreQuery ? this->removeQuery(key) : key;
                return this->internal_cache[rKey];
            }

            CacheEntry<T>& operator[](std::string key) {
                return this->get(key);
            }

            static std::string removeQuery(std::string query) {
                for (int i = 0; i < query.length(); ++i)
                    if (query.at(i) == '?')
                        return query.substr(0, i);
                return query;
            }
            
            bool expired(CacheEntry<T>& e) {
                return time(NULL) - e.cachedAt > this->expires;
            }
    };
}

#endif