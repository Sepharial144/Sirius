#ifndef _SIRIUS_STORAGE_INTERFACE_HPP_
#define _SIRIUS_STORAGE_INTERFACE_HPP_

#include <exception>
#include <string>

namespace sirius
{
    /*
    * Exception wrapper
    */
    void throw_exception_on(const bool is_error, const char* error);

    /*
    * Exception
    */
    class Storage
    {
    public:
        explicit Storage();
        virtual ~Storage();

        virtual connect(const char* connection_line) = 0;
        virtual reconnect(const char* connection_line) = 0;
        virtual disconnect(const char* connection_line) = 0;
    };

} // !namespace sirius

#endif // !_SIRIUS_STORAGE_INTERFACE_HPP_