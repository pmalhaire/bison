#ifndef BISON_OBJECTS_HPP
#define BISON_OBJECTS_HPP

#include <string>
#include <ctime>
#include <vector>

#include "bison_types.hpp"

class BsonObj {
public:
    BsonObj(){};                        // use for document (not embeded)
    BsonObj(char*& buff);               // compute the name at construction
    virtual ~BsonObj(){};
    BSON_TYPE type;                     // get the type needed to access the real type
    std::string name;                   // name of the field or "" for the initial document
    //todo test with bigger files
    virtual std::string dump() = 0;     // use to get a human readable string representing the object                 
    BsonObj* next();
    void setNext(BsonObj* obj);
    std::string dump_one(std::string);  // small helper for type with one value
    static BsonObj* Parse(char*& buff); // factory used to create objects
private:
    BsonObj* _next = nullptr;
};

class BsonDoc : public BsonObj{
public:
    BsonDoc() = delete;
    BsonDoc(char*& buff, size_t buff_size);
    BsonDoc(char*& buff);
    ~BsonDoc();
    BSON_TYPE type = BSON_TYPE::DOC;
    BsonObj* get();
    std::string dump();
private:
    BsonObj* _obj = nullptr;
};

class BsonJsCodeWC : public BsonObj{
public:
    BsonJsCodeWC(){};                        
    BsonJsCodeWC(char*& buff);
    ~BsonJsCodeWC();
    BSON_TYPE type = BSON_TYPE::JS_CODE;
    std::string dump();
    int32_t getLength();
    std::string& getCode();
    BsonDoc* getDoc();          
private:
    int32_t _length;
    std::string _code;
    BsonDoc* _doc = nullptr;
};

class BsonArr : public BsonDoc{
public:
    BsonArr() = delete;
    BsonArr(char*& buff);
    BSON_TYPE type = BSON_TYPE::ARR;
    std::string dump();
};

class BsonNull: public BsonObj{
public:
    BsonNull() = delete;
    BsonNull(char*& buff);
    BSON_TYPE type = BSON_TYPE::NULL_VALUE;
    std::string dump();
};

class BsonUndef: public BsonObj{
public:
    BsonUndef() = delete;
    BsonUndef(char*& buff);
    BSON_TYPE type = BSON_TYPE::UNDEF;
    std::string dump();
};

class BsonMinKey: public BsonObj{
public:
    BsonMinKey() = delete;
    BsonMinKey(char*& buff);
    BSON_TYPE type = BSON_TYPE::MIN_KEY;
    std::string dump();
};

class BsonMaxKey: public BsonObj{
public:
    BsonMaxKey() = delete;
    BsonMaxKey(char*& buff);
    BSON_TYPE type = BSON_TYPE::MAX_KEY;
    std::string dump();
};

class BsonBool: public BsonObj{
public:
    BsonBool() = delete;
    BsonBool(char*& buff);
    BSON_TYPE type = BSON_TYPE::BOOL;
    std::string dump();
    bool get();
private:
    bool _val;

};

class BsonInt32: public BsonObj{
public:
    BsonInt32() = delete;
    BsonInt32(char*& buff);
    BSON_TYPE type = BSON_TYPE::INT32;
    std::string dump();
    int32_t get();
private:
    int32_t _val;

};

class BsonInt64: public BsonObj{
public:
    BsonInt64() = delete;
    BsonInt64(char*& buff);
    BSON_TYPE type = BSON_TYPE::INT64;
    std::string dump();
    int64_t get();
private:
    int64_t _val;
};

class BsonUint64: public BsonObj{
public:
    BsonUint64() = delete;
    BsonUint64(char*& buff);
    BSON_TYPE type = BSON_TYPE::INT64;
    std::string dump();
    uint64_t get();
private:
    uint64_t _val;
};

class BsonDouble: public BsonObj{
public:
    BsonDouble() = delete;
    BsonDouble(char*& buff);
    BSON_TYPE type = BSON_TYPE::DOUBLE;
    std::string dump();
    double get();
private:
    double _val;
};

class BsonString: public BsonObj{
public:
    BsonString() = delete;
    BsonString(char*& buff);
    BSON_TYPE type = BSON_TYPE::STRING;
    std::string dump();
    std::string get();
private:
    std::string _str;
};

class BsonSymbol: public BsonString{
public:
    BsonSymbol(char*& buff):BsonString(buff){}
    BSON_TYPE type = BSON_TYPE::SYMBOL;
};

class BsonJsCode: public BsonString{
public:
    BsonJsCode(char*& buff):BsonString(buff){}
    BSON_TYPE type = BSON_TYPE::JS_CODE;
};

class BsonCString: public BsonObj{
public:
    BsonCString() = delete;
    BsonCString(char*& buff);
    BSON_TYPE type = BSON_TYPE::CSTRING;
    std::string dump();
    std::string get();
private:
    std::string _str;
};

class BsonTime: public BsonObj{
public:
    BsonTime() = delete;
    BsonTime(char*& buff);
    BSON_TYPE type = BSON_TYPE::TIME;
    std::string dump();
    std::time_t get();
private:
    std::time_t _val;
};

class BsonID: public BsonObj{
public:
    BsonID() = delete;
    BsonID(char*& buff);
    BSON_TYPE type = BSON_TYPE::OBJ_ID;
    std::string dump();
    const int fixed_len = 12;
    const std::vector<unsigned char>& get();
private:
    std::vector<unsigned char> _val;
};

class BsonDBPointer: public BsonObj{
public:
    BsonDBPointer() = delete;
    BsonDBPointer(char*& buff);
    BSON_TYPE type = BSON_TYPE::DBPOINTER;
    std::string dump();
    const int fixed_len = 12;
    const std::vector<unsigned char>& get();
    const std::string& getString();
private:
    std::vector<unsigned char> _val;
    std::string _string;
};


class BsonDec128: public BsonID{
public:
    BsonDec128() = delete;
    BsonDec128(char*& buff);
    BSON_TYPE type = BSON_TYPE::DEC128;
    const int fixed_len = 16;
};

class BsonBin: public BsonObj{
public:
    BsonBin() = delete;
    BsonBin(char*& buff);
    BSON_TYPE type = BSON_TYPE::BIN;

    std::string dump();
    const std::vector<unsigned char>& get();
    int32_t size(); //size is stored as int32_t
    unsigned char subtype();    
private:
    std::vector<unsigned char> _val;
    unsigned char _subtype;
};


#endif // ndef BISON_OBJECTS_HPP