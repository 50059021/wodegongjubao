#include "EditingDialog.h"

CEditingDialog::CEditingDialog()
{
}

CEditingDialog::~CEditingDialog()
{
}

bool CEditingDialog::OnInitDialog()
{
	if (m_rcScale.getWidth()==0&&m_rcScale.getHeight()==0&&
		m_rcOffset.getWidth()==0&&m_rcOffset.getHeight()==0)
	{
		m_rcScale.set(0,0,100,100);
	}
	//EnableCaption(true);
	return true;
}

void CEditingDialog::OnFrameRender(const Matrix& mTransform, double fTime, float fElapsedTime)
{
	CUIDialog::OnFrameRender(fTime, fElapsedTime);
	/*UIGraph::DrawRect( GetBoundingBox(), 0xA00000FF );
	if( m_bCaption )// ������
	{
		UIGraph::DrawRect( m_rcCaption, 0xA00000FF );
	}
	if( !m_bMinimized )
	{
		for( int i=0; i < m_Controls.size(); i++ )
		{
			CUIControl* pControl = m_Controls[i];   

			// Focused control is drawn last
			if( pControl != s_pControlFocus )
			{
				UIGraph::DrawRect( pControl->GetBoundingBox(), 0xA0FF0000 );
			}
		}

		if( s_pControlFocus != NULL && s_pControlFocus->GetParentDialog() == this )
		{
			UIGraph::DrawRect( s_pControlFocus->GetBoundingBox(), 0xA000FF00 );
		}
	}*/
}