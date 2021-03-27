// XML言語設定ファイルを読み込み
#pragma once
#include <atlcomcli.h>
#include <atlstr.h>
#include <xmllite.h>
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
namespace stdxml
{
	/// <summary>
	/// オプションデータ
	/// </summary>
	struct SXml {
		CString tag;	// XMLタグ
		CString ntype;	// XMLデータタイプ
		CString key;	// 要素キー
		CString value;	// 要素名
	};
	class XMLRead
	{
	public:
		////////////////////////////////////////////
		// XMLファイルからタグ情報取得
		////////////////////////////////////////////
		/// <summary>
		/// コンストラクタ―
		/// XMLファイルパス設定
		/// </summary>
		XMLRead();
		/// <summary>
		/// コンストラクタ―
		/// </summary>
		/// <param name="parafile">パラメータファイルパス</param>
		XMLRead(const char* parafile);
		/// <summary>
		/// デストラクタ―
		/// </summary>
		virtual ~XMLRead() {}
		/// <summary>
		/// XMLオブジェクト生成
		/// XMLファイルオープン
		/// 読み込み準備
		/// </summary>
		/// <return>BOOL TRUE=正常、FALSE=失敗</return>
		virtual BOOL GenXML();
		/// <summary>
		/// XMLデータ読み込みメインメソッド
		/// </summary>
		/// <returns>0=正常、0!=異常</returns>
		virtual int XMLReader();
		/// <summary>
		/// XMLデータオブジェクトを返す
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		virtual std::vector<SXml> get_xml(void);
		/// <summary>
		/// XMLファイルパス設定
		/// </summary>
		/// <param name="parafile">XMLファイルパス</param>
		virtual void SetFilePath(const char* parafile);
	protected:
		/// <summary>
		/// XML初期化
		/// </summary>
		void InitXml(void);
		// XMLファイル名
		fs::path xmlfile;
		// XMLオブジェクト
		CComPtr<IXmlReader>	m_pReader;
		// XMLストリームオブジェクト
		CComPtr<IStream>	m_pStream;
		// defaultid デフォルトタグのid
		std::string defid;
		// XMLノードタイプ
		XmlNodeType m_nodeType;
		/// <summary>
		/// コントロールと要素名
		/// </summary>
		std::vector<SXml> vXml;
	};
}