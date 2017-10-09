#include <bison_objects.hpp>

#include <iostream>

#define TEST(X)\
if ( X ) std::cout << "=      " << __func__ << " OK " << std::endl;\
else  { \
    std::cout << "=      " << __func__ << " KO " << std::endl;\
    exit(1);\
}

void accessor_int() {
    std::vector<char> int_v = {'i','n','t','\x00','\x01','\x00','\x00','\x00'};
    vect_it buff = int_v.begin();

    BsonInt32 bson_int(buff);

    std::string name = bson_int.name();
    int32_t value = bson_int.get();
    TEST( !name.compare("int") && value  == 1 );
}

void accessor_string() {
    std::vector<char> int_v = {'s','t','r','i','n','g','\x00','\x07','\x00','\x00','\x00','s','t','r','i','n','g','\x00'};
    vect_it buff = int_v.begin();
 
    BsonString bson_string(buff);

    std::string name = bson_string.name();
    std::string value = bson_string.get();
    TEST( !name.compare("string") && !name.compare("string") );
}

int main(){
    accessor_int();
    accessor_string();
}