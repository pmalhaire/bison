#include "bison_objects.hpp"
#include "bison_read_bytes.hpp"
#include "bison_types.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace bison {
std::unique_ptr<BsonObj> BsonObj::Parse(vect_it &it) {
    Bson_type type = static_cast<Bson_type>(read(it));
    // std::cout << "reading type 0x" << std::hex << std::setfill('0') <<
    // std::setw(2) << (int) type << std::endl;
    switch (type) {
    case Bson_type::DOC:
        return nullptr; // 00 indicates the end of the document
    case Bson_type::DOUBLE:
        return std::make_unique<BsonDouble>(it);
    case Bson_type::STRING:
        return std::make_unique<BsonString>(it);
    case Bson_type::EMB_DOC:
        return std::make_unique<BsonDoc>(it);
    case Bson_type::ARR:
        return std::make_unique<BsonArr>(it);
    case Bson_type::BIN:
        return std::make_unique<BsonBin>(it);
    case Bson_type::UNDEF:
        return std::make_unique<BsonUndef>(it);
    case Bson_type::OBJ_ID:
        return std::make_unique<BsonID>(it);
    case Bson_type::BOOL:
        return std::make_unique<BsonBool>(it);
    case Bson_type::TIME:
        return std::make_unique<BsonTime>(it);
    case Bson_type::NULL_VALUE:
        return std::make_unique<BsonNull>(it);
    case Bson_type::CSTRING:
        return std::make_unique<BsonCString>(it);
    case Bson_type::DBPOINTER:
        return std::make_unique<BsonDBPointer>(it);
    case Bson_type::JS_CODE:
        return std::make_unique<BsonJsCode>(it);
    case Bson_type::SYMBOL:
        return std::make_unique<BsonSymbol>(it);
    case Bson_type::JS_CODE_WS:
        return std::make_unique<BsonJsCodeWC>(it);
    case Bson_type::INT32:
        return std::make_unique<BsonInt32>(it);
    case Bson_type::UINT64:
        return std::make_unique<BsonUint64>(it);
    case Bson_type::INT64:
        return std::make_unique<BsonInt64>(it);
    case Bson_type::DEC128:
        return std::make_unique<BsonDec128>(it);
    case Bson_type::MIN_KEY:
        return std::make_unique<BsonMinKey>(it);
    case Bson_type::MAX_KEY:
        return std::make_unique<BsonMaxKey>(it);
    }
    std::cerr << "fatal type 0x" << std::hex << std::setfill('0')
              << std::setw(2) << (int)type << " is not in specification"
              << std::endl;
    exit(1);
    return nullptr;
}

BsonObj::BsonObj(vect_it &it, Bson_type type) : _type(type) {
    read_string(it, _name_begin, _name_size, false);
}

Bson_type BsonObj::type() const { return _type; }

std::string BsonObj::name() const {
    return std::string(_name_begin, _name_begin + (_name_size - 1));
}

std::string BsonObj::dump_one(std::string str) const {
    std::string s = "\"";
    s += std::string(_name_begin, _name_begin + (_name_size - 1));
    s += "\" : ";
    s += str;
    return s;
}

BsonString::BsonString(vect_it &it, Bson_type type) : BsonObj(it, type) {
    read_string(it, _string_begin, _string_size, true);
}

std::string BsonString::dump() const {
    return dump_one(
        "\"" + std::string(_string_begin, _string_begin + (_string_size - 1)) +
        "\"");
}

std::string BsonString::get() const {
    return std::string(_string_begin, _string_begin + (_string_size - 1));
}

BsonCString::BsonCString(vect_it &it) : BsonObj(it, Bson_type::STRING) {
    read_string(it, _string_begin, _string_size, false);
}

std::string BsonCString::dump() const {
    return dump_one(
        "\"" + std::string(_string_begin, _string_begin + (_string_size - 1)) +
        "\"");
}

std::string BsonCString::get() const {
    return std::string(_string_begin, _string_begin + (_string_size - 1));
}

BsonDoc::BsonDoc(vect_it it, size_t it_size)
    : BsonObj(Bson_type::DOC), _has_name(false), _pos(it), _size(read(_pos)),
      _begin(_pos), _end(it + _size) {
    _init(it_size);
}

BsonDoc::BsonDoc(vect_it it, Bson_type type)
    : BsonObj(it, type), _has_name(true), _pos(it), _size(read(_pos)),
      _begin(_pos), _end(it + _size) {
    _init();
}

void BsonDoc::_init(size_t it_size) {
    // std::cout << __FILE__<< __LINE__ << __func__ << " " << _size << " " <<
    // this << std::endl  << std::endl  << std::flush;  make sure size is > 0
    // (why use a signed int here ?? bson spec seems broken)
    if (_size < 0) {
        std::cerr << "fatal invalid bson file"
                  << "document size " << _size << "can't be negative "
                  << std::endl;
        exit(1);
    }

    // check vector size for initial documents
    if (it_size != (size_t)-1) {
        if ((size_t)_size > it_size) {
            std::cerr << "fatal incomplete bson file"
                      << " needed size " << _size << " vector size " << it_size
                      << std::endl;
            exit(1);
        }
        _vector_remaining_size = it_size;
    }
    _size -= 4;
}

vect_it BsonDoc::end() const { return _end; }

std::string BsonDoc::dump() const {
    vect_it local_it = _begin;
    std::string str;
    str += "{\n";
    std::unique_ptr<BsonObj> obj = BsonObj::Parse(local_it);
    str += obj->dump();
    if (obj->type() == Bson_type::ARR || obj->type() == Bson_type::DOC ||
        obj->type() == Bson_type::EMB_DOC) {
        auto doc = dynamic_cast<BsonDoc &>(*obj);
        local_it = doc.end();
    }
    while ((obj = BsonObj::Parse(local_it))) {
        str += ",\n";
        str += obj->dump();
        if (obj->type() == Bson_type::ARR || obj->type() == Bson_type::DOC ||
            obj->type() == Bson_type::EMB_DOC) {
            auto doc = dynamic_cast<BsonDoc &>(*obj);
            local_it = doc.end();
        }
    }
    str += "\n}\n";

    if (_has_name) {
        return dump_one(str);
    }
    return str;
}

std::unique_ptr<BsonObj> BsonDoc::next() {
    // std::unique_ptr<BsonObj> obj = std::move(_obj);
    // std::cout << std::endl << __FILE__<< " " << __LINE__ << " " << __func__
    // << " " << this << " " << _size << std::endl << std::flush;
    if (_size > 0) {
        vect_it start = _pos;
        std::unique_ptr<BsonObj> obj = BsonObj::Parse(_pos);
        _size -= std::distance(start, _pos);
        if (_vector_remaining_size != -1) {
            _vector_remaining_size -= std::distance(start, _pos);
        }
        // std::cout << __FILE__<< " " << __LINE__ << " " << __func__ << " " <<
        // std::distance(start, _pos) << " " << this << " " << _size <<
        // std::endl
        // << std::flush;
        if (_size > 0 && !obj) {
            // std::cerr << "error size should be null and is " << _size <<
            // std::endl;  exit(1);
        }
        return obj;
    }

    // std::cout << __FILE__<< " " << __LINE__ << " " << __func__ << " " << this
    // << " " << _size << std::endl << std::flush;

    return nullptr;
}

BsonNull::BsonNull(vect_it &it) : BsonObj(it, Bson_type::NULL_VALUE) {}

std::string BsonNull::dump() const { return dump_one("null"); }

BsonUndef::BsonUndef(vect_it &it) : BsonObj(it, Bson_type::UNDEF) {}

std::string BsonUndef::dump() const { return dump_one("undef"); }

BsonMinKey::BsonMinKey(vect_it &it) : BsonObj(it, Bson_type::MIN_KEY) {}

std::string BsonMinKey::dump() const { return dump_one("min_key"); }

BsonMaxKey::BsonMaxKey(vect_it &it) : BsonObj(it, Bson_type::MAX_KEY) {}

std::string BsonMaxKey::dump() const { return dump_one("max_key"); }

BsonBool::BsonBool(vect_it &it) : BsonObj(it, Bson_type::BOOL) {
    _val = read(it);
}

std::string BsonBool::dump() const {
    std::string str;
    if (_val) {
        str = dump_one("true");
    } else {
        str = dump_one("false");
    }
    return str;
}

bool BsonBool::get() const { return _val; }

BsonInt32::BsonInt32(vect_it &it) : BsonObj(it, Bson_type::INT32) {
    _val = read(it);
}

std::string BsonInt32::dump() const { return dump_one(std::to_string(_val)); }

int32_t BsonInt32::get() const { return _val; }

BsonInt64::BsonInt64(vect_it &it, Bson_type t) : BsonObj(it, t) {
    _val = read(it);
}

std::string BsonInt64::dump() const { return dump_one(std::to_string(_val)); }

int64_t BsonInt64::get() const { return _val; }

BsonUint64::BsonUint64(vect_it &it) : BsonObj(it, Bson_type::UINT64) {
    _val = read(it);
}

std::string BsonUint64::dump() const {
    // spec is not really clear but it seems logical like this
    // from https://json-bson-converter.appspot.com
    std::string s;
    s += "{ \"inc\" : ";
    s += std::to_string(_val << 32 >> 32);
    s += ", \"time\" : ";
    s += std::to_string(_val >> 32);
    s += " }";
    return dump_one(s);
}

uint64_t BsonUint64::get() const { return _val; }

BsonDouble::BsonDouble(vect_it &it) : BsonObj(it, Bson_type::DOUBLE) {
    _val = read(it);
}

std::string BsonDouble::dump() const { return dump_one(std::to_string(_val)); }

double BsonDouble::get() const { return _val; }

BsonID::BsonID(vect_it &it, Bson_type t)
    : BsonObj(it, t), _val(read_hex(it, _fixed_len)) {}

std::string BsonID::dump() const {
    // dumping as hex here
    std::stringstream ss;
    for (unsigned char c : _val) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)c;
    }
    return dump_one(ss.str());
}
const std::vector<unsigned char> &BsonID::get() const { return _val; }

BsonDBPointer::BsonDBPointer(vect_it &it) : BsonObj(it, Bson_type::DBPOINTER) {
    read_string(it, _db_begin, _db_size, true);
    _val = read_hex(it, fixed_len);
}

std::string BsonDBPointer::dump() const {
    std::stringstream ss;
    std::string s;
    s += "\"";
    s += std::string(_db_begin, _db_begin + (_db_size - 1));
    s += "\":";
    for (unsigned char c : _val) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)c;
    }
    s += ss.str();
    return dump_one(s);
}

const std::vector<unsigned char> &BsonDBPointer::get() { return _val; }

std::string BsonDBPointer::getString() const {
    return std::string(_db_begin, _db_begin + (_db_size - 1));
}

BsonBin::BsonBin(vect_it &it) : BsonObj(it, Bson_type::BIN) {
    int32_t size = read(it);
    // don't get why size is signed in spec
    if (size < 0) {
        std::cerr << "fatal binary object with negative size " << size
                  << std::endl;
        exit(1);
    }
    _subtype = read(it);
    _val = read_hex(it, size);
}

std::string BsonBin::dump() const {
    std::stringstream ss;
    ss << "bin len(" << size() << "):" << std::hex << std::setfill('0')
       << std::setw(2) << (int)_subtype << " ";
    for (unsigned char c : _val) {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)c;
    }
    return dump_one(ss.str());
}

const std::vector<unsigned char> &BsonBin::get() const { return _val; }

int32_t BsonBin::size() const { return _val.size(); }

unsigned char BsonBin::subtype() const { return _subtype; }

BsonJsCodeWC::BsonJsCodeWC(vect_it &it) : BsonObj(it, Bson_type::JS_CODE_WS) {
    _length = read(it);
    read_string(it, _code_begin, _code_size, true);
    _doc = std::make_unique<BsonDoc>(it);
}

std::string BsonJsCodeWC::dump() const {
    std::string s;
    s += "js_code len(";
    s += _length;
    s += ") :";
    s += std::string(_code_begin,
                     _code_begin +
                         (_code_size - 1)); // size in bson is with the \0
    s += " ";
    if (_doc != nullptr) {
        s += _doc->dump();
    }
    return dump_one(s);
}

int32_t BsonJsCodeWC::getLength() const { return _length; }

std::string BsonJsCodeWC::getCode() const {
    return std::string(_code_begin, _code_begin + (_code_size - 1));
}

std::unique_ptr<BsonDoc> BsonJsCodeWC::getDoc() { return std::move(_doc); }
} // namespace bison
