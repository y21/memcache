// memcache
// Copyright (c) 2019 y21 - All rights reserved
// MIT License
#ifndef __MEMCACHE_CACHE_H__
#define __MEMCACHE_CACHE_H__

#include <map>
#include <string>
#include <stdexcept>
#include <time.h>

/**
 * Namespace for this library
 *
 * Everything this library offers can be found in this namespace.
 */
namespace memcache {

    /**
     * This struct is used to represent entries in a cache
     *
     * It is mainly used inside of the Cache class, such as when calling `get` on a Cache object.
     * Both constructors set `cachedAt` to the current time using time(NULL).
     */
    template<typename T>
    struct CacheEntry {
        /// @brief The timestamp of when this object was created
        time_t cachedAt;
        /// @brief The value this cache entry holds (its type is specified as template argument)
        T value;

        /**
         * Constructor that sets the value of this cache entry
         *
         * @param val The value of this entry
         */
        CacheEntry(T val): cachedAt(time(NULL)), value(val) {

        }

        /**
         * Default constructor for this class that only sets the `cachedAt` timestamp
         */
        CacheEntry(): cachedAt(time(NULL)) {

        }
    };

    /**
     * This class represents a cache
     *
     * Cache entries will internally be stored in a map. 
     * It is keyed by routes and map values are CacheEntry objects that hold the actual cached value.
     */
    template<typename T>
    class Cache {
        private:
            /// @brief The actual map that holds all values, stored inside of a map to increase performance
            std::map<std::string, CacheEntry<T>> internal_cache;
            /// @brief The maximum number of cache elements this cache is able to hold
            /// Adding an element when the cache limit has been exceeded will result in std::length_error being thrown
            uint16_t cacheLimit;
            /// @brief Whether query parameters (?key=value) should be ignored
            bool ignoreQuery;
            /// @brief For how long each entry should be cached (in seconds)
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
                CacheEntry<T>& entry = this->internal_cache[rKey];
                if (this->expired(entry)) {
                    throw new std::runtime_error("Requested cache entry is no longer available due to cache limit");
                } else {
                    return entry;
                }
            }

            CacheEntry<T>& operator[](std::string key) {
                return this->get(key);
            }

            static std::string removeQuery(std::string query) {
                for (int i = 0; i < query.length(); ++i) {
                    if (query.at(i) == '?') {
                        return query.substr(0, i);
                    }
                }
                return query;
            }
            
            bool expired(CacheEntry<T>& e) {
                return time(NULL) - e.cachedAt > this->expires;
            }
    };
}

#endif