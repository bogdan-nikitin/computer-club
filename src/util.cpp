#include "util.h"

std::size_t util::string_hash::operator()(const char* str) const { 
    return hash_type{}(str); 
}
std::size_t util::string_hash::operator()(std::string_view str) const { 
    return hash_type{}(str); 
}
std::size_t util::string_hash::operator()(const std::string& str) const { 
    return hash_type{}(str); 
}
