#ifndef MY_EXCEPTION_HPP
#define MY_EXCEPTION_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


class my_exception : public std::runtime_error {
    std::string file;
    int line;
    public:
        my_exception(const std::string &arg, const char *file, int line) :
        std::runtime_error(arg), file(file), line(line) {}

        const char *what() const noexcept override {
            std::ostringstream o;
            o << file << ":" << line << ": " << std::runtime_error::what();
            return o.str().c_str();
        }
};

#define throw_line(arg) throw my_exception(arg, __FILE__, __LINE__);

#endif // MY_EXCEPTION_HPP
