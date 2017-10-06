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
    int32_t v;
    //could be done a nicer way but I had padding issues this way is working
    char* p = (char*)&v;
    for (int i=0; i < 4; i++) {
        *(p+i) = *(c+i);
    }
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
        //size including '\0' end char
        int32_t size = read<int32_t>(c);
        str = std::string(c, size-1);
        //std::string gives the size without '\0' so size-1
        if ( (size-1) != str.size() ) {
            std::cerr << "fatal wrong string size for \""<< str <<"\" expected:" << size << " got:"<< str.size()+1 << std::endl;
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

BsonObj* newBsonObj(char*& buff) {
    BSON_TYPE type = static_cast<BSON_TYPE>(read<char>(buff));
    if ( type == BSON_TYPE::DOC ) return nullptr;

    switch (type) {
        case BSON_TYPE::DOUBLE : return new BsonDouble(buff);
        case BSON_TYPE::STRING : return new BsonString(buff);
        case BSON_TYPE::INT32  : return new BsonInt32(buff);
        case BSON_TYPE::INT64  : return new BsonInt64(buff);
        case BSON_TYPE::UINT64 : return new BsonUint64(buff);
        case BSON_TYPE::TIME   : return new BsonTime(buff);
        case BSON_TYPE::OBJ_ID : return new BsonObjID(buff);

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
    return nullptr;
}

Bson::Bson(std::vector<char>& vect) {
        char* start = vect.data();
        char* buff = start;
        BsonObj* current = nullptr;
        //document size \x00
        int32_t size = read<int32_t>(buff);

        if (size > vect.size()){
            std::cerr << "fatal incomplete bson file" << std::endl;
            exit(1);
        } 

        //read first
        obj = newBsonObj(buff);

        if (obj == nullptr) {
            std::cerr << "fatal could not read bson file" << std::endl;
            exit(1);
        }

        current = obj;
        //read more if needed
        while ( buff-start < vect.size() )
        {
            BsonObj* temp = newBsonObj(buff);
            
            //end of document but there is maybe more data
            if ( temp == nullptr) {
                if (buff-start >= vect.size()) {
                    break;
                }
                int32_t size = read<int32_t>(buff);
                // size is the size of the next document
                // (buff-start) is the already parsed data
                // - 4 is the size of the size we just read from the next obj
                if ( (size + (buff-start) - 4) > vect.size()){
                    std::cerr << "fatal incomplete continuing bson file" 
                    << " needed size " << (size+(buff-start)) 
                    << " vector size " << vect.size()
                    << std::endl;
                    exit(1);
                } 
        
                continue;
            }
            current->setNext(temp);
            current = temp;
        }
};

Bson::~Bson(){
    std::vector<BsonObj*> toDelete;
    BsonObj* current = this->getObj();
    while (current!= nullptr) {
        toDelete.push_back(current);
        current = current->next();
    }
    for (BsonObj* obj: toDelete) {
        delete obj;
    }
}

std::string Bson::dump() {
    std::string str = "{\n";
    BsonObj* obj = getObj();
    str += obj->dump();
    while( (obj=obj->next()) ) 
    {
        str.append(",\n");
        str.append(obj->dump());
    }
    str.append("\n}\n");
    return str;
}

BsonObj* Bson::getObj()
{
    if (obj == nullptr) {
        std::cerr << "fatal Bson object not initialized" << std::endl;
        exit(1);
    }
    return obj;
}

void BsonObj::setNext(BsonObj* obj){
    _next = obj;
}

BsonObj* BsonObj::next()
{
    return _next;
}

BsonArray::BsonArray(char*& buff):BsonObj(buff){
    //TODO IMPLEMENT
}

std::string BsonArray::dump() {
    return "not implemented yet";
}



BsonObj::BsonObj(char*& buff){
    name = read<std::string>(buff);
}

BsonString::BsonString(char*& buff):BsonObj(buff) {
    str = read<std::string>(buff,1);
}

std::string BsonString::dump() {
    return "\""+name+"\":\""+str+"\"";
}

std::string BsonString::get() {
    return str;
}

BsonInt32::BsonInt32(char*& buff):BsonObj(buff) {
    val = read<int32_t>(buff);
}

std::string BsonInt32::dump() {
    return std::to_string(val);
}

int32_t BsonInt32::get() {
    return val;
}

BsonInt64::BsonInt64(char*& buff):BsonObj(buff){
    val = read<int64_t>(buff);
}

std::string BsonInt64::dump() {
    return std::to_string(val);
}

int64_t BsonInt64::get() {
    return val;
}

BsonUint64::BsonUint64(char*& buff):BsonObj(buff){
    val = read<uint64_t>(buff);
}

std::string BsonUint64::dump() {
    return std::to_string(val);
}

uint64_t BsonUint64::get() {
    return val;
}


BsonDouble::BsonDouble(char*& buff):BsonObj(buff){
    val = read<double>(buff);
}

std::string BsonDouble::dump() {
    return std::to_string(val);
}

double BsonDouble::get() {
    return val;
}

BsonTime::BsonTime(char*& buff):BsonObj(buff){
    val = std::time_t(read<int64_t>(buff));
}

std::string BsonTime::dump() {
    return std::string(std::asctime(std::gmtime(&val)));
}

std::time_t BsonTime::get() {
    return val;
}

BsonObjID::BsonObjID(char*& buff):BsonObj(buff){
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
