#include "bison.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iostream>
#include <memory>


std::unique_ptr<BsonDoc> Bson::next()
{
    auto remaining = std::distance(_pos, _vect.end());
    if (remaining > 0 ) {
        std::unique_ptr<BsonDoc> doc = std::make_unique<BsonDoc>(_pos, remaining);
        _pos = doc->end();
        return doc;
    }
    return nullptr;
}
