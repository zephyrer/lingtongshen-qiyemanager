#pragma once

class CLock
{
public:
    virtual bool lock(void) = 0;
    virtual bool unlock(void) = 0;
};

class CCriticalSectionLock : public CLock
{
public:
    inline CCriticalSectionLock() 
    {
        InitializeCriticalSection(&m_cs);
    }
    inline virtual ~CCriticalSectionLock()
    {
        DeleteCriticalSection(&m_cs);
    }

    inline bool lock(void)
    {
        EnterCriticalSection(&m_cs);
        return true;
    }
    inline bool unlock(void)
    {
        LeaveCriticalSection(&m_cs);
        return true;
    }

private:
    CRITICAL_SECTION m_cs;
};

template <class LOCK_OBJ = CCriticalSectionLock>
class CLockableObject : public CLock
{
public:
    CLockableObject() {};
    virtual ~CLockableObject() {};

    bool lock(void)
    {   
        return m_LockObj.lock();
    }
    bool unlock(void)
    {
        return m_LockObj.unlock();
    }

protected:
    LOCK_OBJ m_LockObj;
};

template<class C, class L = CCriticalSectionLock>
class CLockable : public C, public CLockableObject<L>
{
public:
    CLockable() {};
    virtual ~CLockable() {};
};

class CAutoLock
{
public:
    inline CAutoLock(CLock * cLock) : m_Lock(cLock)
    {
        m_Lock->lock();
    }
    inline ~CAutoLock(void)
    {
        m_Lock->unlock();
    }
private:
    CAutoLock();
    CLock * m_Lock;
};

