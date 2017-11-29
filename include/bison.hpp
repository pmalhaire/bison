#ifndef BISON_HPP
#define BISON_HPP

#include "bison_objects.hpp"

#include <vector>

/*
main class to use the bson parser
this class implements the specification v1.1 of the bson spec

how to use :
loads a bsonfile in memory using std::vector<char>

Note:
the file MUST be a complete and valid bson file
invalid bson file will result on hard exit

see bison_object.hpp for each type cpp accessor
simple type T use T BsonT::get()
composite types may have several get functions ex: getCode() getLenght()
see accessor_tests for an example

all objects except intial BsonDoc have a name accessed by std::string
BsonT::name()

*/
namespace bison {
class Bson {
  public:
    Bson() = delete;
    explicit Bson(const std::vector<char> &vect)
        : _vect(vect), _pos(vect.begin()) {}
    // get the current document as BsonDoc or nullptr if there is no more
    std::unique_ptr<BsonDoc> next();

  private:
    const std::vector<char> &_vect;
    vect_it _pos;
};
} // namespace bison
#endif // ndef BISON_HPP
