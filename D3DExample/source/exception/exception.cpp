#include "exception.hpp"

#include <sstream>
#include <exception>

#include "Windows.h"

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

    exception::exception(std::string& error) :m_error{ error } {}

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

namespace Win32
{
    inline std::string HrToString(HRESULT hr)
    {
        char s_str[64] = {};
        sprintf_s(s_str, " HRESULT of 0x%08X", static_cast<UINT>(hr));
        return std::string(s_str);
    }

    void throw_exception(const char* error, HRESULT status_code)
    {
        std::string errorString{ error };
        errorString.append(HrToString(status_code));
        throw ui::exception(errorString);
    }
} // !namespace Win32