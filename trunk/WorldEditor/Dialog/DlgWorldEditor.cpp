#include "DlgWorldEditor.h"
#include "LumpFile.h"
#include "FileSystem.h"
#include "Camera.h"
#include "..\Config\Config.h"

CDlgWorldEditor::CDlgWorldEditor()
{
	m_wstrFileType = L".map";
}

CDlgWorldEditor::~CDlgWorldEditor()
{	
}

CUIWorldEditorDisplay& CDlgWorldEditor::getDisplay()
{
	return m_WorldEditorDisplay;
}

bool CDlgWorldEditor::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//.//
	//m_DlgFPS.SetViewDir(m_WorldEditorDisplay.GetCamera().GetViewDir());
	//
	bool bNoFurtherProcessing = CUIMainDialog::MsgProc(hWnd, uMsg, wParam, lParam);
	if ( bNoFurtherProcessing )
	{
		return true;
	}
	return false;
}

void CDlgWorldEditor::OnControlRegister()
{
	CUIMainDialog::OnControlRegister();

	m_DlgToolbar.Create("IDD_TOOLBAR", this);
	m_DlgFile.Create("IDD_FILE", this);
	m_DlgRegister.Create("IDD_REGISTER", this);
	//m_DlgFPS.Create("IDD_FPS", this);

	RegisterControl( "IDC_STATIC_POS_X", m_StaticPosX);
	RegisterControl( "IDC_STATIC_POS_Y", m_StaticPosY);
	RegisterControl( "IDC_STATIC_POS_HEIGHT", m_StaticPosHeight);

	RegisterControl( "IDC_STATIC_INFO", m_StaticInfo);
	RegisterControl( "IDC_DISPLAY_WORLDEDITOR", m_WorldEditorDisplay);

	RegisterControlEvent("IDC_BTN_NEW_FILE",	(PEVENT)&CDlgWorldEditor::OnBtnNewFile);
	RegisterControlEvent("IDC_BTN_OPEN_FILE",	(PEVENT)&CDlgWorldEditor::OnBtnOpenFile);
	RegisterControlEvent("IDC_BTN_SAVE_FILE",	(PEVENT)&CDlgWorldEditor::OnBtnSaveFile);
	RegisterControlEvent("IDC_BTN_TOOLBAR",		(PEVENT)&CDlgWorldEditor::OnBtnToolbar);

	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgWorldEditor::OnFileNew,CDlgFile::EVENT_NEW);
	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgWorldEditor::OnFileOpen,CDlgFile::EVENT_OPEN);
	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgWorldEditor::OnFileSave,CDlgFile::EVENT_SAVE);
	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgWorldEditor::OnFileCancel,CDlgFile::EVENT_CANCEL);
}
#include "IORead.h"
bool CDlgWorldEditor::OnInitDialog()
{
	IOReadBase* pRead = IOReadBase::autoOpen("update.inf");
	if (pRead)
	{
		size_t filesize = pRead->GetSize();
		if (filesize>0)
		{
			char c;
			std::string strInfo;
			for (size_t i=0;i<filesize;++i)
			{
				pRead->Read(&c, 1);
				strInfo.push_back(c);
			}
			m_StaticInfo.SetText(s2ws(strInfo));
		}
		IOReadBase::autoClose(pRead);
	}
	loadPlugFromPath("");

//#if defined(_MU)
	SetControlEnabled("IDC_BTN_NEW_FILE",false);
//#endif
	m_DlgToolbar.SetVisible(false);
	m_DlgFile.SetVisible(false);
	return CUIMainDialog::OnInitDialog();
}

void CDlgWorldEditor::OnFrameMove(double fTime, float fElapsedTime)
{
	CUIMainDialog::OnFrameMove(fTime,fElapsedTime);
	const Vec3D& vPos = getDisplay().getScene().getTargetPos();
	m_StaticPosX.SetText(FormatW(L"%0.2f",vPos.x));
	m_StaticPosY.SetText(FormatW(L"%0.2f",vPos.z));
	m_StaticPosHeight.SetText(FormatW(L"%0.2f",vPos.y));
}


void CDlgWorldEditor::OnBtnNewFile()
{
	std::wstring wstrPath = s2ws(GetParentPath(getDisplay().getTerrain().GetData().getFilename()));
	if (wstrPath.length()==0)
	{
		wstrPath=getCurrentDirectory();
	}
	m_DlgFile.NewFile(wstrPath,m_wstrFileType);
}

void CDlgWorldEditor::OnBtnOpenFile()
{
	std::wstring wstrPath = s2ws(GetConfig().m_strLastPath);
	if (wstrPath.length()==0)
	{
		wstrPath=getCurrentDirectory();
	}
	m_DlgFile.OpenFile(wstrPath,m_wstrFileType);
}

void CDlgWorldEditor::OnBtnSaveFile()
{
	std::wstring wstrPath = s2ws(GetConfig().m_strLastPath);
	if (wstrPath.size()==0)
	{
		wstrPath=getCurrentDirectory();
	}
	m_DlgFile.SaveFile(wstrPath,m_wstrFileType);
}

void CDlgWorldEditor::OnBtnToolbar()
{
	m_DlgToolbar.SetVisible(!m_DlgToolbar.IsVisible());
}

void CDlgWorldEditor::OnFileNew()
{
	std::wstring wstrFilename = m_DlgFile.GetFilename();
	std::wstring wstrPath = GetParentPath(wstrFilename);
	std::wstring wstrSceneName = GetFilename(wstrFilename);
	wstrSceneName=ChangeExtension(wstrSceneName,L"");
	//fileCopy(L"Data\\default\\Default.rar",		wstrPath+L"Default.rar");
	fileCopy(L"Data\\default\\Tile.csv",		wstrPath+L"Tile.csv");
	fileCopy(L"Data\\default\\Object.csv",		wstrPath+L"Object.csv");
	fileCopy(L"Data\\default\\TerrainLight.bmp",wstrPath+L"TerrainLight.bmp");
	fileCopy(L"Data\\default\\Default.sce",		wstrPath+wstrSceneName+L".sce");
	fileCopy(L"Data\\default\\Default.map",		wstrPath+wstrSceneName+L".map");
	fileCopy(L"Data\\default\\Default.obj",		wstrPath+wstrSceneName+L".obj");

	pathCopy(L"Data\\default\\Obj\\",		wstrPath+L"Obj\\");
	pathCopy(L"Data\\default\\Tile\\",		wstrPath+L"Tile\\");
	OnFileOpen();
}

void CDlgWorldEditor::OnFileOpen()
{
	std::string strFilename = ws2s(m_DlgFile.GetFilename());
	GetConfig().m_strLastPath = GetParentPath(strFilename);
	if(m_arrPlugObj.size()>0)
	{
		m_arrPlugObj[0].pObj->importData(&getDisplay().getScene(),strFilename);
	}
	// Update UI
	m_DlgToolbar.reset();
}

void CDlgWorldEditor::OnFileSave()
{
	std::string strFilename = ws2s(m_DlgFile.GetFilename());
	if(m_arrPlugObj.size()>0)
	{
		m_arrPlugObj[0].pObj->exportData(&getDisplay().getScene(),strFilename);
	}
}

void CDlgWorldEditor::OnFileCancel()
{
	m_DlgFile.GetFilename();
}

bool CDlgWorldEditor::loadPlugFromPath(const std::string& strPath)
{
	std::string strFindFile = strPath+"*.dll";

	WIN32_FIND_DATAA wfd;
	HANDLE hf = FindFirstFileA(strFindFile.c_str(), &wfd);
	if (INVALID_HANDLE_VALUE != hf){
		createPlug(strPath + wfd.cFileName);
		while (FindNextFileA(hf, &wfd))
		{
			createPlug(strPath + wfd.cFileName);
		}
		FindClose(hf);
	}
	return TRUE;
}

bool CDlgWorldEditor::createPlug(const std::string& strFilename)
{
	bool brt = FALSE;

	if (m_arrPlugObj.size() > 255){
		MessageBoxA(NULL,"�������", "message", MB_OK|MB_ICONINFORMATION);
		return brt;
	}

	PLUG_ST stPs;

	ZeroMemory(&stPs, sizeof(stPs));

	stPs.hIns = LoadLibraryA(strFilename.c_str());
	if (stPs.hIns){
		PFN_Plug_CreateObject pFunc = (PFN_Plug_CreateObject)GetProcAddress(
			stPs.hIns, "Plug_CreateObject");
		if (pFunc){
			if (pFunc((void **)&stPs.pObj)){
				brt =true;
				m_arrPlugObj.push_back(stPs);
				m_wstrFileType=s2ws(stPs.pObj->GetFormat());
			}
		}
	}
	if (!brt){
		if (stPs.pObj){
			stPs.pObj->Release();
		}
		if (stPs.hIns){
			FreeLibrary(stPs.hIns);
		}
	}
	return brt;
}