#ifndef _MY_STRING_H
#define _MY_STRING_H

#include <cstring>
#include <crtdefs.h>
#include <memory>
#include <iostream>
#include <stdexcept>

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

        // указатель на буфер
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

        // перемещение из локального буфера
        void _move_from_local( const size_t _size = m_npos )
        {
            size_t new_size = ( m_npos == _size ) ? _local_lenght() * 2 : _size;
            m_buffer = std::make_unique<_Char_T[]>( new_size );
            memcpy( m_buffer.get(), m_local_buffer, m_local_capasity );
            m_is_local = false;
            m_capacity = new_size;
        }

        // перемещение в локальный буфер
        void _move_to_local()
        {
            size_t new_size = ( m_current_lenght < _local_lenght() ) ? m_current_lenght : _local_lenght();
            memcpy( m_local_buffer, m_buffer.get(), new_size * sizeof( _Char_T ) );
            m_is_local = true;
            m_current_lenght = new_size;
            m_buffer.release();
        }

        // изменение размера буфера
        void resize_buffer( size_t _size = m_npos )
        {
            size_t new_size = ( m_npos == _size ) ? m_capacity * 2 : _size;
            m_capacity = new_size;
            std::unique_ptr<_Char_T[]> _buffer{ std::make_unique<_Char_T[]>( m_capacity ) };
            if( nullptr != m_buffer )
                memcpy( _buffer.get(), m_buffer.get(), m_capacity * sizeof( _Char_T ) );
            m_buffer = std::move( _buffer );
        }

    public:
        // деструктор и стандартный заработает
        ~basic_string() = default;

        // конструктор по умолчанию
        explicit basic_string() noexcept {}

        // копирующий конструктор
        basic_string( const basic_string & _right ) noexcept:
            m_current_lenght( _right.m_current_lenght ),
            m_is_local( _right.m_is_local )
        {
            if( m_current_lenght == m_npos )
                return;

            if( m_is_local )
            {
                memcpy( m_local_buffer, _right.m_local_buffer, m_local_capasity );
            }
            else
            {
                m_capacity = _right.m_capacity;
                m_buffer = std::make_unique<_Char_T[]>( m_capacity );
                memcpy( m_buffer.get(), _right.m_buffer.get(), m_capacity * sizeof( _Char_T ) );
            }
        }

        // перемещающий конструктор
        basic_string( basic_string && _right ) noexcept:
            m_current_lenght( std::move( _right.m_current_lenght ) ),
            m_is_local( std::move( _right.m_is_local ) )
        {
            if( m_current_lenght == m_npos )
                return;

            if( m_is_local )
            {
                // тут проще скопировать
                memcpy( m_local_buffer, _right.m_local_buffer, m_local_capasity );
            }
            else
            {
                // а тут переместить
                m_capacity = std::move( _right.m_capacity );
                m_buffer = std::move( _right.m_buffer );
            }
            _right.reset();
        }

        // конструктор из с-строк
        basic_string( const _Char_T * _right, size_t _size = m_npos ) noexcept
        {
            m_current_lenght = ( _size != m_npos ) ? _size : strlen( _right );

            if( m_current_lenght == m_npos )
                return;

            if( m_current_lenght < _local_lenght() )
            {
                memcpy( m_local_buffer, _right, m_current_lenght * sizeof( _Char_T ) );
            }
            else
            {
                m_capacity = m_current_lenght + 1;
                m_buffer = std::make_unique<_Char_T[]>( m_capacity );
                memcpy( m_buffer.get(), _right, m_capacity * sizeof( _Char_T ) );
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

        // оператор прямого доступа
        inline _Char_T & operator []( size_t _index )
        {
            if( _index > m_current_lenght )
                throw std::out_of_range("basic_string:index out of range");

            return m_is_local ? m_local_buffer[ _index ] : m_buffer[ _index ];
        }

        // оператор присваивания с копированием
        inline basic_string & operator = ( const basic_string & _right )
        {
            if (this == &_right)
            {
                return *this;
            }
            reset();
            m_is_local = _right.m_is_local;
            m_current_lenght = _right.m_current_lenght;
            memcpy( m_local_buffer, _right.m_local_buffer, m_local_capasity );

            if( !m_is_local )
            {
                resize_buffer( m_capacity );
                memcpy( m_buffer.get(), _right.m_buffer.get(), m_current_lenght * sizeof( _Char_T ) );
            }
            return *this;
        }

        // оператор присваивания с перемещением
        inline basic_string & operator = ( basic_string && _right )
        {
            if (this == &_right)
            {
                return *this;
            }
            reset();
            m_is_local = std::move( _right.m_is_local );
            m_current_lenght = std::move( _right.m_current_lenght );
            memcpy( m_local_buffer, _right.m_local_buffer, m_local_capasity );

            if( !m_is_local )
            {
                m_buffer = std::move( _right.m_buffer );
            }
            _right.reset();
            return *this;
        }

        inline basic_string operator + ( const basic_string & _right ) const
        {
            size_t new_size = m_current_lenght + _right.m_current_lenght;
            std::unique_ptr<_Char_T[]> _buffer = std::make_unique<_Char_T[]>( new_size + 1 );
            if( m_is_local )
            {
                memcpy( _buffer.get(), m_local_buffer, m_current_lenght );
            }
            else
            {
                memcpy( _buffer.get(), m_buffer.get(), m_current_lenght );
            }

            if( _right.m_is_local )
            {
                memcpy( _buffer.get() + m_current_lenght, _right.m_local_buffer, _right.m_current_lenght );
            }
            else
            {
                memcpy( _buffer.get() + m_current_lenght, _right.m_buffer.get(), _right.m_current_lenght );
            }

            return std::move( basic_string( _buffer.get(), new_size ) );
        }

        inline basic_string & operator += ( const basic_string & _right )
        {
            *this = *this + _right;
            return *this;
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
