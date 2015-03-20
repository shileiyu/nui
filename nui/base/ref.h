#ifndef NUI_BASE_REF_COUNT_H_
#define NUI_BASE_REF_COUNT_H_

#include <nui/nui.h>

namespace nui
{

class RefCountBase
{
protected:
    RefCountBase()
        : ref_count_(1)
    {
        memset(tag_, 0, kTagCapcity);
        strncpy_s(tag_, "RefCntObj", kMaxTagSize);
    }

    virtual ~RefCountBase() {};

    void Dispose() const
    {
        delete this;
    }

protected:
    //disallow cpoy and assign
    RefCountBase(const RefCountBase &);
    RefCountBase & operator=(const RefCountBase &);


    static const size_t kMaxTagSize = 16;
    static const size_t kTagCapcity = 20;

    char tag_[kTagCapcity];
    volatile mutable long ref_count_;
};

class RefCount : public RefCountBase
{
public:
    int IncRef() const
    {
        return ++ref_count_;
    }

    int DecRef() const
    {
        int decreased_value = --ref_count_;
        if (decreased_value == 0)
            this->Dispose();
        return decreased_value;
    }
protected:
    ~RefCount() {};
};


class ThreadSafeRefCount : public RefCountBase
{
public:
    int IncRef() const
    {
        return _InterlockedIncrement(&ref_count_);
    }

    int DecRef() const
    {
        int decreased_value = _InterlockedDecrement(&ref_count_);
        if (decreased_value == 0)
            this->Dispose();
        return decreased_value;
    }
protected:
    ~ThreadSafeRefCount() {};
};

template<typename T>
class ScopedRef
{
    typedef typename T Object;
public:

    ScopedRef() : object_(nullptr) {};

    ScopedRef(const ScopedRef & other) 
        : object_(nullptr)
    {
        Reset(other.object_);
    };

    ScopedRef(Object * object)
        : object_(nullptr)
    {
        Reset(object);
    };

    template<typename Derived>
    ScopedRef(const ScopedRef<Derived> & other)
        : object_(nullptr)
    {
        Reset(other.Get());
    }

    ~ScopedRef() 
    {
        Reset();
    }

    Object * operator->()
    {
        return object_;
    }

    const Object * operator->() const
    {
        return object_;
    }

    ScopedRef & operator=(const ScopedRef & other)
    {
        Reset(other.object_);
        return *this;
    }

    ScopedRef & operator=(Object * object)
    {
        Reset(object);
        return *this;
    }

    template<typename Derived>
    ScopedRef & operator=(const ScopedRef<Derived> & other)
    {
        Reset(other.Get());
        return *this;
    }

    bool operator == (const ScopedRef & other) const
    {
        return this->object_ == other.object_;
    }

    bool operator != (const ScopedRef & other) const
    {
        return this->object_ != other.object_;
    }

    bool operator == (const Object * object) const
    {
        return this->object_ == object;
    }

    bool operator != (const Object * object) const
    {
        return this->object_ != object;
    }
    
    Object * Get () const
    {
        return object_;
    }

    operator bool() const
    {
        return IsValid();
    }

    void Reset()
    {
        Reset(nullptr);
    }

    void Reset(Object * object)
    {
        if (object_)
            object_->DecRef();
        object_ = object;
        if (object_)
            object_->IncRef();
        return;
    }

    bool IsValid() const
    {
        return object_ != nullptr;
    }

private:
    Object * object_;
};

template<typename T>
ScopedRef<T> Adopt(T * object)
{
    ScopedRef<T> ref;
    ref.Reset(object);
    if(ref)
        ref->DecRef();
    return ref;
}

}

#endif