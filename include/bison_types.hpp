#include <string>
#include <ctime>
#include <vector>

enum class BSON_TYPE : unsigned char{
    DOC             = 0x00,  // BSON Document. 
    DOUBLE          = 0x01,  // name double 64-bit binary floating point
    STRING          = 0x02,  // UTF-8 string String - The int32 is the number bytes in the (byte*) + 1 (for the trailing '\x00'). The (byte*) is zero or more UTF-8 encoded characters.
    EMB_DOC         = 0x03,  //	name document 	Embedded document
    ARR             = 0x04,	 //	name document 	Array
    BIN             = 0x05,  // name binary 	Binary data
    UNDEF           = 0x06,  // name Undefined (value) — Deprecated
    OBJ_ID          = 0x07,  // name (byte*12) 	ObjectId
    BOOL	        = 0x08,  // name "\x00" Boolean "false" or "\x08" e_name "\x01" 	Boolean "true"
    TIME 	        = 0x09,  // name int64 	UTC datetime
    NULL_VALUE      = 0x0A,  // name 	Null value
    CSTRING 	    = 0x0B,  // name cstring cstring 	Regular expression - The first cstring is the regex pattern, the second is the regex options string. Options are identified by characters, which must be stored in alphabetical order. Valid options are 'i' for case insensitive matching, 'm' for multiline matching, 'x' for verbose mode, 'l' to make \w, \W, etc. locale dependent, 's' for dotall mode ('.' matches everything), and 'u' to make \w, \W, etc. match unicode.
    DBPOINTER       = 0x0C,  // name string (byte*12) 	DBPointer — Deprecated
    JS_CODE         = 0x0D,  // name string 	JavaScript code
    SYMBOL          = 0x0E,  // name string 	Symbol. Deprecated
    JS_S_CODE       = 0x0F,  // name code_w_s 	JavaScript code w/ scope
    INT32           = 0x10,  // name int32 	32-bit integer
    UINT64          = 0x11,  // name uint64 	Timestamp
    INT64           = 0x12,  // name int64 	64-bit integer
    DEC128          = 0x13,  // name decimal128 	128-bit decimal floating point
    MIN_KEY         = 0xFF,  // name Min key
    MAX_KEY         = 0x7F   // name Max key
};

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
