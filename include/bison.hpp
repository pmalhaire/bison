#include <vector>

/* {"hello":[1, "two", 3.0]}
BsonObj{
    type dict
    size 
    elements [
        BsonObj
        { 
            type key
            value "hello"
        },
        BsonObj
        {
            type array
        }
    ]
}

*/
namespace BSON{
    enum type {
        DOC,        // int32 e_list "\x00" 	BSON Document. int32 is the total number of bytes comprising the document.
        DOUBLE,     //"\x01" e_name double 	64-bit binary floating point
        STRING, 	// UTF-8 string String - The int32 is the number bytes in the (byte*) + 1 (for the trailing '\x00'). The (byte*) is zero or more UTF-8 encoded characters.
        EMB_DOC,    //	"\x03" e_name document 	Embedded document
        ARR,	    //	"\x04" e_name document 	Array
        BIN, 	    // "\x05" e_name binary 	Binary data
        UNDEF, 	    //  "\x06" e_name 	Undefined (value) — Deprecated
        OBJ_ID,     // "\x07" e_name (byte*12) 	ObjectId
        BOOL,	    //  "\x08" e_name "\x00" Boolean "false" or "\x08" e_name "\x01" 	Boolean "true"
        TIME,	    // 	"\x09" e_name int64 	UTC datetime
        NULL_VALUE, // 	"\x0A" e_name 	Null value
        CSTRING,	// 	"\x0B" e_name cstring cstring 	Regular expression - The first cstring is the regex pattern, the second is the regex options string. Options are identified by characters, which must be stored in alphabetical order. Valid options are 'i' for case insensitive matching, 'm' for multiline matching, 'x' for verbose mode, 'l' to make \w, \W, etc. locale dependent, 's' for dotall mode ('.' matches everything), and 'u' to make \w, \W, etc. match unicode.
        DBPOINTER,  // 	"\x0C" e_name string (byte*12) 	DBPointer — Deprecated
        JS_CODE,    // 	"\x0D" e_name string 	JavaScript code
        SYMBOL,        // 	"\x0E" e_name string 	Symbol. Deprecated
        JS_SCOPE_CODE,  // 	"\x0F" e_name code_w_s 	JavaScript code w/ scope
        INT32,          // 	"\x10" e_name int32 	32-bit integer
        UINT64,         // 	"\x11" e_name uint64 	Timestamp
        INT64,          // 	"\x12" e_name int64 	64-bit integer
        DEC128,         // 	"\x13" e_name decimal128 	128-bit decimal floating point
        MIN_KEY,        // 	"\xFF" e_name 	Min key
        MAX_KEY        // 	"\x7F" e_name 	Max key
    };
}

class BsonObj {
    BSON::type type;
    //todo test with bigger files
    virtual std::string dump() = 0;
};

class BsonArray: BsonObj{
    BsonArray(char* buff);
    BSON::type type = BSON::ARR;
    std::vector<BsonObj*> values;
    virtual std::string dump();
};

class BsonMap: BsonObj{
    BsonMap(char* buff);
    BSON::type type = BSON::EMB_DOC;
    //std::map is ordered alphabeticaly nothing says that in the spec
    std::vector<std::string*> keys;
    std::vector<BsonObj*> values;
    virtual std::string dump();
};
    


class Bson {
public:
    Bson() = delete;
    Bson(const std::vector<char>& vect);
    BsonObj* next();

private:
    BsonObj *obj = nullptr;
};


