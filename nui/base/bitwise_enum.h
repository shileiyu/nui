﻿#ifndef NUI_BASE_BITWISE_ENUM_H_
#define NUI_BASE_BITWISE_ENUM_H_

#include <nui/nui.h>

namespace nui
{

template<typename EnumClass>
struct BitwiseEnum : public EnumClass
{
    BitwiseEnum()
    {
        value_ = static_cast<typename EnumClass::Enum>(0);
    }

    BitwiseEnum(const typename EnumClass::Enum & value)
    {
        value_ = value;
    }

    BitwiseEnum & operator = (const typename EnumClass::Enum & value)
    {
        value_ = value;
        return *this;
    }

    operator typename EnumClass::Enum () const
    {
        return value_;
    }

    BitwiseEnum operator | (typename EnumClass::Enum value) const
    {
        auto result = static_cast<typename EnumClass::Enum>(value_ | value);
        return BitwiseEnum(result);
    }

    BitwiseEnum operator & (typename EnumClass::Enum value) const
    {
        auto result = static_cast<typename EnumClass::Enum>(value_ & value);
        return BitwiseEnum(result);
    }

    BitwiseEnum & operator |= (typename EnumClass::Enum value)
    {
        Set(value);
        return *this;
    }    

    void Set(typename EnumClass::Enum value)
    {
        value_ = static_cast<typename EnumClass::Enum>(value_ | value);
    }

    void Clear(typename EnumClass::Enum value)
    {
        value_ = static_cast<typename EnumClass::Enum>(value_ & ~value);
    }

    bool Test (typename EnumClass::Enum value) const
    {
        return (value_ & value) != 0;
    }

private:
    typename EnumClass::Enum value_;
};

}

#endif