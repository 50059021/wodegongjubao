// MUWorldTransform.cpp : �������̨Ӧ�ó������ڵ㡣
//

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#include <stdio.h>
#include <tchar.h>
#include "FileSystem.h"
#include "LumpFile.h"
#include "Color.h"
#include "CSVFile.h"
#include "Direct.h"

void fileOffset(const std::string& strSrcFilename, const std::string& strDestFilename,int offset)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		if (offset<0)
		{
			size_t addSize=abs(offset);
			fseek(fp, 0, SEEK_SET);
			size+=addSize;
			char* buffer= new char[size];
			fread(buffer,addSize,1,fp);
			fseek(fp, 0, SEEK_SET);
			fread(&buffer[addSize],size-addSize,1,fp);
			{

				FILE* fpw = fopen(strDestFilename.c_str(), "wb");
				if (fpw)
				{
					fwrite(buffer,size,1,fpw);
					fclose(fpw);
				}
			}
			delete buffer;
		}
		else if (size>offset)
		{
			fseek(fp, offset, SEEK_SET);
			size-=offset;
			char* buffer= new char[size];
			fread(buffer,size,1,fp);
			{

				FILE* fpw = fopen(strDestFilename.c_str(), "wb");
				if (fpw)
				{
					fwrite(buffer,size,1,fpw);
					fclose(fpw);
				}
			}
			delete buffer;
		}
		fclose(fp);
	}
}

void decryptMuBuffer(char* buffer, size_t size)
{
	const char xorKeys[] = {
		0xd1, 0x73, 0x52, 0xf6,
		0xd2, 0x9a, 0xcb, 0x27,
		0x3e, 0xaf, 0x59, 0x31,
		0x37, 0xb3, 0xe7, 0xa2
	};
	char key = 0x5E;
	for (size_t i=0; i<size; ++i)
	{
		char encode = *buffer;
		*buffer ^= xorKeys[i%16];
		*buffer -= key;
		key = encode+0x3D;
		buffer++;
	}
}

void decryptBuffEffectFile(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBuffer(buffer+4,size-4);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

void decryptMuFile(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBuffer(buffer,size);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

void decryptMuBufferXOR3(char* buffer, size_t size)
{
	const char xorKeys[] = {0xFC, 0xCF, 0xAB};
	for (size_t i=0; i<size; ++i)
	{
		*buffer ^= xorKeys[i%3];
		buffer++;
	}
}

struct itemdata97
{
	char cName[30];
	unsigned char ucInt[34];
};

struct itemdatastruct97 //96y,97,98c
{
	char cName[30];	//1��Ʒ����
	unsigned char ucHand;		//2 0���֣�1˫��
	unsigned char ucDroplv;	//3 ������Ʒ�Ĺ���ȼ�Level
	unsigned char ucX;		//4 X,Y ��ռ����
	unsigned char ucY;        //5
	unsigned char ucDamMin;	//6 ��С������
	unsigned char ucDamMax;	//7 ��󹥻���
	unsigned char ucDefxx;	//8 ������
	unsigned char ucDef;		//9 ������Def
	unsigned char ucMagDef;	//10 00 MagDef ħ������?
	unsigned char ucSpeed;	//11 �����ٶ�/���ֵ�����
	unsigned char ucWalkSeed;	//12 WalkSpeed Ь������
	unsigned char ucDur;		//13 �־ö�
	unsigned char ucUnknown1;	//14 00
	unsigned short usiReqStr; //15 ����,���������������Ҫ����,���и���������*10 ���� /����12����Ʒ,������ֱ�Ӱ�ֵ����
	unsigned short usiReqDex; //16 ����,���������������Ҫ����,���и����ݲ���*10 ���� /����12����Ʒ
	//unsigned char ucUnknown2;  //17
	//unsigned char ucUnknown3;      //18
	unsigned char ucReqLev;	//19 ��С�����Ʒ�ȼ�
	unsigned char ucValue;		//20 14����Ʒ����۸����
	unsigned char ucUnknown4;	//21 ?�� 12�����
	unsigned char ucUnknown5;        //22
	unsigned char ucUnknown6;	//23 ? �� �������Ϸ���
	unsigned char ucUnknown7;	//24 00
	unsigned char ucType1; //25 ?��04 ..��ͷ04//װ���඼Ϊ04 ��ͨ����������1 ����3,��������0,����2 ��ʦ��03
	unsigned char ucDs;		//26 ��ʦ
	unsigned char ucDk;		//27 սʿ
	unsigned char ucElf;		//28 ����
	unsigned char ucMg;		//29 ħ��ʿ
	unsigned char ucBing;		//30 ��������
	unsigned char ucDu;		//31 ��������
	unsigned char ucLei;		//32 ��������
	unsigned char ucHuo;		//33 ��������
}; //64byte
#include<iostream>
#include<iomanip>
#include<fstream>

void encryptItemBMD(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	CCsvFile csvFile;
	if (csvFile.Open(strSrcFilename))
	{
		char buffer[512*64];
		memset(buffer,0,512*64);
		while (csvFile.SeekNextLine())
		{
			int id = csvFile.GetInt(0);
			if (id>=512)
			{
				continue;
			}
			itemdata97* pItem=(itemdata97*)(buffer+id*64);
			std::string strName = csvFile.GetStr(1);
			strcpy(pItem->cName,strName.c_str());
			for (size_t n=0;n<34;++n)
			{
				pItem->ucInt[n]=csvFile.GetInt(n+2);
			}
		}
		csvFile.Close();
		//
		for (size_t i=0;i<512;++i)
		{
			decryptMuBufferXOR3(buffer+i*64,64);
		}
		DWORD dwCheck;
		//
		FILE* fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,512*64,1,fp);
			fwrite(&dwCheck,4,1,fp);
			fclose(fp);
		}
	}
}

void decryptItemBMD(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		for (size_t i=0;i<size/64;++i)
		{
			decryptMuBufferXOR3(buffer+i*64,64);
		}

		std::ofstream file;
		file.open(strDestFilename.c_str(), std::ios::out);
		if ( file.is_open() )
		{
			file << "ID,��Ʒ����,˫������,����Ʒ�Ĺ���ȼ�,��ռ����X,��ռ����Y,��С������,��󹥻���,������,������,ħ������,�����ٶ�,Ь������,�־ö�,Unknown,��������,��������,�ȼ�����,�۸����,��,22,�鼫�����Ϸ���,00,����,��ʦ,սʿ,����,ħ��ʿ,��������,��������,��������,��������,9,9,9,9"<< std::endl;
			for (size_t i=0;i<size/64;++i)
			{
				itemdata97* pItem = (itemdata97*)(buffer+i*64);
				if (strlen(pItem->cName)!=0)
				{
					file <<i<<","<<pItem->cName;
					for (size_t n=0;n<34;++n)
					{
						file<<","<<(int)pItem->ucInt[n];
					}
					file<<std::endl;
				}
			}
		}
		file.close();

		delete buffer;
	}
}

void decryptMuFileXOR3(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBufferXOR3(buffer,size);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

void decryptMuATTFile(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBuffer(buffer,size);
		decryptMuBufferXOR3(buffer,size);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

bool isEncBmd(const std::string& strSrcFilename)
{
	//FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	//if (fp)
	//{
	//}
	return false;
}

int main(int argc, _TCHAR* argv[])
{
	mkdir("Dec");mkdir("Enc");
	std::wstring wstrFindPath = getCurrentDirectory();
	CDir dir;
	dir.ReadDir(wstrFindPath);
	for (size_t i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (!dir.m_FileInfo[i].IsDirectory())
		{
			std::wstring wstrExt = GetExtension(dir.m_FileInfo[i].wstrFilename);
			std::string strFilename = ws2s(dir.m_FileInfo[i].wstrFilename);
			if (wstrExt==L".ozj")
			{
				std::string strDestFilename = "Dec\\"+ChangeExtension(strFilename,".jpg");
				fileOffset(strFilename,strDestFilename,24);
			}
			else if (wstrExt==L".ozt")
			{
				std::string strDestFilename = "Dec\\"+ChangeExtension(strFilename,".tga");
				fileOffset(strFilename,strDestFilename,4);
			}
			else if (wstrExt==L".ozb")
			{
				std::string strDestFilename = "Dec\\"+ChangeExtension(strFilename,".bmp");
				fileOffset(strFilename,strDestFilename,4);
			}
			else if (wstrExt==L".jpg")
			{
				std::string strDestFilename = "Enc\\"+ChangeExtension(strFilename,".ozj");
				fileOffset(strFilename,strDestFilename,-24);
			}
			else if (wstrExt==L".tga")
			{
				std::string strDestFilename = "Enc\\"+ChangeExtension(strFilename,".ozt");
				fileOffset(strFilename,strDestFilename,-4);
			}
			else if (wstrExt==L".bmp")
			{
				std::string strDestFilename = "Enc\\"+ChangeExtension(strFilename,".ozb");
				fileOffset(strFilename,strDestFilename,-4);
			}
			else if (wstrExt==L".map"||wstrExt==L".obj")
			{
				decryptMuFile(strFilename,"Dec\\"+strFilename+"d");
			}
			else if (wstrExt==L".att")
			{
				decryptMuATTFile(strFilename,"Dec\\"+strFilename+"d");
			}
			else if (wstrExt==L".bmd")
			{
				if ("item.bmd"==strFilename)
				{
					decryptItemBMD(strFilename,"Dec\\"+ChangeExtension(strFilename,".csv"));
				}
				else if ("BuffEffect.bmd"==strFilename)
				{
					decryptBuffEffectFile(strFilename,"Dec\\"+strFilename+"d");
				}
				else if (isEncBmd(strFilename))
				{
					decryptMuFile(strFilename,"Dec\\"+strFilename+"d");
				}
				else
				{
					decryptMuFileXOR3(strFilename,"Dec\\"+strFilename+"d");
				}
			}
			else if (wstrExt==L".csv")
			{
				if ("item.csv"==strFilename)
				{
					encryptItemBMD(strFilename,"Enc\\"+ChangeExtension(strFilename,".bmd"));
				}
			}
		}
	}

	return 0;
}