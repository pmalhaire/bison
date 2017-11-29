#ifndef BISON_TYPES_HPP
#define BISON_TYPES_HPP
namespace bison {
enum class Bson_type : unsigned char {
    DOC = 0x00,    // BSON Document.
    DOUBLE = 0x01, // name double 64-bit binary floating point
    STRING = 0x02, // UTF-8 string String - The int32 is the number bytes in the
                   // (byte*) + 1 (for the trailing '\x00'). The (byte*) is zero
                   // or more UTF-8 encoded characters.
    EMB_DOC = 0x03,    //	name document 	Embedded document
    ARR = 0x04,        //	name document 	Array
    BIN = 0x05,        // name binary 	Binary data
    UNDEF = 0x06,      // name Undefined (value) — Deprecated
    OBJ_ID = 0x07,     // name (byte*12) 	ObjectId
    BOOL = 0x08,       // name "\x00" Boolean "false" or "\x08" e_name "\x01"
                       // Boolean "true"
    TIME = 0x09,       // name int64 	UTC datetime
    NULL_VALUE = 0x0A, // name 	Null value
    CSTRING = 0x0B,    // name cstring cstring 	Regular expression - The first
                       // cstring is the regex pattern, the second is the regex
                       // options string. Options are identified by characters,
    // which must be stored in alphabetical order. Valid options
    // are 'i' for case insensitive matching, 'm' for multiline
    // matching, 'x' for verbose mode, 'l' to make \w, \W, etc.
    // locale dependent, 's' for dotall mode ('.' matches
    // everything), and 'u' to make \w, \W, etc. match unicode.
    DBPOINTER = 0x0C,  // name string (byte*12) 	DBPointer — Deprecated
    JS_CODE = 0x0D,    // name string 	JavaScript code
    SYMBOL = 0x0E,     // name string 	Symbol. Deprecated
    JS_CODE_WS = 0x0F, // name code_w_s 	JavaScript code w/ scope
    INT32 = 0x10,      // name int32 	32-bit integer
    UINT64 = 0x11,     // name uint64 	Timestamp
    INT64 = 0x12,      // name int64 	64-bit integer
    DEC128 = 0x13,     // name decimal128 	128-bit decimal floating point
    MIN_KEY = 0xFF,    // name Min key
    MAX_KEY = 0x7F     // name Max key
};
} // namespace bison
#endif // ndef BISON_TYPES_HPP
