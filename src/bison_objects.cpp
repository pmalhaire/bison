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
        case BSON_TYPE::ARR         : return new BsonArr(buff);
        case BSON_TYPE::BIN         : return new BsonBin(buff);
        case BSON_TYPE::UNDEF       : return new BsonUndef(buff);
        case BSON_TYPE::OBJ_ID      : return new BsonID(buff);
        case BSON_TYPE::BOOL        : return new BsonBool(buff);
        case BSON_TYPE::TIME        : return new BsonTime(buff);
        case BSON_TYPE::NULL_VALUE  : return new BsonNull(buff);
        case BSON_TYPE::CSTRING     : return new BsonCString(buff);
        case BSON_TYPE::DBPOINTER   : return new BsonDBPointer(buff);
        case BSON_TYPE::JS_CODE     : return new BsonJsCode(buff);
        case BSON_TYPE::SYMBOL      : return new BsonSymbol(buff);
        case BSON_TYPE::JS_CODE_WS  : return new BsonJsCodeWC(buff);
        case BSON_TYPE::INT32       : return new BsonInt32(buff);
        case BSON_TYPE::UINT64      : return new BsonUint64(buff);
        case BSON_TYPE::INT64       : return new BsonInt64(buff);
        case BSON_TYPE::DEC128      : return new BsonDec128(buff);
        case BSON_TYPE::MIN_KEY     : return new BsonMinKey(buff);
        case BSON_TYPE::MAX_KEY     : return new BsonMaxKey(buff);
    }
    std::cerr << "fatal type 0x" << std::hex << std::setfill('0') << std::setw(2) 
    << (int) type << " is not in specification";
    exit(1);
    return nullptr;
}


BsonObj::BsonObj(char*& buff){
    read_string(buff, &_name_begin,  &_name_size, false);
}

BsonObj* BsonObj::next(){
    return _next;
}

void BsonObj::setNext(BsonObj* obj){
    _next = obj;
}

std::string BsonObj::dump_one(std::string str){
    std::string s = "\"";
    s += std::string(_name_begin, _name_size-1);
    s += "\" : ";
    s +=str;
    return s;
}

BsonString::BsonString(char*& buff):BsonObj(buff) {
    read_string(buff, &_string_begin,  &_string_size, true);
}

std::string BsonString::dump() {
    return dump_one("\""+std::string(_string_begin, _string_size-1)+"\"");
}

std::string BsonString::get() {
    return std::string(_string_begin, _string_size);
}

BsonCString::BsonCString(char*& buff):BsonObj(buff) {
    read_string(buff, &_string_begin,  &_string_size, false);
}

std::string BsonCString::dump() {
    return dump_one("\""+std::string(_string_begin, _string_size-1)+"\"");
}

std::string BsonCString::get() {
    return std::string(_string_begin, _string_size-1);
}



BsonDoc::BsonDoc(char*& buff, size_t buff_size):BsonObj() {
    _init(buff, buff_size);
}

BsonDoc::BsonDoc(char*& buff):BsonObj(buff) {
    _init(buff);
}

void BsonDoc::_init(char*& buff, size_t buff_size) {
    char* start = buff;
    BsonObj* current = nullptr;
    //document size

    int32_t size = read<int32_t>(buff);

    //check buffer size for initial documents
    if ( buff_size != -1 ) {
        if (size > buff_size){
            std::cerr << "fatal incomplete bson file" 
            << " needed size " << size
            << " buffer size " << buff_size
            << std::endl;
            exit(1);
        } 
        size = buff_size;
    }

    //read first
    _obj = BsonObj::Parse(buff);

    if (_obj == nullptr) {
        std::cerr << "fatal could not read bson document" << std::endl;
        exit(1);
    }

    current = _obj;
    //read more if needed
    while ( buff-start < size )
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
    //TODO check leaks
    BsonObj* last = get();
    BsonObj* current = last->next();
    while( current != nullptr ) {   
        current = last->next();
        delete last;
        last = current;
    }
    //delete last element if 2 elements or more are present
    if (last != nullptr) (delete last);
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

BsonArr::BsonArr(char*& buff):BsonDoc(buff) {}

std::string BsonArr::dump() {
    std::string str;
    if (name.size() > 0 ) {
        str += "\""+name+"\" : ";
    }
    str += "[ ";
    BsonObj* obj = get();
    str += obj->dump();

    while( (obj=obj->next()) ) 
    {
        str.append(",");
        str.append(obj->dump());
    }
    str.append(" ]\n");
    return str;
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
    _time = std::time_t(read<int64_t>(buff));
}

std::string BsonTime::dump() {
    return dump_one(std::string(std::asctime(std::gmtime(&_time))));
}

std::time_t BsonTime::get() {
    return _time;
}

BsonID::BsonID(char*& buff):BsonObj(buff),_val(read_hex(buff,fixed_len)){}

std::string BsonID::dump(){
    std::stringstream ss;
    for (unsigned char c: _val) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int) c;
    }
    return dump_one(ss.str());
}
const std::vector<unsigned char>& BsonID::get(){
    return _val;
}

BsonDBPointer::BsonDBPointer(char*& buff):BsonObj(buff){
    _string = read_string(buff, true);
    _val = read_hex(buff,fixed_len);
}

std::string BsonDBPointer::dump(){
    std::stringstream ss;
    ss << "\"" << _string << "\":";
    for (unsigned char c: _val) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int) c;
    }
    return dump_one(ss.str());
}

const std::vector<unsigned char>& BsonDBPointer::get(){
    return _val;
}

const std::string& BsonDBPointer::getString(){
    return _string;
}

BsonDec128::BsonDec128(char*& buff):BsonID(buff){}

BsonBin::BsonBin(char*& buff):BsonObj(buff){
    int32_t size = read<int32_t>(buff);
    //don't get why size is signed in spec 
    if ( size < 0 ) {
        std::cerr << "fatal binary object with negative size " 
        << size
        << std::endl;
        exit(1);
    }
    _subtype = read<unsigned char>(buff);
    _val = read_hex(buff,size);
}

std::string BsonBin::dump(){
    std::stringstream ss;
    ss << "bin len("<< size() << "):" <<
    std::hex << std::setfill('0') << std::setw(2) << (int) _subtype << " ";
    for (unsigned char c: _val) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int) c;
    }
    return dump_one(ss.str());
}

const std::vector<unsigned char>& BsonBin::get(){
    return _val;
}

int32_t BsonBin::size(){
    return _val.size();
}

unsigned char BsonBin::subtype(){
    return _subtype;
}

BsonJsCodeWC::BsonJsCodeWC(char*& buff):BsonObj(buff){
    _length = read<int32_t>(buff);
    _code = read_string(buff,true);
    _doc = new BsonDoc(buff);
}

BsonJsCodeWC::~BsonJsCodeWC(){
    if (_doc) delete _doc;
}

std::string BsonJsCodeWC::dump(){
    std::string s;
    s += "js_code len(";
    s += _length;
    s += ") :";
    s += _code;
    s += " ";
    if ( _doc != nullptr ) {
        s += _doc->dump();
    }
    return dump_one(s);
}

int32_t BsonJsCodeWC::getLength(){
    return _length;
}

std::string& BsonJsCodeWC::getCode(){
    return _code;
}

BsonDoc* BsonJsCodeWC::getDoc(){
    return _doc;
}       