#include "exception.hpp"

#include <sstream>
#include <exception>

namespace ui
{
    void throw_exception_on(const bool is_error, const char* error)
    {
        if (is_error)
            throw ui::exception(error);
    }


    exception::exception(const char* error)
    {
        std::stringstream ss;
        ss << error;
        m_error = ss.str();
    }


    exception::exception(const char* error, int32_t status_code)
    {
        std::stringstream ss;
        ss << error << " return code " << status_code;
        m_error = ss.str();
    }


    exception::~exception() noexcept
    {
    }


    const char* exception::what() const noexcept {
        return m_error.c_str();
    }
} // !namespace net