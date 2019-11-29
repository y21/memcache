#include <y21/memcache/Cache.h>
#include <iostream>

using namespace memcache;

int main() {
    Cache<int> a(4, true, 10000);
    a.add("/bruh", 1);
    a.add("/aa", 2);
    a.add("/bb", 3);
    std::cout << a.get("/bruh?q=1") << std::endl << a.get("/bruh");
}