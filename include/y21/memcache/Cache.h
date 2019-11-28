#ifndef __MEMCACHE_CACHE_H__
#define __MEMCACHE_CACHE_H__

#include <map>
#include <string>

namespace memcache {
    template<typename T>
    class Cache {
        private:
            std::map<std::string, T> internal_cache;
            uint16_t cacheLimit;
            bool ignoreQuery;
        public:
            Cache(uint16_t cacheLimit, bool ignoreQuery): cacheLimit(cacheLimit), ignoreQuery: ignoreQuery {

            }

            bool has(std::string key) {
                return this->internal_cache::count(key) > 0;
            }
    };
}

#endif