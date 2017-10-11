#include <bison_objects.hpp>

#include <iostream>

#define TEST(X)\
if ( X ) std::cout << "=      " << __func__ << " OK " << std::endl;\
else  { \
    std::cout << "=      " << __func__ << " KO " << std::endl;\
    exit(1);\
}

void accessor_int32() {
    std::vector<char> int_v = {'i','n','t','3','2','\x00','\x01','\x00','\x00','\x00'};
    vect_it buff = int_v.begin();

    BsonInt32 bson_int(buff);

    std::string name = bson_int.name();
    int32_t value = bson_int.get();
    TEST( !name.compare("int32") && value  == 1 );
}

void accessor_int64() {
    std::vector<char> int_v = {'i','n','t','6','4','\x00','\x03','\x00','\x00','\x00','\x00','\x00','\x00','\x00'};
    vect_it buff = int_v.begin();

    BsonInt64 bson_int(buff);

    std::string name = bson_int.name();
    int64_t value = bson_int.get();
    TEST( !name.compare("int64") && value  == 3 );
}

void accessor_uint64() {
    std::vector<char> int_v = {'u','i','n','t','6','4','\x00','\x00','\x00','\x00','\x00','\x00','\x02','\x00','\x00'};
    vect_it buff = int_v.begin();

    BsonUint64 bson_int(buff);

    std::string name = bson_int.name();
    uint64_t value = bson_int.get();
    TEST( !name.compare("uint64") && value  == (uint64_t)2199023255552 );
}

void accessor_string() {
    std::vector<char> int_v = {'s','t','r','i','n','g','\x00','\x07','\x00','\x00','\x00','s','t','r','i','n','g','\x00'};
    vect_it buff = int_v.begin();
 
    BsonString bson_string(buff);

    std::string name = bson_string.name();
    std::string value = bson_string.get();
    TEST( !name.compare("string") && !name.compare("string") );
}


void accessor_cstring() {
    std::vector<char> int_v = {'s','t','r','i','n','g','\x00','s','t','r','i','n','g','\x00'};
    vect_it buff = int_v.begin();
 
    BsonCString bson_string(buff);

    std::string name = bson_string.name();
    std::string value = bson_string.get();
    TEST( !name.compare("string") && !value.compare("string") );
}

void accessor_doc() {
    std::vector<char> int_v = {'\x16','\x00','\x00','\x00','\x02','h','e','l','l','o','\x00',\
    '\x06','\x00','\x00','\x00','w','o','r','l','d','\x00','\x00'};
    vect_it buff = int_v.begin();
    BsonDoc doc(buff, int_v.size());

    std::unique_ptr<BsonObj> obj = doc.next();

    std::string name;
    std::string value;

    if (obj->type() == Bson_type::STRING) {
        auto bson_string = dynamic_cast<BsonString&>(*obj);
        name = bson_string.name();
        value = bson_string.get();
    }

    TEST( !name.compare("hello") && !value.compare("world") );

}
int main(){
    accessor_int32();
    accessor_int64();
    accessor_uint64();
    accessor_string();
    accessor_cstring();
    
    accessor_doc();
}