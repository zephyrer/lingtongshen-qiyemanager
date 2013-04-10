#include "stdafx.h"
#include "ShowSampleGridView.h"


CShowSampleGridView::CShowSampleGridView(CWnd*   pWnd,CRect   Rect)
					:CGridView(pWnd,Rect,MAX_ROW,MAX_COLUMN,1,1,SAMPLE_ITEM)
{

}

CShowSampleGridView::~CShowSampleGridView()
{


}