#include <string>
#include <iostream>
#include <vector>

using vect_it = std::vector<char>::const_iterator;


//here we use vect_it reference to move through the pointers
template<typename T, typename = std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>>
T read(vect_it& it) {
    T v;
    char* p = (char*)&v;
    for (size_t i=0; i < sizeof(T); i++) {
        *(p+i) = *(it++);
    }
    return v;
}

void read_string(vect_it& c, vect_it& init, int32_t& size, bool has_size = false);
std::vector<unsigned char> read_hex(vect_it& c, int size);

void read_string(vect_it& c, vect_it& init, int32_t& size, bool has_size) {
    
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
}

std::vector<unsigned char> read_hex(vect_it& c, int size) {
    std::vector<unsigned char> vect;
    while(size > 0) {
        vect.push_back(read<unsigned char>(c));
        size--;
    }
    return vect;
}
