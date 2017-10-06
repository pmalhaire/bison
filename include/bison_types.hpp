#include <string>
#include <ctime>
#include <vector>

enum class BSON_TYPE : unsigned char{
    DOC             = 0x00,  // BSON Document. 
    DOUBLE          = 0x01,  //"\x01" e_name double 	64-bit binary floating point
    STRING          = 0x02,  // UTF-8 string String - The int32 is the number bytes in the (byte*) + 1 (for the trailing '\x00'). The (byte*) is zero or more UTF-8 encoded characters.
    EMB_DOC         = 0x03,  //	"\x03" e_name document 	Embedded document
    ARR             = 0x04,	 //	"\x04" e_name document 	Array
    BIN             = 0x05,  // "\x05" e_name binary 	Binary data
    UNDEF           = 0x06,  //  "\x06" e_name 	Undefined (value) — Deprecated
    OBJ_ID          = 0x07,  // "\x07" e_name (byte*12) 	ObjectId
    BOOL,	        //  "\x08" e_name "\x00" Boolean "false" or "\x08" e_name "\x01" 	Boolean "true"
    TIME,	        // 	"\x09" e_name int64 	UTC datetime
    NULL_VALUE,     // 	"\x0A" e_name 	Null value
    CSTRING,	    // 	"\x0B" e_name cstring cstring 	Regular expression - The first cstring is the regex pattern, the second is the regex options string. Options are identified by characters, which must be stored in alphabetical order. Valid options are 'i' for case insensitive matching, 'm' for multiline matching, 'x' for verbose mode, 'l' to make \w, \W, etc. locale dependent, 's' for dotall mode ('.' matches everything), and 'u' to make \w, \W, etc. match unicode.
    DBPOINTER,      // 	"\x0C" e_name string (byte*12) 	DBPointer — Deprecated
    JS_CODE,        // 	"\x0D" e_name string 	JavaScript code
    SYMBOL,        // 	"\x0E" e_name string 	Symbol. Deprecated
    JS_SCOPE_CODE,  // 	"\x0F" e_name code_w_s 	JavaScript code w/ scope
    INT32,          // 	"\x10" e_name int32 	32-bit integer
    UINT64,         // 	"\x11" e_name uint64 	Timestamp
    INT64,          // 	"\x12" e_name int64 	64-bit integer
    DEC128,         // 	"\x13" e_name decimal128 	128-bit decimal floating point
    MIN_KEY,        // 	"\xFF" e_name 	Min key
    MAX_KEY        // 	"\x7F" e_name 	Max key
};

class BsonObj {
public:
    BsonObj() = delete;
    BsonObj(char*& buff);
    virtual ~BsonObj(){};
    BSON_TYPE type;
    std::string name;
    //todo test with bigger files
    virtual std::string dump() = 0;
    BsonObj* next();
    void setNext(BsonObj* obj);
private:
    BsonObj* _next = nullptr;
};

namespace BSON{
    BsonObj* parse(char*& buff);
}

class BsonDoc{
public:
    BsonDoc() = delete;
    BsonDoc(char*& buff, size_t buff_size);
    ~BsonDoc();
    BsonObj* get();
    std::string dump();
    BsonDoc* next();
    void setNext(BsonDoc* next);
private:
    BsonObj* _obj = nullptr;
    BsonDoc* _next = nullptr;
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


class BsonArray: public BsonObj{
public:
    BsonArray() = delete;
    BsonArray(char*& buff);
    BSON_TYPE type = BSON_TYPE::ARR;
    std::string dump();

private:
    std::vector<BsonObj*> values;
};