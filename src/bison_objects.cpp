#include "bison_types.hpp"
#include "bison_read_bytes.hpp"
#include "bison_objects.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

BsonObj* BsonObj::Parse(char*& buff) {
    BSON_TYPE type = static_cast<BSON_TYPE>(read<char>(buff));

    switch (type) {
        case BSON_TYPE::DOC         : return nullptr; //00 indicates the end of the document
        //DOC is not present here since we are in obj
        case BSON_TYPE::DOUBLE      : return new BsonDouble(buff);
        case BSON_TYPE::STRING      : return new BsonString(buff);
        case BSON_TYPE::EMB_DOC     : return new BsonDoc(buff);
        //here we can make it as an actual array
        case BSON_TYPE::ARR         : return new BsonDoc(buff);
        case BSON_TYPE::BIN         : {
            //TEMPORARY WHILE CODING
            std::cerr << "TODO implement BIN"  << std::endl;
            exit(1);
        }
        case BSON_TYPE::UNDEF       : return new BsonUndef(buff);
        case BSON_TYPE::OBJ_ID      : return new BsonObjID(buff);
        case BSON_TYPE::BOOL        : return new BsonBool(buff);
        case BSON_TYPE::TIME        : return new BsonTime(buff);
        case BSON_TYPE::NULL_VALUE  : return new BsonNull(buff);
        case BSON_TYPE::CSTRING     : {
            //TEMPORARY WHILE CODING
            std::cerr << "TODO implement CSTRING"  << std::endl;
            exit(1);
        }
        case BSON_TYPE::DBPOINTER   : {
            //TEMPORARY WHILE CODING
            std::cerr << "TODO implement DBPOINTER"  << std::endl;
            exit(1);
        }
        case BSON_TYPE::JS_CODE     : {
            //TEMPORARY WHILE CODING
            std::cerr << "TODO implement JS_CODE"  << std::endl;
            exit(1);
        }
        case BSON_TYPE::SYMBOL      : {
            //TEMPORARY WHILE CODING
            std::cerr << "TODO implement SYMBOL"  << std::endl;
            exit(1);
        }
        case BSON_TYPE::JS_S_CODE   : {
            //TEMPORARY WHILE CODING
            std::cerr << "TODO implement JS_S_CODE"  << std::endl;
            exit(1);
        }
        case BSON_TYPE::INT32       : return new BsonInt32(buff);
        case BSON_TYPE::UINT64      : return new BsonUint64(buff);
        case BSON_TYPE::INT64       : return new BsonInt64(buff);
        case BSON_TYPE::DEC128      : {
            //TEMPORARY WHILE CODING
            std::cerr << "TODO implement DEC128"  << std::endl;
            exit(1);
        }
        case BSON_TYPE::MIN_KEY     : return new BsonMinKey(buff);
        case BSON_TYPE::MAX_KEY     : return new BsonMaxKey(buff);
    }
    std::cerr << "fatal type 0x" << std::hex << std::setfill('0') << std::setw(2) 
    << (int) type << " is not in specification";
    exit(1);
    return nullptr;
}


BsonObj::BsonObj(char*& buff){
    name = read_string(buff);
}

BsonObj* BsonObj::next(){
    return _next;
}

void BsonObj::setNext(BsonObj* obj){
    _next = obj;
}

std::string BsonObj::dump_one(std::string str){
    return "\""+name+"\" : "+str;
}

BsonString::BsonString(char*& buff):BsonObj(buff) {
    _str = read_string(buff,true);
}

std::string BsonString::dump() {
    return dump_one("\""+_str+"\"");
}

std::string BsonString::get() {
    return _str;
}


BsonDoc::BsonDoc(char*& buff, size_t buff_size) {
    char* start = buff;
    BsonObj* current = nullptr;
    //document size

    int32_t size = read<int32_t>(buff);

    if (size > buff_size){
        std::cerr << "fatal incomplete bson file" 
        << " needed size " << size
        << " buffer size " << buff_size
        << std::endl;
        exit(1);
    } 

    //read first
    _obj = BsonObj::Parse(buff);

    if (_obj == nullptr) {
        std::cerr << "fatal could not read bson document" << std::endl;
        exit(1);
    }

    current = _obj;
    //read more if needed
    while ( buff-start < buff_size )
    {
        BsonObj* temp = BsonObj::Parse(buff);
        
        
        if ( temp == nullptr) {
            //end of document but there is maybe an other document
            if (buff-start < buff_size) {
                BsonDoc* doc = new BsonDoc(buff,buff_size-(buff-start));
                if (doc == nullptr){
                    std::cerr << "fatal could not read document" << std::endl;
                    exit(1);
                }
                setNext(doc);
            }
            
            break;
        }
        current->setNext(temp);
        current = temp;
    }
}

BsonDoc::BsonDoc(char*& buff):BsonObj(buff) {
    char* start = buff;
    BsonObj* current = nullptr;
    //document size

    int32_t size = read<int32_t>(buff);

    //read first
    _obj = BsonObj::Parse(buff);

    if (_obj == nullptr) {
        std::cerr << "fatal could not read bson document" << std::endl;
        exit(1);
    }

    current = _obj;
    //read more if needed
    while ( buff-start<size )
    {
        BsonObj* temp = BsonObj::Parse(buff);
        if ( temp == nullptr) { 
            break;
        }
        current->setNext(temp);
        current = temp;
    }
}

BsonDoc::~BsonDoc(){
    std::vector<BsonObj*> toDelete;
    BsonObj* obj = get();
    toDelete.push_back(obj);
    while( (obj=obj->next()) ) {
        toDelete.push_back(obj);
    }

    for (const BsonObj* d: toDelete) {
        delete d;
    }
}

std::string BsonDoc::dump() {
    std::string str;
    //case of an embeded document
    //TODO check this means a null name won't work
    //there may be a bug
    if (name.size() > 0 ) {
        str += "\""+name+"\" : \n";
    }
    str += "{\n";
    BsonObj* obj = get();
    str += obj->dump();
    while( (obj=obj->next()) ) 
    {
        str.append(",\n");
        str.append(obj->dump());
    }
    str.append("\n}\n");
    return str;
}

BsonObj* BsonDoc::get(){
    return _obj;
}

BsonNull::BsonNull(char*& buff):BsonObj(buff) {}

std::string BsonNull::dump() {
    return dump_one("null");
}

BsonUndef::BsonUndef(char*& buff):BsonObj(buff) {}

std::string BsonUndef::dump() {
    return dump_one("undef");
}

BsonMinKey::BsonMinKey(char*& buff):BsonObj(buff) {}

std::string BsonMinKey::dump() {
    return dump_one("min_key");
}

BsonMaxKey::BsonMaxKey(char*& buff):BsonObj(buff) {}

std::string BsonMaxKey::dump() {
    return dump_one("max_key");
}

BsonBool::BsonBool(char*& buff):BsonObj(buff) {
    _val = read<char>(buff);
}

std::string BsonBool::dump() {
    std::string str;
    if (_val) {
        str = dump_one("true");
    } else {
        str = dump_one("false");
    }
    return str;
}

bool BsonBool::get() {
    return _val;
}

BsonInt32::BsonInt32(char*& buff):BsonObj(buff) {
    _val = read<int32_t>(buff);
}

std::string BsonInt32::dump() {
    return dump_one(std::to_string(_val));
}

int32_t BsonInt32::get() {
    return _val;
}

BsonInt64::BsonInt64(char*& buff):BsonObj(buff){
    _val = read<int64_t>(buff);
}

std::string BsonInt64::dump() {
    return dump_one(std::to_string(_val));
}

int64_t BsonInt64::get() {
    return _val;
}

BsonUint64::BsonUint64(char*& buff):BsonObj(buff){
    _val = read<uint64_t>(buff);
}

std::string BsonUint64::dump() {
    return dump_one(std::to_string(_val));
}

uint64_t BsonUint64::get() {
    return _val;
}


BsonDouble::BsonDouble(char*& buff):BsonObj(buff){
    _val = read<double>(buff);
}

std::string BsonDouble::dump() {
    return dump_one(std::to_string(_val));
}

double BsonDouble::get() {
    return _val;
}

BsonTime::BsonTime(char*& buff):BsonObj(buff){
    _val = std::time_t(read<int64_t>(buff));
}

std::string BsonTime::dump() {
    return dump_one(std::string(std::asctime(std::gmtime(&_val))));
}

std::time_t BsonTime::get() {
    return _val;
}

BsonObjID::BsonObjID(char*& buff):BsonObj(buff){
    for (int i = 0 ; i<fixed_len ; i++){
        _val.emplace_back(read<char>(buff));
    }
}

std::string BsonObjID::dump(){
    std::stringstream ss;
    for (unsigned char c: _val) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int) c;
    }
    return dump_one(ss.str());
}


