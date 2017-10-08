#include "bison.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <ctime>

#include <iostream>

Bson::Bson(std::vector<char>& vect):_vect(vect) {
    char* buff = vect.data();
    _doc = new BsonDoc(buff, vect.size());
    _pos = vect.begin() + (buff - vect.data());
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
        char* start = _vect.data() + std::distance(_vect.begin(), _pos);
        char* buff = start;
        _doc = new BsonDoc(buff, remaining);
        _pos += buff - start;
    }
}
