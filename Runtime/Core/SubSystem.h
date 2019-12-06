#pragma once

#include "Helper.h"

namespace Mile
{
    class Context;
	/**
	 * @brief	������ �����ϴ� �⺻���� �ý���, ��ɵ��� ǥ���ϴ� Ŭ�����Դϴ�.
	 */
    class MEAPI SubSystem
    {
    public:
        SubSystem( Context* context );
        virtual ~SubSystem( )
        {
           DeInit( ); 
        }

        virtual bool Init( ) = 0;
        virtual void Update( ) { }
        virtual void DeInit( ) { m_bIsInitialized = false; }

        Context* GetContext( ) const { return m_context; }

        bool IsInitialized( ) const { return m_bIsInitialized; }

    protected:
        Context* m_context;
        bool     m_bIsInitialized;

    };
}