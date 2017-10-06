#include "bison.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <ctime>

#include <iostream>

//here we use char* reference to move through the pointers
//once the buff is read the pointer is incremented for the next read
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
double read<double>(char*& c, int32_t) {
    double v = static_cast<double>(*c);
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
        char* start = vect.data();
        char* buff = start;
        BsonObj* current = obj;
        //document size \x00
        int32_t size = read<int32_t>(buff);

        if (size > vect.size()){
            std::cerr << "fatal incomplete bson file" << std::endl;
            exit(1);
        } 

        //read first
        obj = new BsonMap(buff);

        if (obj == nullptr) {
            std::cerr << "fatal could not read bson file" << std::endl;
            exit(1);
        }

        //read more if needed
        while ( buff-start < vect.size())
        {
            current = new BsonMap(buff);
            current = current->next();
        }
};

BsonObj* Bson::next()
{
    if (obj == nullptr) {
        std::cerr << "fatal Bson object not initialized" << std::endl;
        exit(1);
    }
    return obj;
}

BsonObj* BsonObj::next()
{
    if (obj == nullptr) {
        std::cerr << "fatal BsonObj object not initialized" << std::endl;
        exit(1);
    }
    return obj;
}

BsonArray::BsonArray(char*& buff) {

};

std::string BsonArray::dump() {
    return "not implemented yet";
};

BsonMap::BsonMap(char*& buff) {
    BSON_TYPE type = static_cast<BSON_TYPE>(read<char>(buff));
    while ( type != BSON_TYPE::DOC )
    {
        std::string str = read<std::string>(buff);
        keys.emplace_back(str);
        switch (type) {
            case BSON_TYPE::DOUBLE : {
                values.push_back(new BsonDouble(buff));
                break;
            }
            case BSON_TYPE::STRING : {
                int32_t size = read<int32_t>(buff);
                //fix -1
                BsonString* s = new BsonString(buff,-1);
                if (size != s->get().size()+1) {
                    std::cerr << "fatal wrong string size" << std::endl;
                    exit(1);
                }
                values.push_back(s);
                break;
            }
            case BSON_TYPE::INT32   : {
                values.push_back(new BsonInt32(buff));
                break;
            }
            case BSON_TYPE::INT64   : {
                values.push_back(new BsonInt64(buff));
                break;
            }
            case BSON_TYPE::UINT64   : {
                values.push_back(new BsonUint64(buff));
                break;
            }
            case BSON_TYPE::TIME    : {
                values.push_back(new BsonTime(buff));
                break;
            }
            case BSON_TYPE::OBJ_ID  : {
                BsonObjID* id = new BsonObjID(buff);
                std::cout << id->dump() << std::endl;
                break;
            }
            case BSON_TYPE::UNDEF :{
                break;
            }
            case BSON_TYPE::NULL_VALUE : {
                break;
            }
            default : {
                //TEMPORARY WHILE CODING
                std::cerr << "fatal type:" << std::hex << (int) type << " not implemented"  << std::endl;
                exit(1);
            }
        }
        type = static_cast<BSON_TYPE>(read<char>(buff));
    }
};

std::string BsonMap::dump(){
    std::string s;
    s+="{\n";
    if(keys.size() != values.size()){
        std::cerr << "fatal error map size is invalid" << std::endl;
        exit(1);
    }
    for (int i=0; i<keys.size(); i++) {
        s.append("\"");
        s.append(keys[i]);
        s.append("\" : ");
        s.append(values[i]->dump());
        s.append(" , ");
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

BsonInt32::BsonInt32(char*& buff) {
    val = read<int32_t>(buff);
}

std::string BsonInt32::dump() {
    return std::to_string(val);
}

int32_t BsonInt32::get() {
    return val;
}

BsonInt64::BsonInt64(char*& buff){
    val = read<int64_t>(buff);
}

std::string BsonInt64::dump() {
    return std::to_string(val);
}

int64_t BsonInt64::get() {
    return val;
}

BsonUint64::BsonUint64(char*& buff){
    val = read<uint64_t>(buff);
}

std::string BsonUint64::dump() {
    return std::to_string(val);
}

uint64_t BsonUint64::get() {
    return val;
}


BsonDouble::BsonDouble(char*& buff){
    val = read<double>(buff);
}

std::string BsonDouble::dump() {
    return std::to_string(val);
}

double BsonDouble::get() {
    return val;
}

BsonTime::BsonTime(char*& buff){
    val = std::time_t(read<int64_t>(buff));
}

std::string BsonTime::dump() {
    return std::string(std::asctime(std::gmtime(&val)));
}

std::time_t BsonTime::get() {
    return val;
}

BsonObjID::BsonObjID(char*& buff){
    for (int i = 0 ; i<fixed_len ; i++){
        val.emplace_back(read<char>(buff));
    }
}

std::string BsonObjID::dump(){
    std::stringstream ss;
    for (char c: val) {
        ss << std::hex << (int) static_cast<unsigned char>(c);
    }
    return ss.str();
}
