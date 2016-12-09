#pragma once
#include "Mile.h"
#include "string"

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
		/**
		* ���ڸ� �빮�ڷ� ��ȯ��.
		* @param Char ��ȯ�� ����.
		*/
		static inline CharType ToUpper( CharType Char );
		/**
		* ���ڸ� �ҹ��ڷ� ��ȯ��.
		* @param Char ��ȯ�� ����.
		*/
		static inline CharType ToLower( CharType Char );
		/**
		* ���ڰ� �빮������ �˻���.
		* @param Char �˻��� ����.
		*/
		static inline bool IsUpper( CharType Char );
		/**
		* ���ڰ� �ҹ������� �˻���.
		* @param Char �˻��� ����.
		*/
		static inline bool IsLower( CharType Char );
		/**
		* ���ڰ� ���ĺ����� �˻���.
		* @param Char �˻��� ����.
		*/
		static inline bool IsAlpha( CharType Char );
		/**
		* ���ڰ� ���������� �˻���.
		* @param Char �˻��� ����.
		*/
		static inline bool IsPunct( CharType Char );
		/**
		* ���ڰ� ���ĺ� �Ǵ� �������� �˻���.
		* @param Char �˻��� ����.
		*/
		static inline bool IsAlnum( CharType Char );
		/**
		* ���ڰ� 10�������� �˻���.
		* @param Char �˻��� ����.
		*/
		static inline bool IsDigit( CharType Char );

		/**
		* ���ڰ� �������� �˻���.
		* @param Char �˻��� ����.
		*/
		static inline bool IsUnderscore( CharType Char )
		{
			return Char == LITERAL( CharType, '_' );
		}
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
		return iswupper( Char );
	}

	template <>
	inline bool BaseChar<WIDECHAR>::IsLower( CharType Char )
	{
		return iswlower( Char );
	}

	template <>
	inline bool BaseChar<WIDECHAR>::IsAlpha( CharType Char )
	{
		return iswalpha( Char );
	}

	template <>
	inline bool BaseChar<WIDECHAR>::IsPunct( CharType Char )
	{
		return iswpunct( Char );
	}

	template <>
	inline bool BaseChar<WIDECHAR>::IsAlnum( CharType Char )
	{
		return iswalnum( Char );
	}

	template <>
	inline bool BaseChar<WIDECHAR>::IsDigit( CharType Char )
	{
		return iswdigit( Char );
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
		return isupper( Char );
	}

	template <>
	inline bool BaseChar<ANSICHAR>::IsLower( CharType Char )
	{
		return islower( Char );
	}

	template <>
	inline bool BaseChar<ANSICHAR>::IsAlpha( CharType Char )
	{
		return isalpha( Char );
	}

	template <>
	inline bool BaseChar<ANSICHAR>::IsPunct( CharType Char )
	{
		return ispunct( Char );
	}

	template <>
	inline bool BaseChar<ANSICHAR>::IsAlnum( CharType Char )
	{
		return isalnum( Char );
	}

	template <>
	inline bool BaseChar<ANSICHAR>::IsDigit( CharType Char )
	{
		return isdigit( Char );
	}

	/** Mile Engine(API)�� �⺻ ����Ÿ�� */
	using MChar = BaseChar<MCHAR>;
}