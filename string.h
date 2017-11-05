#ifndef STRING_H
#define STRING_H

#include <crtdefs.h>
#include <memory>
#include <iostream>

namespace My
{
    template< typename _Char_T >
    class basic_string
    {
    private:
        // возврат неправильного значения в случае провала
        constexpr static size_t m_npos { static_cast< size_t >( -1 ) };
        // длина локального буфера
        constexpr static size_t m_local_capasity{ 15 };

        // текущая длина
        size_t m_current_lenght{ m_npos };

        // указатель на буффер
        std::unique_ptr<_Char_T[]> m_buffer{ nullptr };

        // флаг для коротких строк
        bool m_is_local{ true };

        // локальный буфер для коротких строк
        // пусть будет длиной 15 байт
        constexpr static size_t _local_lenght() noexcept
        {
            return static_cast<size_t>( m_local_capasity / sizeof( _Char_T ) );
        }
        union
        {
            _Char_T m_local_buffer[_local_lenght()] = {0};
            size_t m_capacity;
        };

        constexpr inline size_t _strlen( const _Char_T * _buff )
        {
            size_t result = m_npos;
            if( nullptr != _buff )
            {
                result = 0;
                while( *_buff != static_cast<_Char_T>( '\0' ) )
                {
                    ++result;
                    ++_buff;
                }
            }
            return result;
        }

    public:
        // конструктор по умолчанию
        explicit basic_string() noexcept {}

        // копирующий конструктор
        explicit basic_string( const basic_string & _left ) noexcept:
            m_current_lenght( _left.m_current_lenght ),
            m_is_local( _left.m_is_local )
        {
            if( m_is_local )
            {
                memcpy( m_local_buffer, _left.m_local_buffer, m_local_capasity );
            }
            else
            {
                m_capacity = _left.m_capacity;
                m_buffer = std::make_unique<_Char_T[]>( m_capacity );
                memcpy( m_buffer.get(), _left.m_buffer.get(), m_capacity );
            }
        }

        // перемещающий конструктор
        explicit basic_string( basic_string && _left ) noexcept:
            m_current_lenght( _left.m_current_lenght ),
            m_is_local( _left.m_is_local )
        {
            if( m_is_local )
            {
                // тут проще скопировать
                memcpy( m_local_buffer, _left.m_local_buffer, m_local_capasity );
            }
            else
            {
                // а тут переместить
                m_capacity = _left.m_capacity;
                m_buffer = std::move( _left.m_buffer );
            }
            _left.reset();
        }

        // коструктор из с-строк
        explicit basic_string( const _Char_T * _left ) noexcept
        {
            m_current_lenght = _strlen( _left );
            if( m_current_lenght < _local_lenght() )
            {
                memcpy( m_local_buffer, _left, m_current_lenght * sizeof( _Char_T ) );
            }
            else
            {
                m_capacity = m_current_lenght + 1;
                m_buffer = std::make_unique<_Char_T[]>( m_capacity );
                memcpy( m_buffer.get(), _left, m_capacity );
                m_is_local = false;
            }
        }

        // очистка
        void reset() noexcept
        {
            for( size_t i = 0; i < _local_lenght(); ++i )
            {
                m_local_buffer[i] = static_cast<_Char_T>('\0');
            }
            m_buffer.release();

            m_is_local = true;
            m_current_lenght = m_npos;

            return;
        }
    };

    using String = basic_string<char>;
}
#endif // STRING_H
