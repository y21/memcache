# memcache
A header-only C++ HTTP caching library that stores in memory.<br/>
This can be used to store data of any size, but it is recommended to not store large amounts of data.

# Example
```cpp
using namespace memcache;
// Create cache instance
Cache<std::string> cache(0, true);

// Check if cache has route
std::string route("/test");
if (cache.has(route)) {
    // Log cached result
    std::cout << cache.get(route) << std::endl;
} else {
    // Log uncached result and add to cache
    cache.add(route, "hello");
    std::cout << "hello";
}
```