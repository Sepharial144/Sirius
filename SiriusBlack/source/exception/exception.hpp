#ifndef _UI_EXCEPTION_HPP_
#define _UI_EXCEPTION_HPP_

#include <exception>
#include <string>

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
        explicit exception(const char* error, int32_t status_code);
        virtual ~exception() noexcept;
        virtual const char* what() const noexcept;

    protected:
        std::string m_error;
    };

} // !namespace ui

#endif // !_UI_EXCEPTION_HPP_