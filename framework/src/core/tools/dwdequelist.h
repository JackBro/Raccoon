
#pragma once

#include "dwcore/dwmalloc.h"

class ChunkCache;

class DW_CORE_EXPORT DwDequeList
{
public:
    class Chunk;

    class DW_CORE_EXPORT Iterator
    {
    public:
        friend class DwDequeList;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

    public:
        Iterator(Chunk* ptr = NULL, int slotid = -1, int id = -1);
        Iterator(const Iterator& other);
        int index() const;
        void** address() const;

    protected:
        difference_type distance(const Iterator& other) const;
        bool equal(const Iterator& other) const;
        bool less(const Iterator& other) const;
        bool greater(const Iterator& other) const;
        void increase();
        void decrease();
        void advance(difference_type n);
        void recede(difference_type n);

    private:
        bool isBegin();
        bool isEnd();

    private:
        Chunk* m_pChunk;							//���������ڵ�chunk
        int m_slotID;								//��������m_curָ���chunk�е�����
        int m_id;									//��¼��������ArrayList�е�����
    };

    class Chunk
    {
    private:
        friend class DwDequeList;
        friend class DwDequeList::Iterator;

    private:
        void initialize();
        int count() const;
        bool empty() const;							//m_endΪm_beginʱ���
        bool full() const;							//m_end���¸�����Ϊm_beginʱ����
        int incIndex(int curID, int n = 1) const;
        int decIndex(int curID, int n = 1) const;
        void** incIndex(void** curPtr, int n = 1);
        void** decIndex(void** curPtr, int n = 1);
        int backCount(int curID) const;
        int frontCount(int curID) const;
        void** index(int n);						//��ǰ���ĵ�i������
        int offset(int beforeID, int afterID) const;//����beforeID��afterID���ƫ����

    private:
        /*
        	----------------------------
        	ms_bufSize ���ȶ��ձ�
        	----------------------------
        	(1 << 4) - 1	->	15
        	(1 << 5) - 1	->	31
        	(1 << 6) - 1	->	64
        	(1 << 7) - 1	->	127
        	(1 << 8) - 1	->	255
        	(1 << 9) - 1	->	511
        	(1 << 10) - 1	->	1023
        	(1 << 11) - 1	->	2047
        	(1 << 12) - 1	->	4095
        	(1 << 13) - 1	->	8191
        	(1 << 14) - 1	->	16383
        	(1 << 15) - 1	->	32767
        	(1 << 16) - 1	->	65535
        	(1 << 17) - 1	->	131071
        	(1 << 18) - 1	->	262143
        	(1 << 19) - 1	->	524287
        	(1 << 20) - 1	->	1048575
        */
        static const int ms_bufSize = (1 << 9) - 1;	//ÿ��chunk�е�Ԫ�ظ���
        int m_begin;
        int m_end;									//���һ��Ԫ�ص���һ��λ��
        Chunk* m_prev;
        Chunk* m_next;
        void* m_buf[1];
    };

    enum
    {
        ChunkSize = sizeof(Chunk) - sizeof(void*)
    };

public:
    //����������
    DwDequeList();
    ~DwDequeList();

    //�������
    Iterator append();
    Iterator prepend();
    Iterator insert(const Iterator& it);

    //ɾ�����
    void removeFirst();
    void removeLast();
    Iterator remove(Iterator it);
    void clear();

    //�������
    bool isEmpty() const;
    int count() const;					//Ԫ����
    int size() const;					//ռ���ڴ���
    Iterator begin() const;
    Iterator end() const;
    Iterator begin();
    Iterator end();
    Iterator first() const;
    Iterator last() const;
    Iterator at(int id) const;
    Iterator createIterator(int id) const;

private:
    DW_DISABLE_COPY(DwDequeList);

    Chunk* allocate();					//����ռ䲢��ʼ��
    void deallocate(Chunk* ptr);
    bool validIterator(const Iterator& it);
    Chunk* halveNode(Chunk* pChunk, int& offset, int offsetID);
    Chunk* searchBefore(Chunk* pChunk, int beforeCount, int& offset) const;
    Chunk* searchAfter(Chunk* pChunk, int& offset) const;

private:
    Chunk* m_head;						//ָ��ͷ���
    Chunk* m_tail;						//ָ��β���
    int m_count;						//��Ԫ�ظ���
    int m_nodeCount;					//������
    ChunkCache* m_lastAccess;			//����������ʵĽ��
};

//////////////////////////////////////////////////////////////////////////
//DwDequeList::Iterator
//////////////////////////////////////////////////////////////////////////

inline DwDequeList::Iterator::Iterator(DwDequeList::Chunk* ptr, int slotid, int id)
    : m_pChunk(ptr), m_slotID(slotid), m_id(id)
{

}

inline DwDequeList::Iterator::Iterator(const DwDequeList::Iterator& other)
    : m_pChunk(other.m_pChunk), m_slotID(other.m_slotID), m_id(other.m_id)
{

}

inline int DwDequeList::Iterator::index() const
{
    return m_id;
}

inline void** DwDequeList::Iterator::address() const
{
    DW_ASSERT(NULL != m_pChunk);
    return m_pChunk->m_buf + m_slotID;
}

inline DwDequeList::Iterator::difference_type DwDequeList::Iterator::distance(const Iterator& other) const
{
    return m_id - other.m_id;
}

inline bool DwDequeList::Iterator::equal(const Iterator& other) const
{
    return (m_pChunk == other.m_pChunk) && (m_slotID == other.m_slotID) && (m_id == other.m_id);
}

inline bool DwDequeList::Iterator::less(const Iterator& other) const
{
    return NULL != m_pChunk &&  NULL != other.m_pChunk && m_id < other.m_id;
}

inline bool DwDequeList::Iterator::greater(const Iterator& other) const
{
    return NULL != m_pChunk &&  NULL != other.m_pChunk && m_id > other.m_id;
}

inline void DwDequeList::Iterator::increase()
{
    if (NULL != m_pChunk && !isEnd())	//��Ϊ�����е�endʱ���ƶ�������ƶ���end
    {
        ++m_id;
        m_slotID = m_pChunk->incIndex(m_slotID);
        if (m_slotID == m_pChunk->m_end && NULL != m_pChunk->m_next)
        {
            m_pChunk = m_pChunk->m_next;
            m_slotID = m_pChunk->m_begin;
        }
    }
}

inline void DwDequeList::Iterator::decrease()
{
    if (NULL != m_pChunk && !isBegin())		//��Ϊ�����е�beginʱ���ƶ�
    {
        --m_id;
        if (m_slotID != m_pChunk->m_begin)
        {
            m_slotID = m_pChunk->decIndex(m_slotID);
        }
        else
        {
            DW_ASSERT(NULL != m_pChunk->m_prev);
            m_pChunk = m_pChunk->m_prev;
            m_slotID = m_pChunk->decIndex(m_pChunk->m_end);
        }
    }
}

inline bool DwDequeList::Iterator::isBegin()
{
    DW_ASSERT(NULL != m_pChunk);
    return (NULL == m_pChunk->m_prev) && (m_slotID == m_pChunk->m_begin);
}

inline bool DwDequeList::Iterator::isEnd()
{
    DW_ASSERT(NULL != m_pChunk);
    return (NULL == m_pChunk->m_next) && (m_slotID == m_pChunk->m_end);
}

//////////////////////////////////////////////////////////////////////////
//DwDequeList::Chunk
//////////////////////////////////////////////////////////////////////////

inline void DwDequeList::Chunk::initialize()
{
    m_begin = m_end = 0;
    m_prev = m_next = NULL;
}

//ע�⣺����Ϊms_bufSize����ʵ����ms_bufSize + 1�������һ�����ڸ����ж϶����Ƿ�����
inline int DwDequeList::Chunk::count() const
{
    return offset(m_begin, m_end);
}

inline bool DwDequeList::Chunk::empty() const
{
    return m_begin == m_end;
}

inline bool DwDequeList::Chunk::full() const
{
    return m_begin == incIndex(m_end);
}

inline int DwDequeList::Chunk::incIndex(int curID, int n) const
{
    return (curID + n) & ms_bufSize;
}

inline int DwDequeList::Chunk::decIndex(int curID, int n) const
{
    return (curID + ms_bufSize + 1 - n) & ms_bufSize;
}

inline void** DwDequeList::Chunk::incIndex(void** curPtr, int n)	//��ǰָ���ĵ�n��ָ�루���ڣ�
{
    return m_buf + incIndex(curPtr - m_buf, n);
}

inline void** DwDequeList::Chunk::decIndex(void** curPtr, int n)
{
    return m_buf + decIndex(curPtr - m_buf, n);
}

//��ǰChunk�У�curID���Ԫ�ظ�����������curID����
inline int DwDequeList::Chunk::backCount(int curID) const
{
    DW_ASSERT(!empty());
    return (m_end + ms_bufSize - curID) & ms_bufSize;
}

//��ǰChunk�У�curIDǰ��Ԫ�ظ�����������curID����
inline int DwDequeList::Chunk::frontCount(int curID) const
{
    DW_ASSERT(!empty());
    return (curID + ms_bufSize + 1 - m_begin) & ms_bufSize;
}

inline void** DwDequeList::Chunk::index(int i)
{
    DW_ASSERT(i >= 0 && i < count());
    return m_buf + incIndex(m_begin, i);
}

inline int DwDequeList::Chunk::offset(int beforeID, int afterID) const
{
    return (afterID + ms_bufSize + 1 - beforeID) & ms_bufSize;
}

//////////////////////////////////////////////////////////////////////////
//DwDequeList
//////////////////////////////////////////////////////////////////////////

inline DwDequeList::DwDequeList()
    : m_count(0), m_nodeCount(0)
    , m_head(NULL), m_tail(NULL), m_lastAccess(NULL)
{

}

inline DwDequeList::~DwDequeList()
{
    clear();
}

inline DwDequeList::Chunk* DwDequeList::allocate()
{
    DW_ASSERT(Chunk::ms_bufSize >= 1);
    //��δ�������쳣�Ĵ���
    Chunk* pChunk = static_cast<Chunk*>(dwMalloc(ChunkSize + sizeof(void*) * (Chunk::ms_bufSize + 1)));
    ++m_nodeCount;
    pChunk->initialize();
    return pChunk;
}

inline void DwDequeList::deallocate(DwDequeList::Chunk* ptr)
{
    //��δ�������쳣�Ĵ���
    dwFree(static_cast<void*>(ptr));
    --m_nodeCount;
}

inline bool DwDequeList::validIterator(const Iterator& it)
{
    if(NULL != it.m_pChunk)
    {
        return (it.m_id >= 0 && it.m_id < m_count);
    }
    return (-1 != it.m_id && -1 != it.m_slotID);
}

inline bool DwDequeList::isEmpty() const
{
    return (NULL == m_head) && (NULL == m_tail) && (0 == m_count) && (0 == m_nodeCount);
}

inline int DwDequeList::count() const
{
    return m_count;
}

inline int DwDequeList::size() const
{
    return m_nodeCount * (ChunkSize + sizeof(void*) * (Chunk::ms_bufSize + 1));
}

inline DwDequeList::Iterator DwDequeList::begin()
{
    if (0 != m_count)
    {
        return Iterator(m_head, m_head->m_begin, 0);
    }
    return Iterator();
}

inline DwDequeList::Iterator DwDequeList::end()
{
    if (0 != m_count)
    {
        return Iterator(m_tail, m_tail->m_end, m_count);
    }
    return Iterator();
}

inline DwDequeList::Iterator DwDequeList::begin() const
{
    if (0 != m_count)
    {
        return Iterator(m_head, m_head->m_begin, 0);
    }
    return Iterator();
}

inline DwDequeList::Iterator DwDequeList::end() const
{
    if (0 != m_count)
    {
        return Iterator(m_tail, m_tail->m_end, m_count);
    }
    return Iterator();
}

inline DwDequeList::Iterator DwDequeList::first() const
{
    if (0 != m_count)
    {
        return Iterator(m_head, m_head->m_begin, 0);
    }
    return Iterator();
}

inline DwDequeList::Iterator DwDequeList::last() const
{
    if (0 != m_count)
    {
        return Iterator(m_tail, m_tail->decIndex(m_tail->m_end), m_count - 1);
    }
    return Iterator();
}

inline DwDequeList::Iterator DwDequeList::at(int id) const
{
    DW_ASSERT(id >= 0 && id < m_count);
    return createIterator(id);
}