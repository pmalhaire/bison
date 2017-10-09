#ifndef BISON_OBJECTS_HPP
#define BISON_OBJECTS_HPP

#include <string>
#include <ctime>
#include <vector>

#include "bison_types.hpp"

using vect_it = std::vector<char>::const_iterator;

class BsonObj {
public:
    BsonObj(){};                         // use for document (not embeded)
    BsonObj(vect_it& buff);                // compute the name at construction
    virtual ~BsonObj(){};

    //const BSON_TYPE type;              the type needed to access the real type (defined in subclasses)
    //[cpp type] get()                   function to access the object as a cpp type (defined in subclasses)
    std::string name();                  // name of the field or "" for the initial document
    virtual std::string dump() = 0;      // use to get a human readable string representing the object                 
    BsonObj* next();                     // get the next object of the document
    void setNext(BsonObj* obj);          // internal function to set the next document
    std::string dump_one(std::string);   // small helper for type with one value
    static BsonObj* Parse(vect_it& buff);  // factory used to create objects
private:
    BsonObj* _next = nullptr;
    vect_it _name_begin;                   // pointer to the begining of the name string
    int32_t _name_size;                  // size of the name string
};

class BsonDoc : public BsonObj{
public:
    BsonDoc(vect_it& buff, size_t buff_size);
    BsonDoc(vect_it& buff);
    ~BsonDoc();
    const BSON_TYPE type = BSON_TYPE::DOC;
    BsonObj* get();                       //first object T of the document use T->next() until nullptr for the all document
    std::string dump();
private:
    void _init(vect_it& buff, size_t buff_size = -1);
    BsonObj* _obj = nullptr;
    const bool _has_name;
};

class BsonArr : public BsonDoc{
public:
    BsonArr(vect_it& buff):BsonDoc(buff){};
    const BSON_TYPE type = BSON_TYPE::ARR;
};

class BsonJsCodeWC : public BsonObj{
public:                      
    BsonJsCodeWC(vect_it& buff);
    ~BsonJsCodeWC();
    const BSON_TYPE type = BSON_TYPE::JS_CODE;
    std::string dump();
    int32_t getLength();
    std::string getCode();
    BsonDoc* getDoc();          
private:
    int32_t _length;
    vect_it   _code_begin;                 // pointer to the begining of the code string
    int32_t _code_size;                  // size of the code string
    BsonDoc* _doc = nullptr;
};

class BsonNull: public BsonObj{
public:
    BsonNull(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::NULL_VALUE;
    std::string dump();
};

class BsonUndef: public BsonObj{
public:
    BsonUndef(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::UNDEF;
    std::string dump();
};

class BsonMinKey: public BsonObj{
public:
    BsonMinKey(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::MIN_KEY;
    std::string dump();
};

class BsonMaxKey: public BsonObj{
public:
    BsonMaxKey(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::MAX_KEY;
    std::string dump();
};

class BsonBool: public BsonObj{
public:
    BsonBool(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::BOOL;
    std::string dump();
    bool get();
private:
    bool _val;
};

class BsonInt32: public BsonObj{
public:
    BsonInt32(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::INT32;
    std::string dump();
    int32_t get();
private:
    int32_t _val;

};

class BsonInt64: public BsonObj{
public:
    BsonInt64(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::INT64;
    std::string dump();
    int64_t get();
private:
    int64_t _val;
};

class BsonTime: public BsonInt64{
public:
    BsonTime(vect_it& buff):BsonInt64(buff){};
    const BSON_TYPE type = BSON_TYPE::TIME;
};


class BsonUint64: public BsonObj{
public:
    BsonUint64(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::INT64;
    std::string dump();
    uint64_t get();
private:
    uint64_t _val;
};

class BsonDouble: public BsonObj{
public:
    BsonDouble(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::DOUBLE;
    std::string dump();
    double get();
private:
    double _val;
};

class BsonString: public BsonObj{
public:
    BsonString(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::STRING;
    std::string dump();
    std::string get();
private:
    vect_it _string_begin;
    int32_t _string_size;
};

class BsonSymbol: public BsonString{
public:
    BsonSymbol(vect_it& buff):BsonString(buff){}
    const BSON_TYPE type = BSON_TYPE::SYMBOL;
};

class BsonJsCode: public BsonString{
public:
    BsonJsCode(vect_it& buff):BsonString(buff){}
    const BSON_TYPE type = BSON_TYPE::JS_CODE;
};

class BsonCString: public BsonObj{
public:
    BsonCString(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::CSTRING;
    std::string dump();
    std::string get();
private:
    vect_it _string_begin;
    int32_t _string_size;
};

class BsonID: public BsonObj{
public:
    BsonID(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::OBJ_ID;
    std::string dump();
    const int fixed_len = 12;
    const std::vector<unsigned char>& get();
private:
    std::vector<unsigned char> _val;
};

class BsonDBPointer: public BsonObj{
public:
    BsonDBPointer(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::DBPOINTER;
    std::string dump();
    const int fixed_len = 12;
    const std::vector<unsigned char>& get();
    std::string getString();
private:
    std::vector<unsigned char> _val;
    vect_it _db_begin;                 // pointer to the begining of the db string
    int32_t _db_size;                  // size of the db string
};


class BsonDec128: public BsonID{
public:
    BsonDec128(vect_it& buff);
    //dec128 is not in cpp14
    const BSON_TYPE type = BSON_TYPE::DEC128;
    const int fixed_len = 16;
};

class BsonBin: public BsonObj{
public:
    BsonBin(vect_it& buff);
    const BSON_TYPE type = BSON_TYPE::BIN;

    std::string dump();
    const std::vector<unsigned char>& get();
    int32_t size(); //size is stored as int32_t
    unsigned char subtype();    
private:
    std::vector<unsigned char> _val;
    unsigned char _subtype;
};


#endif // ndef BISON_OBJECTS_HPP