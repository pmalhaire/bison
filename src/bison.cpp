#include "bison.hpp"

#include <string>
#include <vector>

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

template<typename T>
T read(char*& c, int32_t s = -1) {
    return T();
};

template<>
char read<char>(char*& c, int32_t s) {
    char v = *c;
    c++;
    return v;
};

template <>
int32_t read<int32_t>(char*& c, int32_t) {
    int32_t v = static_cast<int32_t>(*c);
    c+=4;
    return v;
};

template <>
int64_t read<int64_t>(char*& c, int32_t) {
    int64_t v = static_cast<int64_t>(*c);
    c+= 8;
    return v;
};

template <>
std::string read<std::string>(char*& c, int32_t s) {
    std::string str;
    if (s == -1) {
        str = std::string(c);
        c += str.size() + 1; //string size + \0
    } else {
        int32_t s = read<int32_t>(c);
        c += 4;
        str = std::string(c, s);
        c += s;
    }
    return str;
};


Bson::Bson(std::vector<char>& vect) {
        char* start = reinterpret_cast<char*>(vect.data());
        char* buff = start;
        BsonObj* current = obj;
        //document size \x00
        int32_t size = read<int32_t>(buff);

        assert(size <= vect.size() && "fatal incomplete bson file");

        //read first
        obj = new BsonMap(buff);

        assert(obj != nullptr);

        //read more if needed
        while ( buff-start < vect.size())
        {
            current = new BsonMap(buff);
            current = current->next();
        }
};

BsonObj* Bson::next()
{
    assert(obj != nullptr);
    return obj;
}

BsonObj* BsonObj::next()
{
    assert(obj != nullptr);
    return obj;
}

BsonArray::BsonArray(char*& buff) {

};

std::string BsonArray::dump() {
    return "not implemented yet";
};

BsonMap::BsonMap(char*& buff) {
    BSON::type type = static_cast<BSON::type>(read<char>(buff));
    while ( type != BSON::DOC )
    {
        keys.emplace_back(read<std::string>(buff));
        switch (type) {
            case BSON::STRING : {
                int32_t size = read<int32_t>(buff);
                BsonString* s = new BsonString(buff,-1);
                assert(size == s->get().size()+1);
                values.push_back(s);
                break;
            }
            default : {
                assert(0 && "type not supported yet");
            }
        }
        type = static_cast<BSON::type>(read<char>(buff));
    }
};

std::string BsonMap::dump(){
    std::string s;
    s+="{\n";
    assert(keys.size() == values.size());
    for (int i=0; i<keys.size(); i++) {
        s.append("\"");
        s.append(keys[i]);
        s.append("\" : ");
        s.append(values[i]->dump());
    }
    s+=("\n}\n");
    return s;
};

BsonString::BsonString(char*& buff,int32_t s){
    str = read<std::string>(buff,s);
}

std::string BsonString::dump() {
    return "\""+str+"\"";
}

std::string BsonString::get() {
    return str;
}

    
