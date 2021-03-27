// XML����ݒ�t�@�C����ǂݍ���
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
	/// �I�v�V�����f�[�^
	/// </summary>
	struct SXml {
		CString tag;	// XML�^�O
		CString ntype;	// XML�f�[�^�^�C�v
		CString key;	// �v�f�L�[
		CString value;	// �v�f��
	};
	class XMLRead
	{
	public:
		////////////////////////////////////////////
		// XML�t�@�C������^�O���擾
		////////////////////////////////////////////
		/// <summary>
		/// �R���X�g���N�^�\
		/// XML�t�@�C���p�X�ݒ�
		/// </summary>
		XMLRead();
		/// <summary>
		/// �R���X�g���N�^�\
		/// </summary>
		/// <param name="parafile">�p�����[�^�t�@�C���p�X</param>
		XMLRead(const char* parafile);
		/// <summary>
		/// �f�X�g���N�^�\
		/// </summary>
		virtual ~XMLRead() {}
		/// <summary>
		/// XML�I�u�W�F�N�g����
		/// XML�t�@�C���I�[�v��
		/// �ǂݍ��ݏ���
		/// </summary>
		/// <return>BOOL TRUE=����AFALSE=���s</return>
		virtual BOOL GenXML();
		/// <summary>
		/// XML�f�[�^�ǂݍ��݃��C�����\�b�h
		/// </summary>
		/// <returns>0=����A0!=�ُ�</returns>
		virtual int XMLReader();
		/// <summary>
		/// XML�f�[�^�I�u�W�F�N�g��Ԃ�
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		virtual std::vector<SXml> get_xml(void);
		/// <summary>
		/// XML�t�@�C���p�X�ݒ�
		/// </summary>
		/// <param name="parafile">XML�t�@�C���p�X</param>
		virtual void SetFilePath(const char* parafile);
	protected:
		/// <summary>
		/// XML������
		/// </summary>
		void InitXml(void);
		// XML�t�@�C����
		fs::path xmlfile;
		// XML�I�u�W�F�N�g
		CComPtr<IXmlReader>	m_pReader;
		// XML�X�g���[���I�u�W�F�N�g
		CComPtr<IStream>	m_pStream;
		// defaultid �f�t�H���g�^�O��id
		std::string defid;
		// XML�m�[�h�^�C�v
		XmlNodeType m_nodeType;
		/// <summary>
		/// �R���g���[���Ɨv�f��
		/// </summary>
		std::vector<SXml> vXml;
	};
}