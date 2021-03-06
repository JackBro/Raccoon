#include "stable.h"
#include "dwnumwidthvalidater.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwNumWidthDocValidater, 'NDVR', DwDocumentValidater);

//------------------------------------------------------------------------------
/**
*/
DwNumWidthDocValidater::DwNumWidthDocValidater(DwTextDocument *doc/* = NULL*/, int num/* = 0*/)
    : DwDocumentValidater(doc)
    , m_maxNumWidth(num)
    , m_curWidth (0)
{
    open();
    setValidater(DwDocumentValidater::ValidaterAdd);
    setValidater(DwDocumentValidater::ValidaterDel);
}

//------------------------------------------------------------------------------
/**
*/
bool DwNumWidthDocValidater::validateDoc()
{
    m_curWidth = 0;
    for (int i=0; i<m_doc->length(); ++i)
    {
        DwRichChar c = m_doc->at(i);
        m_curWidth += deltaOf(&c);
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
int DwNumWidthDocValidater::deltaOf(DwRichChar *c)
{
    int delta = 0;
    if (c->character() < 256)
    {
        delta = 1;
    }
    else
    {
        delta = 2;
    }
    return delta;
}

//------------------------------------------------------------------------------
/**
*/
bool DwNumWidthDocValidater::setNumWidth(int num)
{
    m_maxNumWidth = num;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwNumWidthDocValidater::onAdd(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(pos);
    int delta = deltaOf(c);
    int selectNum = 0;
    if(m_doc)
    {
	DwTextCursor &cursor = m_doc->cursor();
	DwTextCursor &selectCursor = m_doc->selectCursor();

	if (selectCursor.cur() != cursor.cur())
	{
	    int start = dwMin<int>(selectCursor.cur(), cursor.cur());
	    int end = dwMax<int>(selectCursor.cur(), cursor.cur());
	    selectNum = end -start;
	}
    }

    if (m_curWidth + delta - selectNum <= m_maxNumWidth)
    {
        m_curWidth += delta;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwNumWidthDocValidater::onDel(DwRichChar *c, int pos/* = -1*/)
{
    DW_UNUSED(pos);
    int delta = deltaOf(c);
    m_curWidth -= delta;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwNumWidthDocValidater::refresh()
{
    m_curWidth = 0;
}


