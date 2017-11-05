#ifndef _MY_STRING_H
#define _MY_STRING_H

#include <cstring>
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
            if( m_current_lenght == m_npos )
                return;

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
            if( m_current_lenght == m_npos )
                return;

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
        explicit basic_string( const _Char_T * _left, size_t _size = m_npos ) noexcept
        {
            m_current_lenght = ( _size != m_npos ) ? _size : _strlen( _left );

            if( m_current_lenght == m_npos )
                return;

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
            memset( m_local_buffer, static_cast<_Char_T>('\0'), m_local_capasity );
            m_buffer.release();

            m_is_local = true;
            m_current_lenght = m_npos;

            return;
        }

        template<typename _T>
        friend std::ostream & operator<<( std::ostream & os, const basic_string<_T> & _left );
    };

    using String = basic_string<char>;

    template<typename _T>
    std::ostream & operator<<( std::ostream & os, const basic_string<_T> & _left )
    {
        if( _left.m_current_lenght == _left.m_npos )
            return os;

        if( _left.m_is_local )
        {
            os << _left.m_local_buffer;
        }
        else
        {
            os << _left.m_buffer.get();
        }

        return os;
    }
}
#endif // _MY_STRING_H
