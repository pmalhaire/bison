#ifndef BISON_HPP
#define BISON_HPP

#include "bison_objects.hpp"

#include <vector>

/*
main class to use the bson parser
*/

class Bson{
public:
    Bson() = delete;
    ~Bson();
    Bson(std::vector<char>& vect);
    std::string dump();
    BsonDoc* getDoc();
    void next();
private:
    BsonDoc* _doc = nullptr;
    std::vector<char>::iterator pos;
    std::vector<char>& _vect;
};

#endif // ndef BISON_HPP
