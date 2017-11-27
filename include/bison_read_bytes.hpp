#include <string>
#include <iostream>
#include <vector>
#include <cstdint>

using vect_it = std::vector<char>::const_iterator;


//this is a hack to deduce the type using implicit conversion
struct type_converter {
vect_it& it;

template<typename T, typename = std::enable_if<std::is_fundamental<T>::value>>
	operator T() {
    T v;
    char* p = (char*)&v;
    for (size_t i=0; i < sizeof(T); i++) {
        *(p+i) = *(it++);
    }
    return v;
}
};
	type_converter read(vect_it & it) {
	//here buffer is implicitly converted to T type using the operator T()
	return {it};
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
        int32_t local_size = read(c);
        init = c;
        c += local_size;
        size = local_size;
    }
}

std::vector<unsigned char> read_hex(vect_it& c, int size) {
    std::vector<unsigned char> vect;
    while(size > 0) {
        vect.push_back(read(c));
        size--;
    }
    return vect;
}
