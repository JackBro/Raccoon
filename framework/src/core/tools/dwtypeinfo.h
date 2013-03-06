
#pragma once

template <typename T>
class DwTypeInfo
{
public:
    enum
    {
        isPointer = false,
        isComplex = true,
        isStatic = false,		//QTypeInfo�е�ʵ��Ϊ isStatic = true��ΪЧ�����⣬������
        isLarge = (sizeof(T) > sizeof(void*)),
        isDummy = false
    };
};

template <typename T>
class DwTypeInfo<T*>
{
public:
    enum
    {
        isPointer = true,
        isComplex = false,
        isStatic = false,
        isLarge = false,
        isDummy = false
    };
};

//��ʹ�����¸�ʽ��ָ�����ͽ����ػ���
//DW_DECLARE_TYPEINFO(type, flags);
//����typeΪ������������flagsΪ����TYPEINFO flags��������ֵ

enum
{
    DW_COMPLEX_TYPE = 0,
    DW_PRIMITIVE_TYPE = 0x1,
    DW_STATIC_TYPE = 0,
    DW_MOVABLE_TYPE = 0x2,
    DW_DUMMY_TYPE = 0x4
};

#define DW_DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
class DwTypeInfo<TYPE > \
{ \
public: \
    enum { \
        isComplex = (((FLAGS) & DW_PRIMITIVE_TYPE) == 0), \
        isStatic = (((FLAGS) & (DW_MOVABLE_TYPE | DW_PRIMITIVE_TYPE)) == 0), \
        isLarge = (sizeof(TYPE)>sizeof(void*)), \
        isPointer = false, \
        isDummy = (((FLAGS) & DW_DUMMY_TYPE) != 0) \
    }; \
    static inline const char *name() { return #TYPE; } \
}

#define DW_DECLARE_TYPEINFO(TYPE, FLAGS) \
template<> \
DW_DECLARE_TYPEINFO_BODY(TYPE, FLAGS)


//�Ի����������͵�ģ���ػ�

DW_DECLARE_TYPEINFO(bool, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(char, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(signed char, DW_PRIMITIVE_TYPE);
//DW_DECLARE_TYPEINFO(unsigned char, DW_PRIMITIVE_TYPE);		//uint8_t
//DW_DECLARE_TYPEINFO(short, DW_PRIMITIVE_TYPE);				//int16_t
//DW_DECLARE_TYPEINFO(unsigned short, DW_PRIMITIVE_TYPE);		//uint16_t
//DW_DECLARE_TYPEINFO(int, DW_PRIMITIVE_TYPE);					//int32_t
//DW_DECLARE_TYPEINFO(unsigned int, DW_PRIMITIVE_TYPE);			//uint32_t
DW_DECLARE_TYPEINFO(long, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(unsigned long, DW_PRIMITIVE_TYPE);
//DW_DECLARE_TYPEINFO(__int64, DW_PRIMITIVE_TYPE);				//int64_t
//DW_DECLARE_TYPEINFO(unsigned __int64, DW_PRIMITIVE_TYPE);		//uint64_t
DW_DECLARE_TYPEINFO(float, DW_PRIMITIVE_TYPE);
//DW_DECLARE_TYPEINFO(double, DW_PRIMITIVE_TYPE);				//DwReal
DW_DECLARE_TYPEINFO(long double, DW_PRIMITIVE_TYPE);

//DW_TYPE
DW_DECLARE_TYPEINFO(uint8_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(uint16_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(uint32_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(uint64_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(int16_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(int32_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(int64_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(DwReal, DW_PRIMITIVE_TYPE);