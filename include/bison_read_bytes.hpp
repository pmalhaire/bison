#include <string>
#include <iostream>
#include <vector>

using vect_it = std::vector<char>::const_iterator;

//here we use vect_it reference to move through the pointers
//once the buff is read the pointer is incremented for the next read
template<typename T>
T read(vect_it& c) {
    return T();
};

template<>
char read<char>(vect_it& c) {
    char v = *c;
    c++;
    return v;
};

template<>
unsigned char read<unsigned char>(vect_it& c) {
    unsigned char v;
    v = *c;
    c++;
    return v;
};


//taking a bit of risks here we expect the machine to be little endian
//TODO add compile time check of endianest
template <>
int32_t read<int32_t>(vect_it& c) {
    int32_t v;
    char* p = (char*)&v;
    for (int i=0; i < 4; i++) {
        *(p+i) = *(c++);
    }
    return v;
};

template <>
int64_t read<int64_t>(vect_it& c) {
    int64_t v;
    char* p = (char*)&v;
    for (int i=0; i < 8; i++) {
        *(p+i) = *(c++);
    }
    return v;
};

template <>
uint64_t read<uint64_t>(vect_it& c) {
    uint64_t v;
    char* p = (char*)&v;
    for (int i=0; i < 8; i++) {
        *(p+i) = *(c++);
    }
    return v;
};

template <>
double read<double>(vect_it& c) {
    double v;
    char* p = (char*)&v;
    for (int i=0; i < 8; i++) {
        *(p+i) = *(c++);
    }
    return v;
};

void read_string(vect_it& c, vect_it& init, int32_t& size, bool has_size = false) {
    
    if (!has_size) {
        init = c;
        while(*c != '\0'){
            c++;
        }
        c++;
        size = std::distance(init,c);
    } else {
        //size including '\0' end char
        int32_t local_size = read<int32_t>(c);
        init = c;
        c += local_size;
        size = local_size;
    }
};

std::vector<unsigned char> read_hex(vect_it& c, int size) {
    std::vector<unsigned char> vect;
    while(size > 0) {
        vect.push_back(read<unsigned char>(c));
        size--;
    }
    return vect;
};