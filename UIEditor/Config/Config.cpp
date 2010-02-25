#include "Config.h"
#include "tinyxml.h"

CConfig& GetConfig()
{
	static CConfig g_ModelConfig;
	return g_ModelConfig;
}
#include "IORead.h"
CConfig::CConfig()
{
	//����һ��XML���ĵ�����
	TiXmlDocument myDocument;
	{
		IOReadBase* pRead = IOReadBase::autoOpen("config.xml");
		if (!pRead)
		{
			return;
		}
		char* pBuf = new char[pRead->GetSize()+1];
		if (pBuf)
		{
			pBuf[pRead->GetSize()] = 0;
			pRead->Read(pBuf, pRead->GetSize());
			myDocument.LoadFormMemory(pBuf, pRead->GetSize(), TIXML_ENCODING_UTF8);
			delete[] pBuf;
		}
		IOReadBase::autoClose(pRead);
	}
	if ( myDocument.Error() )
	{
		return;
	}
	else
	{
		//��ø�Ԫ�أ���root��
		TiXmlElement *RootElement = myDocument.RootElement();
		//��õ�һ��window�ڵ㡣
		TiXmlElement *PathElement = RootElement->FirstChildElement("path");

		if (PathElement->Attribute("font"))
		{
			m_strFontFilename = PathElement->Attribute("font");
		}
		if (PathElement->Attribute("theme"))
		{
			m_strThemeFilename = PathElement->Attribute("theme");
		}
		if (PathElement->Attribute("ui"))
		{
			m_strUIFilename = PathElement->Attribute("ui");
		}
		if (PathElement->Attribute("font_size"))
		{
			m_nFontSize = atoi(PathElement->Attribute("font_size"));
		}
	}
}

CConfig::~CConfig()
{
}