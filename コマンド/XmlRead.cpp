#include <Windows.h>
#include <ShlObj.h>
#include <atlbase.h>
#include "XmlRead.hpp"
#pragma comment(lib, "xmllite.lib")
namespace stdxml
{
	/// <summary>
	/// コンストラクタ―
	/// XMLファイルパス設定
	/// </summary>
	XMLRead::XMLRead() :
		xmlfile{}
	{
		this->InitXml();
	}
	/// <summary>
	/// コンストラクタ―
	/// </summary>
	/// <param name="parafile">パラメータファイルパス</param>
	XMLRead::XMLRead(const char* parafile) :
		xmlfile(parafile)
	{
		this->InitXml();
	}
	/// <summary>
	/// XMLオブジェクト生成
	/// XMLファイルオープン
	/// 読み込み準備
	/// </summary>
	/// <return>BOOL TRUE=正常、FALSE=失敗</return>
	BOOL XMLRead::GenXML()
	{
		BOOL retB = FALSE;
		this->m_pReader.Release();
		if (FAILED
		(CreateXmlReader(__uuidof(IXmlReader),
			reinterpret_cast<void**>(&this->m_pReader), 0))
			)
		{
			return retB;
		}
		if (FAILED
		(SHCreateStreamOnFile(
			this->xmlfile.string().c_str(),
			STGM_READ,
			&this->m_pStream))
			)
		{
			return retB;
		}
		if (FAILED(this->m_pReader->SetInput(this->m_pStream))) {
			return retB;
		}
		retB = TRUE;
		return retB;
	}
	/// <summary>
	/// XMLデータ読み込みメインメソッド
	/// </summary>
	/// <returns>0=正常、0!=異常</returns>
	int XMLRead::XMLReader()
	{
		int reti = 0;
		this->vXml.clear();
		LPCWSTR tag = nullptr;
		LPCWSTR key = nullptr;
		LPCWSTR value = nullptr;
		while (S_OK == this->m_pReader->Read(&this->m_nodeType))
		{
			SXml txml = {};
			switch (this->m_nodeType)
			{
			case XmlNodeType_Element:
				UINT cnt;
				this->m_pReader->GetAttributeCount(&cnt);
				this->m_pReader->GetLocalName(&tag, NULL);
				if (S_OK == this->m_pReader->MoveToFirstAttribute())
				{
					do
					{
						this->m_pReader->GetLocalName(&key, NULL);
						this->m_pReader->GetValue(&value, NULL);
						SXml sxml = { tag, _T("XmlOption"), key, value };
						this->vXml.push_back(sxml);
					}
					while (S_OK == this->m_pReader->MoveToNextAttribute());
				}
				break;
			case XmlNodeType_Text:
				this->m_pReader->GetValue(&value, NULL);
				SXml sxml = { tag, _T("XmlValue"), _T(""), value };
				this->vXml.push_back(sxml);
				break;
			}
		}
		return reti;
	}
	/// <summary>
	/// XMLデータオブジェクトを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<SXml> XMLRead::get_xml(void)
	{
		return this->vXml;
	}
	/// <summary>
	/// XMLファイルパス設定
	/// </summary>
	/// <param name="parafile">XMLファイルパス</param>
	void XMLRead::SetFilePath(const char* parafile)
	{
		this->xmlfile = parafile;
	}
	/// <summary>
	/// XML初期化
	/// </summary>
	void XMLRead::InitXml(void)
	{
		this->m_pReader = (nullptr);
		this->m_pStream = (nullptr);
		this->defid = ("");
		this->m_nodeType = {};
		this->vXml = {};
	}
}