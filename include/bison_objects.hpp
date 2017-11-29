#ifndef BISON_OBJECTS_HPP
#define BISON_OBJECTS_HPP

#include <ctime>
#include <string>
#include <vector>

#include "bison_types.hpp"
namespace bison {
using vect_it = std::vector<char>::const_iterator;

class BsonObj {
  public:
    BsonObj() = delete;
    // constructor for document (not embeded)
    explicit BsonObj(Bson_type t) : _type(t) {}
    // compute the name at construction
    explicit BsonObj(vect_it &it, Bson_type t);
    Bson_type type() const; // the type of the object
    //[cpp type] get()                   function to access the object as a cpp
    // type (defined in subclasses)
    std::string
    name() const; // name of the field or "" for the initial document
    virtual std::string dump()
        const = 0; // use to get a human readable string representing the object
    std::string
        dump_one(std::string) const; // small helper for type with one value
    static std::unique_ptr<BsonObj>
    Parse(vect_it &it); // factory used to create objects
  private:
    vect_it _name_begin;    // pointer to the begining of the name string
    int32_t _name_size = 0; // size of the name string
    const Bson_type _type;
};

class BsonDoc : public BsonObj {
  public:
    BsonDoc(vect_it it, size_t it_size); // initial document
    BsonDoc(vect_it it, Bson_type = Bson_type::DOC);
    std::unique_ptr<BsonObj>
    next(); // use T->next() until nullptr for the all document
    vect_it end() const;
    virtual std::string dump() const override;

  private:
    void _init(size_t it_size = static_cast<size_t>(-1));
    const bool _has_name;
    vect_it _pos;
    int32_t _size;
    size_t _vector_remaining_size = static_cast<size_t>(-1);
    const vect_it _begin;
    const vect_it _end; // next pos after vect
};

class BsonArr : public BsonDoc {
  public:
    explicit BsonArr(vect_it &it) : BsonDoc(it, Bson_type::ARR) {}
};

class BsonJsCodeWC : public BsonObj {
  public:
    explicit BsonJsCodeWC(vect_it &it);
    std::string dump() const override;
    int32_t getLength() const;
    std::string getCode() const;
    std::unique_ptr<BsonDoc> getDoc();

  private:
    int32_t _length;
    vect_it _code_begin; // pointer to the begining of the code string
    int32_t _code_size;  // size of the code string
    std::unique_ptr<BsonDoc> _doc = nullptr;
};

class BsonNull : public BsonObj {
  public:
    explicit BsonNull(vect_it &it);
    std::string dump() const override;
};

class BsonUndef : public BsonObj {
  public:
    explicit BsonUndef(vect_it &it);
    std::string dump() const override;
};

class BsonMinKey : public BsonObj {
  public:
    explicit BsonMinKey(vect_it &it);
    std::string dump() const override;
};

class BsonMaxKey : public BsonObj {
  public:
    explicit BsonMaxKey(vect_it &it);
    std::string dump() const override;
};

class BsonBool : public BsonObj {
  public:
    explicit BsonBool(vect_it &it);
    std::string dump() const override;
    bool get() const;

  private:
    bool _val;
};

class BsonInt32 : public BsonObj {
  public:
    explicit BsonInt32(vect_it &it);
    std::string dump() const override;
    int32_t get() const;

  private:
    int32_t _val;
};

class BsonInt64 : public BsonObj {
  public:
    explicit BsonInt64(vect_it &it, Bson_type t = Bson_type::INT64);
    std::string dump() const override;
    int64_t get() const;

  private:
    int64_t _val;
};

class BsonTime : public BsonInt64 {
  public:
    explicit BsonTime(vect_it &it) : BsonInt64(it, Bson_type::TIME) {}
};

class BsonUint64 : public BsonObj {
  public:
    explicit BsonUint64(vect_it &it);
    std::string dump() const override;
    uint64_t get() const;

  private:
    uint64_t _val;
};

class BsonDouble : public BsonObj {
  public:
    explicit BsonDouble(vect_it &it);
    std::string dump() const override;
    double get() const;

  private:
    double _val;
};

class BsonString : public BsonObj {
  public:
    explicit BsonString(vect_it &it, Bson_type type = Bson_type::STRING);
    std::string dump() const override;
    std::string get() const;

  private:
    vect_it _string_begin;
    int32_t _string_size;
};

class BsonSymbol : public BsonString {
  public:
    explicit BsonSymbol(vect_it &it) : BsonString(it, Bson_type::SYMBOL) {}
};

class BsonJsCode : public BsonString {
  public:
    explicit BsonJsCode(vect_it &it) : BsonString(it, Bson_type::JS_CODE) {}
};

class BsonCString : public BsonObj {
  public:
    explicit BsonCString(vect_it &it);
    std::string dump() const override;
    std::string get() const;

  private:
    vect_it _string_begin;
    int32_t _string_size;
};

class BsonID : public BsonObj {
  public:
    explicit BsonID(vect_it &it, Bson_type type = Bson_type::OBJ_ID);
    std::string dump() const override;
    const std::vector<unsigned char> &get() const;

  private:
    static const int _fixed_len = 12;
    std::vector<unsigned char> _val;
};

class BsonDec128 : public BsonID {
  public:
    explicit BsonDec128(vect_it &it) : BsonID(it, Bson_type::DEC128) {}
    // dec128 is not in cpp14
  private:
    static const int _fixed_len = 16;
};

class BsonDBPointer : public BsonObj {
  public:
    explicit BsonDBPointer(vect_it &it);
    std::string dump() const override;
    const int fixed_len = 12;
    const std::vector<unsigned char> &get();
    std::string getString() const;

  private:
    std::vector<unsigned char> _val;
    vect_it _db_begin; // pointer to the begining of the db string
    int32_t _db_size;  // size of the db string
};

class BsonBin : public BsonObj {
  public:
    explicit BsonBin(vect_it &it);
    std::string dump() const override;
    const std::vector<unsigned char> &get() const;
    int32_t size() const; // size is stored as int32_t
    unsigned char subtype() const;

  private:
    std::vector<unsigned char> _val;
    unsigned char _subtype;
};
} // namespace bison
#endif // ndef BISON_OBJECTS_HPP
