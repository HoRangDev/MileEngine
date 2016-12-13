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
    struct BaseChar
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
        inline CharType ToUpper( CharType Char );
        /**
        * ���ڸ� �ҹ��ڷ� ��ȯ��.
        * @param Char ��ȯ�� ����.
        */
        inline CharType ToLower( CharType Char );
        /**
        * ���ڰ� �빮������ �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsUpper( CharType Char );
        /**
        * ���ڰ� �ҹ������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsLower( CharType Char );
        /**
        * ���ڰ� ���ĺ����� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsAlpha( CharType Char );
        /**
        * ���ڰ� ���������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsPunct( CharType Char );
        /**
        * ���ڰ� ���ĺ� �Ǵ� �������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsAlnum( CharType Char );
        /**
        * ���ڰ� 10�������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsDigit( CharType Char );

        /**
        * ���ڰ� �������� �˻���.
        * @param Char �˻��� ����.
        */
        inline bool IsUnderscore( CharType Char )
        {
            return Char == LITERAL( CharType, '_' );
        }

    public:
        CharType Character;

    };

    template <>
    inline BaseChar<WIDECHAR>::CharType BaseChar<WIDECHAR>::ToUpper( CharType Char )
    {
        towupper( Char );
    }

    template <>
    inline BaseChar<WIDECHAR>::CharType BaseChar<WIDECHAR>::ToLower( CharType Char )
    {
        towlower( Char );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsUpper( CharType Char )
    {
        return static_cast<bool>( iswupper( Char ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsLower( CharType Char )
    {
        return static_cast<bool>( iswlower( Char ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsAlpha( CharType Char )
    {
        return static_cast<bool>( iswalpha( Char ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsPunct( CharType Char )
    {
        return static_cast<bool>( iswpunct( Char ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsAlnum( CharType Char )
    {
        return static_cast<bool>( iswalnum( Char ) );
    }

    template <>
    inline bool BaseChar<WIDECHAR>::IsDigit( CharType Char )
    {
        return static_cast<bool>( iswdigit( Char ) );
    }

    template <>
    inline BaseChar<ANSICHAR>::CharType BaseChar<ANSICHAR>::ToUpper( CharType Char )
    {
        toupper( Char );
    }

    template <>
    inline BaseChar<ANSICHAR>::CharType BaseChar<ANSICHAR>::ToLower( CharType Char )
    {
        tolower( Char );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsUpper( CharType Char )
    {
        return static_cast<bool>( isupper( Char ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsLower( CharType Char )
    {
        return static_cast<bool>( islower( Char ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsAlpha( CharType Char )
    {
        return static_cast<bool>( isalpha( Char ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsPunct( CharType Char )
    {
        return static_cast<bool>( ispunct( Char ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsAlnum( CharType Char )
    {
        return static_cast<bool>( isalnum( Char ) );
    }

    template <>
    inline bool BaseChar<ANSICHAR>::IsDigit( CharType Char )
    {
        return static_cast<bool>( isdigit( Char ) );
    }

    /** Mile Engine(API)�� �⺻ ����Ÿ�� */
    using MChar = BaseChar<WIDECHAR>;
}