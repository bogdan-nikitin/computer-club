#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <string_view>

namespace util {
struct string_hash {
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    std::size_t operator()(const char* str) const;
    std::size_t operator()(std::string_view str) const;
    std::size_t operator()(const std::string& str) const;
};
} // namespace util
#endif
