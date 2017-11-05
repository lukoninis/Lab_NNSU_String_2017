#ifndef STRING_H
#define STRING_H

#include <crtdefs.h>
#include <memory>
namespace My
{
    template< typename _Char_T >
    class basic_string
    {
    private:
        // возврат неправильного значения в случае провала
        static const size_t m_npos = static_cast< size_t >( -1 );

        // текущая длина
        size_t m_current_lenght;

        // указатель на буффер
        std::unique_ptr<_Char_T[]> m_buffer;

        // флаг для коротких строк
        bool m_is_local = true;

        // локальный буфер для коротких строк
        // пусть будет длиной 15 байт
        constexpr static size_t _local_lenght() noexcept
        {
            return static_cast<size_t>( 15 / sizeof( _Char_T ) );
        }
        union
        {
            _Char_T m_local_buffer[_local_lenght()] = {0};
            size_t m_capacity;
        };

    public:
        explicit basic_string() noexcept:
            m_current_lenght(0),
            m_is_local(true)
        {

        }
    };

    using String = basic_string<char>;
}
#endif // STRING_H
