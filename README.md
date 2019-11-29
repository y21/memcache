# memcache
A header-only C++ HTTP caching library that stores in memory.<br/>
This can be used to store data of any size, but it is recommended to not store large amounts of data.

# Example usage
```cpp
#include <y21/memcache/Cache.h>
#include <string>

using std::string;
using namespace memcache;
int main() {
    Cache<string> cache(
        4, // maximum cache entries
        true, // ignores queries: "/test?q=12" gets converted to "/test" implicitly
        60 * 60 // maximum number of seconds cache entries will be valid: 1 hour in this example
    );

    // Add route /test to cache
    cache.add("/test", "<h1>hello</h1>");

    // Check if route is cached
    if (cache.has("/")) {
        // Get cached value
        CacheEntry<string> c = cache.get("/");
        std::cout << "Cached content: " << c.value;
    } else {
        // Add to cache if it is not cached yet
        cache.add("/", "<h1>now it is cached</h1>");
        std::cout << "Route cached!";
    }
}
```