#include <exception>
#include <string>
#include "Windows.h"

namespace ui
{
    /*----------------------------------------------------------------------------------------------------
    Description:
        Throw exception on condidion

    Arguments:
        [in] bool     'is_error' - throw exception condition
        [in] const char* 'error' - message of exception

    Return:
        throw exception
    */
    void throw_exception_on(const bool is_error, const char* error);

    class exception : public std::exception
    {
    public:
        explicit exception(const char* error);
        explicit exception(std::string& error);
        explicit exception(const char* error, int32_t status_code);
        virtual ~exception() noexcept;
        virtual const char* what() const noexcept;

    protected:
        std::string m_error;
    };

} // !namespace ui

namespace Win32
{
    void throw_exception(const char* error, HRESULT status_code);
} // !namespace Win32