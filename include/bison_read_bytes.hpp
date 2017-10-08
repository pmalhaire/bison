#include <string>
#include <iostream>
#include <vector>

//here we use char* reference to move through the pointers
//once the buff is read the pointer is incremented for the next read
template<typename T>
T read(char*& c) {
    return T();
};

template<>
char read<char>(char*& c) {
    char v = *c;
    c++;
    return v;
};

template<>
unsigned char read<unsigned char>(char*& c) {
    unsigned char v = *reinterpret_cast<unsigned char*>(c);
    c++;
    return v;
};


//taking a bit of risks here we expect the machine to be little endian
//TODO add compile time check of endianest
template <>
int32_t read<int32_t>(char*& c) {
    int32_t v = *reinterpret_cast<int32_t*>(c);
    c+=4;
    return v;
};

template <>
int64_t read<int64_t>(char*& c) {
    int64_t v = *reinterpret_cast<int64_t*>(c);
    c+= 8;
    return v;
};

template <>
uint64_t read<uint64_t>(char*& c) {
    uint64_t v = *reinterpret_cast<uint64_t*>(c);
    c+= 8;
    return v;
};

template <>
double read<double>(char*& c) {
    double v = *reinterpret_cast<double*>(c);
    c+= 8;
    return v;
};

void read_string(char*& c, char *& init, int32_t& size, bool has_size = false) {
    
    if (!has_size) {
        init = c;
        while(*c != '\0'){
            c++;
        }
        c++;
        size = c - init;
    } else {
        //size including '\0' end char
        int32_t local_size = read<int32_t>(c);
        init = c;
        c += local_size;
        size = local_size;
    }
};

std::vector<unsigned char> read_hex(char*& c, int size) {
    std::vector<unsigned char> vect;
    while(size > 0) {
        vect.push_back(read<unsigned char>(c));
        size--;
    }
    return vect;
};