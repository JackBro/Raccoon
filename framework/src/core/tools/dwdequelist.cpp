
#include "dwdequelist.h"

//////////////////////////////////////////////////////////////////////////
//ChunkCache
//////////////////////////////////////////////////////////////////////////

class ChunkCache
{
private:
    friend class DwDequeList;

public:
    bool isHit(int id);
    void update(DwDequeList::Chunk* ptr, int minid, int maxid);

private:
    DwDequeList::Chunk* m_pChunk;
    int m_minID;
    int m_maxID;
};

inline bool ChunkCache::isHit(int id)
{
    return (m_pChunk != NULL && id >= m_minID && id <= m_maxID);
}

inline void ChunkCache::update(DwDequeList::Chunk* ptr, int minid, int maxid)
{
    m_pChunk = ptr;
    m_minID = minid;
    m_maxID = maxid;
}

//////////////////////////////////////////////////////////////////////////
//DwDequeList::Iterator
//////////////////////////////////////////////////////////////////////////

void DwDequeList::Iterator::advance(DwDequeList::Iterator::difference_type n)
{
    if (n > 0 && (NULL != m_pChunk || !isEnd()))
    {
        int backcount = m_pChunk->backCount(m_slotID);
        while (n > backcount && NULL != m_pChunk->m_next)
        {
            m_pChunk = m_pChunk->m_next;
            n = n - backcount - 1;				//�����̽�����Ԫ��ʱӦ�ų���Ԫ�ر���
            m_id = m_id + backcount + 1;
            m_slotID = m_pChunk->m_begin;
            backcount = m_pChunk->backCount(m_slotID);
        }
        if (n <= backcount)						//����nΪ����ڵ�ƫ��������m_idΪ�������Ԫ������
        {
            m_id += n;
            m_slotID = m_pChunk->incIndex(m_slotID, n);
        }
        else									//��ƫ����n��������m_countʱ����ָ��end
        {
            DW_ASSERT(NULL == m_pChunk->m_next);
            //ע������ֱ�Ӽ���m_pChunk->count()��Ϊ�����ʱ����ֻ��һ�����ʱm_id������Ԫ������
            m_id = m_id + m_pChunk->backCount(m_slotID) + 1;
            m_slotID = m_pChunk->m_end;
        }
    }
}

void DwDequeList::Iterator::recede(DwDequeList::Iterator::difference_type n)
{
    if (n > 0 && (NULL != m_pChunk || !isBegin()))
    {
        int frontcount = m_pChunk->frontCount(m_slotID);
        while (n > frontcount && NULL != m_pChunk->m_prev)
        {
            m_pChunk = m_pChunk->m_prev;
            n = n - frontcount - 1;				//����ǰ��������Ԫ��ʱӦ�ų����һ��Ԫ�ر���
            m_id = m_id - frontcount - 1;		//m_idΪ��ǰ����βԪ������
            m_slotID = m_pChunk->decIndex(m_pChunk->m_end);
            frontcount = m_pChunk->frontCount(m_slotID);
        }
        if (n <= frontcount)
        {
            m_id -= n;
            m_slotID = m_pChunk->decIndex(m_slotID, n);
        }
        else
        {
            DW_ASSERT(NULL == m_pChunk->m_prev);	//��n���ڵ�ǰm_id֮ǰ��Ԫ������ʱ����ָ��begin
            m_id = 0;
            m_slotID = m_pChunk->m_begin;		//ָ����Ԫ��
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//DwDequeList
//////////////////////////////////////////////////////////////////////////

DwDequeList::Chunk* DwDequeList::halveNode(Chunk* pChunk, int& offset, int offsetID)
{
    offsetID;
    DW_ASSERT(pChunk->full());
    int moveCount = 0;
    int midOffset = pChunk->count() / 2;
    int midID = pChunk->incIndex(pChunk->m_begin, midOffset);
    Chunk* newNode = allocate();
    if (offset <= pChunk->count() - offset)	//offset��ǰһ�룬��newNode����pChunkǰ����ʱ�����޸�offset
    {
        newNode->m_prev = pChunk->m_prev;
        newNode->m_next = pChunk;
        if(NULL != newNode->m_prev)
        {
            newNode->m_prev->m_next = newNode;
        }
        pChunk->m_prev = newNode;
        int newNodeID = newNode->m_begin;
        int pChunkID = pChunk->m_begin;
        moveCount = offset;
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;					//ע��newNodeID����ѭ��������һ�����ᳬ���������߽�
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == offsetID && newNodeID == offset);
        ++newNodeID;						//δ����ǰnewNodeIDΪ����λ��
        moveCount = midOffset - offset;
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == midID && newNodeID == midOffset + 1);
        newNode->m_end = newNodeID;
        pChunk->m_begin = pChunkID;
        if (pChunk == m_head)
        {
            m_head = newNode;
        }
    }
    else									//offset�ں�һ�룬��newNode����pChunk�󣬴�ʱ�����޸�offset
    {
        newNode->m_prev = pChunk;
        newNode->m_next = pChunk->m_next;
        if(NULL != newNode->m_next)
        {
            newNode->m_next->m_prev = newNode;
        }
        pChunk->m_next = newNode;
        offset = offset - midOffset - 1;	//������ں�һ��ʱ����⴦��offset
        int newNodeID = newNode->m_begin;
        int pChunkID = pChunk->incIndex(midID);
        moveCount = offset;
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == offsetID && newNodeID == offset);
        ++newNodeID;
        moveCount = pChunk->count() - (midOffset + 1 + offset);
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == pChunk->m_end && newNodeID == pChunk->count() - midOffset);
        pChunk->m_end = pChunk->incIndex(midID);
        newNode->m_end = newNodeID;
        if (pChunk == m_tail)
        {
            m_tail = newNode;
        }
    }
    return newNode;
}

DwDequeList::Iterator DwDequeList::prepend()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_head);
        DW_ASSERT(NULL != m_lastAccess);
        if (m_head->full())
        {
            //Ϊ���append���ܣ�ͷ�������ʱ�������Ѵ�������ֱ����֮ǰ�����½��
            Chunk* newNode = allocate();
            newNode->m_next = m_head;
            m_head->m_prev = newNode;
            m_head = newNode;
        }
    }
    else
    {
        m_tail = m_head = allocate();
        m_lastAccess = static_cast<ChunkCache*>(dwMalloc(sizeof(ChunkCache)));
    }
    m_head->m_begin = m_head->decIndex(m_head->m_begin);
    ++m_count;
    m_lastAccess->update(m_head, 0, m_head->count() - 1);
    return Iterator(m_head, m_head->m_begin, 0);
}

DwDequeList::Iterator DwDequeList::append()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_tail);
        DW_ASSERT(NULL != m_lastAccess);
        if (m_tail->full())
        {
            //Ϊ���prepend���ܣ�β�������ʱ�������Ѵ�������ֱ����֮�󴴽��½��
            Chunk* newNode = allocate();
            newNode->m_prev = m_tail;
            m_tail->m_next = newNode;
            m_tail = newNode;
        }
    }
    else
    {
        m_tail = m_head = allocate();
        m_lastAccess = static_cast<ChunkCache*>(dwMalloc(sizeof(ChunkCache)));
    }
    int listTailID = m_tail->m_end;
    m_tail->m_end = m_tail->incIndex(m_tail->m_end);
    ++m_count;
    m_lastAccess->update(m_tail, m_count - m_tail->count(), m_count - 1);
    return Iterator(m_tail, listTailID, m_count - 1);
}

DwDequeList::Iterator DwDequeList::insert(const DwDequeList::Iterator& it)
{
    if (it.equal(begin()))
    {
        return prepend();
    }
    if (it.equal(end()))
    {
        return append();
    }
    if (!validIterator(it))			//��Ч������
    {
        return Iterator();
    }
    DW_ASSERT(NULL != m_lastAccess);
    Chunk* pChunk = it.m_pChunk;
    int offsetID = it.m_slotID;
    DW_ASSERT(offsetID != pChunk->m_end);
    int offset = pChunk->offset(pChunk->m_begin, offsetID);
    if (!(pChunk->full()))
    {
        int moveCount = 0;
        if (offset < pChunk->count() - offset)				//offset��ǰһ�룬�Ӳ���λ�ÿ�ʼ��ǰ�ƶ�
        {
            pChunk->m_begin = pChunk->decIndex(pChunk->m_begin);
            int before = pChunk->m_begin;
            int after = pChunk->incIndex(before);
            moveCount = offset;
            for (int i = 0; i < moveCount; ++i)
            {
                pChunk->m_buf[before] = pChunk->m_buf[after];
                before = after;
                after = pChunk->incIndex(after);
            }
        }
        else												//offset�ں�һ�룬�Ӳ���λ�ÿ�ʼ����ƶ�
        {
            int after = pChunk->m_end;
            int before = pChunk->decIndex(after);
            moveCount = pChunk->count() - offset;
            for (int i = 0; i < moveCount; ++i)
            {
                pChunk->m_buf[after] = pChunk->m_buf[before];
                after = before;
                before = pChunk->decIndex(before);
            }
            pChunk->m_end = pChunk->incIndex(pChunk->m_end);
        }
    }
    else
    {
        //���ڵ�ǰ�����Ľ�㿪ͷ���룬���˽���ǰһ����ֲ����������ǰһ���β������
        if (0 == offset && NULL != pChunk->m_prev && !(pChunk->m_prev->full()))
        {
            DW_ASSERT(pChunk != m_head);
            pChunk = pChunk->m_prev;
            offset = pChunk->count();
            pChunk->m_end = pChunk->incIndex(pChunk->m_end);
        }
        else
        {
            pChunk = halveNode(pChunk, offset, offsetID);	//��ԭ����е�һ��Ԫ���ƶ����½�㣬��ƽ������
        }
    }
    ++m_count;
    m_lastAccess->update(pChunk, it.m_id - offset, it.m_id - offset + pChunk->count() - 1);
    return Iterator(pChunk, pChunk->incIndex(pChunk->m_begin, offset), it.m_id);
}

void DwDequeList::removeFirst()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_lastAccess);
        DW_ASSERT(NULL != m_head);
        --m_count;
        m_head->m_begin = m_head->incIndex(m_head->m_begin);
        if (m_head->empty())
        {
            Chunk* abandonedNode = m_head;
            m_head = m_head->m_next;
            deallocate(abandonedNode);
            if (NULL != m_head)
            {
                m_head->m_prev = NULL;
            }
            else
            {
                DW_ASSERT(0 == m_count && 0 == m_nodeCount);
                m_tail = NULL;
                dwFree(static_cast<void*>(m_lastAccess));
                m_lastAccess = NULL;
                return;
            }
        }
        m_lastAccess->update(m_head, 0, m_head->count() - 1);
    }
}

void DwDequeList::removeLast()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_lastAccess);
        DW_ASSERT(NULL != m_tail);
        --m_count;
        m_tail->m_end = m_tail->decIndex(m_tail->m_end);
        if (m_tail->empty())
        {
            Chunk* abandonedNode = m_tail;
            m_tail = m_tail->m_prev;
            deallocate(abandonedNode);
            if (NULL != m_tail)
            {
                m_tail->m_next = NULL;
            }
            else
            {
                DW_ASSERT(0 == m_count && 0 == m_nodeCount);
                m_head = NULL;
                dwFree(static_cast<void*>(m_lastAccess));
                m_lastAccess = NULL;
                return;
            }
        }
        m_lastAccess->update(m_head, 0, m_head->count() - 1);
    }
}

DwDequeList::Iterator DwDequeList::remove(DwDequeList::Iterator it)
{
    DW_ASSERT(!isEmpty());
    DW_ASSERT(validIterator(it) && !it.equal(end()));
    if (it.equal(begin()))
    {
        //ע����������Ψһ��Ԫ����end()-1��λ����ʱ���ѱ�it.equal(begin())����
        removeFirst();
        return first();
    }
    //ע����������it.equal(begin())����������´���󲻻�ʹDwDequeListΪ��
    Chunk* pChunk = it.m_pChunk;
    int offsetID = it.m_slotID;
    DW_ASSERT(offsetID != pChunk->m_end);
    int offset = pChunk->offset(pChunk->m_begin, offsetID);
    int moveCount = 0;
    if (offset < pChunk->count() - offset)					//offset������ǰһ��
    {
        int after = offsetID;
        int before = pChunk->decIndex(after);
        moveCount = offset;
        for (int i = 0; i < moveCount; ++i)
        {
            pChunk->m_buf[after] = pChunk->m_buf[before];
            after = before;
            before = pChunk->decIndex(before);
        }
        pChunk->m_begin = pChunk->incIndex(pChunk->m_begin);
        it.m_slotID = it.m_pChunk->incIndex(it.m_slotID);	//ɾ����λ�ñ�ǰһ��Ԫ�ظ��ǣ���Ҫ�޸ĵ�����
    }
    else													//offset�������һ��
    {
        int before = offsetID;
        int after = pChunk->incIndex(before);
        moveCount = pChunk->count() - 1 - offset;			//ע�⣺��1
        for (int i = 0; i < moveCount; ++i)
        {
            pChunk->m_buf[before] = pChunk->m_buf[after];
            before = after;
            after = pChunk->incIndex(after);
        }
        pChunk->m_end = pChunk->decIndex(pChunk->m_end);	//ɾ����λ�ñ���һ��Ԫ�ظ��ǣ�����Ҫ�޸ĵ�����
    }
    if (pChunk->empty())
    {
        Chunk* abandonedNode = pChunk;
        //��DW_ASSERT(m_head != pChunk)������removeFirst����ͷ���ʱ�����ܽ�����жϣ�ɾ����һ��������һ��Ԫ�أ�
        //������Ϊ���һ����㣬��ʱɾ��ǰ�Ĵ˽��ֻ��һ��Ԫ��
        DW_ASSERT(NULL != abandonedNode->m_prev);
        abandonedNode->m_prev->m_next = abandonedNode->m_next;
        if(NULL != abandonedNode->m_next)					//end()�������ԣ����Գƴ���
        {
            abandonedNode->m_next->m_prev = abandonedNode->m_prev;
            it.m_pChunk = abandonedNode->m_next;
            it.m_slotID = it.m_pChunk->m_begin;
        }
        else
        {
            DW_ASSERT(m_tail == abandonedNode);
            m_tail = abandonedNode->m_prev;
            it = end();
        }
        deallocate(abandonedNode);
    }
    --m_count;
    m_lastAccess->update(it.m_pChunk, it.m_id - offset, it.m_id - offset + it.m_pChunk->count() - 1);
    return it;
}

void DwDequeList::clear()
{
    if (!isEmpty())
    {
        Chunk* nextNode = m_head->m_next;
        while (NULL != m_head)
        {
            deallocate(m_head);
            m_head = nextNode;
            if(NULL != nextNode)
            {
                nextNode = nextNode->m_next;
            }
        }
        m_tail = NULL;
        m_count = 0;
        dwFree(static_cast<void*>(m_lastAccess));
        m_lastAccess = NULL;
    }
}

DwDequeList::Iterator DwDequeList::createIterator(int id) const
{
    if (id <= 0)
    {
        if (0 != m_count)
        {
            m_lastAccess->update(m_head, 0, m_head->count() - 1);
        }
        return begin();
    }
    if (id >= m_count)
    {
        if (0 != m_count)
        {
            m_lastAccess->update(m_tail, m_count - m_tail->count(), m_count - 1);
        }
        return end();
    }
    DW_ASSERT(NULL != m_head);
    if (m_lastAccess->isHit(id))							//����
    {
        return Iterator(m_lastAccess->m_pChunk
                        , m_lastAccess->m_pChunk->incIndex(m_lastAccess->m_pChunk->m_begin, id - m_lastAccess->m_minID)
                        , id);
    }
    int offset = id;
    Chunk* curNode = NULL;
    if (id < m_lastAccess->m_minID)
    {
        if (id < m_lastAccess->m_minID - id)				//����ʼ��㵽�������һ��֮ǰ
        {
            DW_ASSERT(m_lastAccess->m_minID > id);
            curNode = searchAfter(m_head, offset);
        }
        else
        {
            DW_ASSERT(NULL != m_lastAccess->m_pChunk && NULL != m_lastAccess->m_pChunk->m_prev);
            curNode = searchBefore(m_lastAccess->m_pChunk->m_prev, m_lastAccess->m_minID, offset);
        }
    }
    else
    {
        DW_ASSERT(id > m_lastAccess->m_maxID);
        if (id - m_lastAccess->m_maxID - 1 < m_count - id)	//�ڻ����㵽������㵽��һ��֮ǰ
        {
            DW_ASSERT(NULL != m_lastAccess->m_pChunk && NULL != m_lastAccess->m_pChunk->m_next);
            offset = offset - m_lastAccess->m_maxID -1;
            curNode = searchAfter(m_lastAccess->m_pChunk->m_next, offset);
        }
        else
        {
            DW_ASSERT(NULL != m_tail);
            curNode = searchBefore(m_tail, m_count, offset);
        }
    }
    m_lastAccess->update(curNode, id - offset, id - offset + curNode->count() - 1);
    return Iterator(curNode, curNode->incIndex(curNode->m_begin, offset), id);
}

DwDequeList::Chunk* DwDequeList::searchBefore(Chunk* pChunk, int beforeCount, int& offset) const
{
    int curEndID = beforeCount - pChunk->count();			//m_minID�൱��ǰ���Ԫ������
    while (offset < curEndID)
    {
        DW_ASSERT(NULL != pChunk);
        pChunk = pChunk->m_prev;
        curEndID -= pChunk->count();
    }
    offset -= curEndID;
    return pChunk;
}

DwDequeList::Chunk* DwDequeList::searchAfter(Chunk* pChunk, int& offset) const
{
    int curNodeCount = pChunk->count();
    while (offset >= curNodeCount)
    {
        offset -= curNodeCount;
        DW_ASSERT(NULL != pChunk);
        pChunk = pChunk->m_next;
        curNodeCount = pChunk->count();
    }
    return pChunk;
}