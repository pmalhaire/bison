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
private:
    BsonObj* _next = nullptr;
};

namespace BSON{
    BsonObj* parse(char*& buff);
}

class BsonDoc : public BsonObj{
public:
    BsonDoc() = delete;
    BsonDoc(char*& buff, size_t buff_size);
    BsonDoc(char*& buff);
    ~BsonDoc();
    BsonObj* get();
    std::string dump();
private:
    BsonObj* _obj = nullptr;
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
    bool val;

};

class BsonInt32: public BsonObj{
public:
    BsonInt32() = delete;
    BsonInt32(char*& buff);
    BSON_TYPE type = BSON_TYPE::INT32;
    std::string dump();
    int32_t get();
private:
    int32_t val;

};

class BsonInt64: public BsonObj{
public:
    BsonInt64() = delete;
    BsonInt64(char*& buff);
    BSON_TYPE type = BSON_TYPE::INT64;
    std::string dump();
    int64_t get();
private:
    int64_t val;
};

class BsonUint64: public BsonObj{
public:
    BsonUint64() = delete;
    BsonUint64(char*& buff);
    BSON_TYPE type = BSON_TYPE::INT64;
    std::string dump();
    uint64_t get();
private:
    uint64_t val;
};

class BsonDouble: public BsonObj{
public:
    BsonDouble() = delete;
    BsonDouble(char*& buff);
    BSON_TYPE type = BSON_TYPE::DOUBLE;
    std::string dump();
    double get();
private:
    double val;
};

class BsonString: public BsonObj{
public:
    BsonString() = delete;
    BsonString(char*& buff);
    BSON_TYPE type = BSON_TYPE::STRING;
    std::string dump();
    std::string get();
private:
    std::string str;

};

class BsonTime: public BsonObj{
public:
    BsonTime() = delete;
    BsonTime(char*& buff);
    BSON_TYPE type = BSON_TYPE::TIME;
    std::string dump();
    std::time_t get();
private:
    std::time_t val;
};

class BsonObjID: public BsonObj{
public:
    BsonObjID() = delete;
    BsonObjID(char*& buff);
    BSON_TYPE type = BSON_TYPE::OBJ_ID;
    std::string dump();
    const int fixed_len = 12;
    //std::time_t get();
private:
    std::vector<char> val;
};

#endif // ndef BISON_OBJECTS_HPP