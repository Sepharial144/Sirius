#include "exception.hpp"

#include <sstream>

namespace sirius
{
    void throw_exception_on(const bool is_error, const char* error)
    {
        if (is_error)
           throw net::exception(error);
    }

    exception::exception(const char* error)
        : m_errorCode{ errno }
    {
        std::stringstream ss;
        ss << error << " code: " << std::to_string(m_errorCode);
        m_error = ss.str();
    }

    exception::exception(const char* error, int32_t status_code)
        : m_errorCode{ errno }
    {
        std::stringstream ss;
        ss << error << " code: " << m_errorCode << " return code " << status_code;
        m_error = ss.str();
    }

    exception::~exception() noexcept
    {
    }

    const char* exception::what() const noexcept {
        return m_error.c_str();
    }

    int32_t exception::errorCode() const
    {
        return m_errorCode;
    }
} // !namespace sirius