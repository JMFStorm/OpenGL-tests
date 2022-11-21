#include "j_util.hpp"
#include <iostream>

void j_assert(bool assertion, std::string errorMessage)
{
    if (assertion == false) {
        std::string text = errorMessage;
        std::cout << text << std::endl;
        abort();
    }
}
