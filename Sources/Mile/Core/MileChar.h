#pragma once
#include "Mile.h"

namespace Mile
{
    /**
    * ���ͷ� ����ü�� �����Ǵ� ����Ÿ�Կ� ���� ANSI �Ǵ� WIDE ���ڿ� ���ͷ��� �����Ͽ� ��ȯ���ݴϴ�.
    */
    template <typename T>
    struct Literal
    {
        static const ANSICHAR Select( const ANSICHAR Ansi, const WIDECHAR )
        {
            return Ansi;
        }

        static const ANSICHAR* Select( const ANSICHAR* Ansi, const WIDECHAR* )
        {
            return Ansi;
        }
    };

    template <>
    struct Literal<WIDECHAR>
    {
        static const WIDECHAR Select( const ANSICHAR, const WIDECHAR Wide )
        {
            return Wide;
        }

        static const WIDECHAR* Select( const ANSICHAR*, const WIDECHAR* Wide )
        {
            return Wide;
        }
    };

#define LITERAL(CharType, Char) Literal<CharType>::Select(Char, L##Char)

    /**
    * �������� ����(Wrapper)�� �������� �߰� ��ɵ��� �����Ͽ��ش�.
    */
    template <typename T>
    struct MILE_API BaseChar
    {
        using CharType = T;
    public:
        BaseChar( )
        {
        }

        BaseChar( CharType Char ) :
            Character( Char )
        {
        }

        BaseChar& operator=( const BaseChar& Value )
        {
            Character = Value.Character;
            return ( *this );
        }

        BaseChar& operator=( CharType Value )
        {
            Character = Value;
            return ( *this );
        }

        bool operator==( CharType Value ) const
        {
            return ( Character == Value );
        }

        bool operator!=( CharType Value ) const
        {
            return ( Character != Value );
        }

        bool operator==( const BaseChar& Value ) const
        {
            return ( Character == Value.Character );
        }

        bool operator!=( const BaseChar& Value ) const
        {
            return ( Character == Value.Character );
        }

        operator CharType( ) const
        {
            return Character;
        }

        /**
        * ���ڸ� �빮�ڷ� ��ȯ��.
        * @param Char ��ȯ�� ����.
        */
        inline CharType ToUpper( );
        /**
        * ���ڸ� �ҹ��ڷ� ��ȯ��.
        * @param Char ��ȯ�� ����.
        */
        inline CharType ToLower( );
        /**
        * ���ڰ� �빮������ �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsUpper( );
        /**
        * ���ڰ� �ҹ������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsLower( );
        /**
        * ���ڰ� ���ĺ����� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsAlpha( );
        /**
        * ���ڰ� ���������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsPunct( );
        /**
        * ���ڰ� ���ĺ� �Ǵ� �������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsAlnum( );
        /**
        * ���ڰ� 10�������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsDigit( );

        /**
        * ���ڰ� �������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsUnderscore( )
        {
            return Character == LITERAL( CharType, '_' );
        }

    public:
        CharType Character;

    };

    template <>
    inline BaseChar<WIDECHAR>::CharType BaseChar<WIDECHAR>::ToUpper( )
    {
        return towupper( Character );
    }

    template <>
    inline BaseChar<WIDECHAR>::CharType BaseChar<WIDECHAR>::ToLower( )
    {
        return towlower( Character );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsUpper( )
    {
        return static_cast< bool >( iswupper( Character ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsLower( )
    {
        return static_cast< bool >( iswlower( Character ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsAlpha( )
    {
        return static_cast< bool >( iswalpha( Character ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsPunct( )
    {
        return static_cast< bool >( iswpunct( Character ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsAlnum( )
    {
        return static_cast< bool >( iswalnum( Character ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsDigit( )
    {
        return static_cast< bool >( iswdigit( Character ) );
    }

    template <>
    inline BaseChar<ANSICHAR>::CharType BaseChar<ANSICHAR>::ToUpper( )
    {
        return toupper( Character );
    }

    template <>
    inline BaseChar<ANSICHAR>::CharType BaseChar<ANSICHAR>::ToLower( )
    {
        return tolower( Character );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsUpper( )
    {
        return static_cast< bool >( isupper( Character ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsLower( )
    {
        return static_cast< bool >( islower( Character ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsAlpha( )
    {
        return static_cast< bool >( isalpha( Character ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsPunct( )
    {
        return static_cast< bool >( ispunct( Character ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsAlnum( )
    {
        return static_cast< bool >( isalnum( Character ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsDigit( )
    {
        return static_cast< bool >( isdigit( Character ) );
    }

    /** Mile Engine(API)�� �⺻ ����Ÿ�� */
    using MChar = BaseChar<WIDECHAR>;
}