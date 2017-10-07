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

std::string read_string(char*& c, bool has_size = false) {
    std::string str;
    if (!has_size) {
        str = std::string(c);
        c += str.size() + 1; //string size + \0
    } else {
        //size including '\0' end char
        int32_t size = read<int32_t>(c);
        str = std::string(c, size-1);
        //std::string gives the size without '\0' so size-1
        if ( (size-1) != str.size() ) {
            std::cerr << "fatal wrong string size for \""<< str 
            <<"\" expected:" << size << " got:"<< str.size()+1 << std::endl;
            exit(1);
        } 
        c += size-1;
        BSON_TYPE type = static_cast<BSON_TYPE>(read<char>(c));
        if (type != BSON_TYPE::DOC ) {
            std::cerr << "fatal invalid string for " << std::endl 
            <<"\""<< str << "\"" << std::endl << "size" << size
            <<" missing '\\0' end char:" << std::hex << (int) type << std::endl;
            exit(1);
        }
    }
    return str;
};

std::vector<unsigned char> read_hex(char*& c, int size) {
    std::vector<unsigned char> vect;
    while(size > 0) {
        vect.emplace_back(read<unsigned char>(c));
        size--;
    }
    return vect;
};