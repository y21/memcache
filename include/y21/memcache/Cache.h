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
            /**
             * Constructor for creating cache objects
             *
             * @param cacheLimit The maximum number of cache elements this cache is able to hold
             * @param ignoreQuery Whether query parameters (?key=value) should be ignored
             * @param expires For how long each entry should be cached (in seconds)
             */
            Cache(uint16_t cacheLimit, bool ignoreQuery, time_t expires): cacheLimit(cacheLimit), ignoreQuery(ignoreQuery), expires(expires) {

            }

            /**
             * Checks whether a given key is cached
             *
             * @returns Whether a key is in cache
             */
            bool has(std::string key) {
                return this->internal_cache.count(key) > 0;
            }

            /**
             * Adds an element to the cache
             *
             * You must not create a CacheEntry object yourself and pass it as parameter.
             * Instead, you simply pass the key and value. This library will automatically create a cache entry and stores it
             * @param key The key of this element
             * @param val The value of this element
             * @exception length_error Cache limit exceeded
             * @returns A reference to the CacheEntry object that is stored in cache
             */
            CacheEntry<T> add(std::string key, T val) {
                std::string rKey = this->ignoreQuery ? this->removeQuery(key) : key;
                if (this->cacheLimit == 0 || this->internal_cache.size() <= this->cacheLimit) {
                    CacheEntry<T> temp(val);
                    return this->internal_cache[rKey] = temp;
                } else {
                    throw new std::length_error("Cache limit exceeded");
                }
            }

            /**
             * Gets an element by its key
             *
             * @exception runtime_error Cache entry expired
             * @param key The key to search for
             * @returns A reference to the Cache Entry object
             */
            CacheEntry<T>& get(std::string key) {
                std::string rKey = this->ignoreQuery ? this->removeQuery(key) : key;
                CacheEntry<T>& entry = this->internal_cache[rKey];
                if (this->expired(entry)) {
                    throw new std::runtime_error("Requested cache entry is no longer available due to cache limit");
                } else {
                    return entry;
                }
            }

            /**
             * Same as `get()`: gets an element by its key
             *
             * Overloaded operator[] for convenience
             * @param key The key to search for
             * @returns A reference to the Cache Entry object
             */
            CacheEntry<T>& operator[](std::string key) {
                return this->get(key);
            }

            /**
             * Removes query parameters off of a string
             *
             * This is used internally to ignore queries if `this->ignoreQuery` is set to true
             * @param query The string to operate on
             * @returns A new string without query parameters
             */
            static std::string removeQuery(std::string query) {
                for (int i = 0; i < query.length(); ++i) {
                    if (query.at(i) == '?') {
                        return query.substr(0, i);
                    }
                }
                return query;
            }
            
            /**
             * Checks whether a cache element has expired or is still valid
             *
             * If you are unsure whether an entry is still cached or has already been removed from the cache, use this method before using `get` or `operator[]`.
             * @param e The CacheEntry object that should be checked
             * @returns Whether it is still valid
             */
            bool expired(CacheEntry<T>& e) {
                return time(NULL) - e.cachedAt > this->expires;
            }
    };
}

#endif