#include "bison.hpp"

#include <string>
#include <vector>

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

#include <iostream>

template<typename T>
T read(const char* c, size_t* offset, size_t s = -1) {
    return T();
};

template<>
char read<char>(const char* c, size_t* offset, size_t s) {
    *offset += 1;
    return *c;
};

template <>
int32_t read<int32_t>(const char* c, size_t* offset, size_t) {
    *offset += 4;
    return static_cast<int32_t>(*c);
};

template <>
int64_t read<int64_t>(const char* c, size_t* offset, size_t) {
            *offset += 8;
            return static_cast<int64_t>(*c);
};

template <>
std::string read<std::string>(const char* c, size_t* offset, size_t s) {
    std::string str;
    if (s == -1) {
        str = std::string(c);
        *offset += str.size() + 1; //string size + \0
    } else {
        int32_t s = read<int32_t>(c, offset);
        c+=4;
        str = std::string(c, s);
        offset += s;
    }
    return str;
};


Bson::Bson(const std::vector<char>& vect) {
        size_t offset = 0;
        const char* buff = &vect[0];
        //document size \x00
        int32_t size = read<int32_t>(buff, &offset);
        
        std::cout << size << " " << vect.size() << std::endl;
        
        /*for (auto& c : vect) {
            std::cout << c << std::hex << (int) c << std::endl;  
        }*/

        //temporary for unit tests
        assert(size == static_cast<int32_t>(vect.size()));
        std::cout << std::hex<< (int) read<char>(buff+offset,&offset) << std::endl;
        std::cout << read<std::string>(buff+offset, &offset) << std::endl;

};

BsonArray::BsonArray(char* buff) {

};

std::string BsonArray::dump() {
    
};

BsonMap::BsonMap(char* buff) {

};

std::string BsonMap::dump(){

};
    
