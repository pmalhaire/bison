#include "bison.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <ctime>

#include <iostream>

Bson::Bson(const std::vector<char>& vect):_vect(vect) {
    _pos = vect.begin();
    _doc = new BsonDoc(_pos, vect.size());
};

Bson::~Bson(){
    std::vector<BsonObj*> toDelete;
    if ( _doc != nullptr ) { delete _doc; };
}

std::string Bson::dump() {
    std::string str;
    BsonObj* doc = getDoc();
    if (doc != nullptr) {
        str += doc->dump();
    }
    return str;
}

BsonDoc* Bson::getDoc()
{
    return _doc;
}

void Bson::next()
{
    if ( _doc != nullptr ) { delete _doc; _doc = nullptr;};

    auto remaining = std::distance(_pos, _vect.end());
    if (remaining > 0 ) {
        _doc = new BsonDoc(_pos, remaining);
    }
}
