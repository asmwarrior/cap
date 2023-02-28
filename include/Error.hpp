#ifndef CAP_ERROR_HPP
#define CAP_ERROR_HPP


#include <string>
#include "Position.hpp"


namespace cap {


    struct Error {
        Position position;
        std::string description;
    };


} //namespace cap


#endif //CAP_ERROR_HPP
