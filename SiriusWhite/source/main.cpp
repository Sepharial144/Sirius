#include "Application.hpp"

#include <memory>
#include <iostream>

// Main code
int main(int, char**)
{
    try
    {
        auto ptrApp = std::make_shared<Application>();
        ptrApp->Run();
    }
    catch (const std::exception& er)
    {
        std::cout << er.what() << &std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
