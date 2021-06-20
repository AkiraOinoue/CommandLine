/*
    �R�}���h���C���I�v�V�������擾
*/
// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// update:2014.8.9
// ----------------------------------------------------------------------------
#ifndef COMMANDLINE_H
#define COMMANDLINE_H
#include <map>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <locale>
#include "CmdLineException.h"
#include "ParamReader.h"
#define OPTSEP "/"
#define UDP "_udp" // ����`�p�����[�^�L�[
#define REPSTR_MARK "@"
#define _NONEP  CommandLine::_none_param
#define _FREEP  CommandLine::_free_param
#define _NEEDP  CommandLine::_need_param
#define _ULMTP  CommandLine::_unlimit_param

namespace locallib {
    class CommandLine :
        public CmdLineException,
        public ParamReader // XML Reader
    {
    public:
		///////////////////////////////////////////////////////
		// �R���X�g���N�^
		///////////////////////////////////////////////////////
        CommandLine() throw();
        ////////////////////////////////////////////////////////////////
        /// <summary>
        /// �R�}���h���C���R���X�g���N�^
        /// �i�R���\�[���v���O�����p�j
        /// </summary>
        /// <param name="agc">�p�����[�^��</param>
        /// <param name="agv">�p�����[�^�e�[�u��</param>
        /// <param name="valid_flg">�I�v�V�����Ȃ��̋��t���O</param>
        /// <param name="hlp_opt">�w���v�I�v�V����������</param>
        /// <returns>�Ȃ�</returns>
        CommandLine(
            const int agc,
            const char* agv[],
            bool valid_flg = false,
            std::string hlp_opt = "help"
        ) throw();
        ////////////////////////////////////////////////////////////////
        /// <summary>
        /// �R�}���h���C���R���X�g���N�^
        /// �iWindows�v���O�����p�j
        /// </summary>
        /// <param name="lpCmdLine">�p�����[�^�e�[�u��</param>
        /// <param name="valid_flg">�I�v�V�����Ȃ��̋��t���O</param>
        /// <param name="hlp_opt">�w���v�I�v�V����������</param>
        /// <returns>�Ȃ�</returns>
        CommandLine(
            LPWSTR    lpCmdLine,
            bool valid_flg = false,
            std::string hlp_opt = "help"
        ) throw();
        virtual ~CommandLine() throw();
    public:
		///////////////////////////////////////////////////////
        // �I�v�V�����p�����[�^�w����@
        ///////////////////////////////////////////////////////
        enum _T_e_ParamNum
        {
            _none_param = 1,                        // �Ȃ�
            _need_param = _none_param << 1,     // �K�{
            _free_param = _need_param << 1,     // �C��
            _unlimit_param = _free_param << 1   // �������p�����[�^
        };
        ///////////////////////////////////////////////////////
        // �I�v�V�������R���e�i
        ///////////////////////////////////////////////////////
        struct _T_OptionInfo
        {
            // �I�v�V��������
            std::string optstr;
            // �I�v�V�����̎w����@:enum _T_e_ParamNum
            // �K�{�F_need_param�A�C�ӁF_free_param�A�������p�����[�^:_unlimit_param
            int opt_need_flg;
            // �I�v�V�����p�����[�^�w����@:enum _T_e_ParamNum
            // �K�{�F_need_param�A�C�ӁF_free_param�A�������p�����[�^:_unlimit_param�A�Ȃ��F_none_param
            int param_flg;
            // �I�v�V�����p�����[�^�C�ӂ̏ꍇ�̃f�t�H���g�l
            std::string default_opt;
            // �I�v�V��������
            std::string opt_desc;
            // ���я�
            int idx;
        };
        ///////////////////////////////////////////////////////
        // �^��`
        ///////////////////////////////////////////////////////
        // �R�}���h���C��������I���W�i���f�[�^�ۑ��̌^��
        typedef std::vector<std::string>                    _T_ArgumentType;
        // �R�}���h���C��������ۑ�����R���e�i�̌^��
        typedef std::multimap<std::string, std::string>     _T_mmap_CmdLineType;
        // �R�}���h���C��������ۑ�����R���e�i�̗v�f�^��
        typedef std::pair<std::string, std::string>         _T_p_CmdLineType;
        // �R�}���h���C�������̕ۑ��R���e�i�̃C�^���[�^�^��
        typedef CommandLine::_T_mmap_CmdLineType::iterator  _T_It_CmdLineType;
        // �R�}���h���C�������̌��������W�̌^��
        typedef std::pair<_T_It_CmdLineType,_T_It_CmdLineType>  _T_pr_CmdLineType;
        // ���j�[�N�ȃI�v�V�����L�[����ۑ�����R���e�i�̌^��
        typedef std::map<std::string, _T_OptionInfo>          _T_map_OptionType;
        // ���j�[�N�ȃI�v�V�����L�[����ۑ�����R���e�i�̃C�^���[�^�^��
        typedef CommandLine::_T_map_OptionType::iterator      _T_It_OptionType;
        // ���j�[�N�ȃI�v�V�����L�[����ۑ�����R���e�i�̗v�f�^��
        typedef std::pair<std::string, _T_OptionInfo>         _T_p_OptionType;
        // �R�}���h�w���v�p�e�[�u��
        typedef std::multimap<std::string, std::string>     _T_mmap_CmdHelpType;
        /////////////////////////////////////////////////////////////////////////////
        // �I�v�V�����p�����[�^�p�n�b�V���e�[�u��
        /////////////////////////////////////////////////////////////////////////////
        // --env os=windows ie=ver9.0�ȂǁBenv�ɑ΂���os��windows�����i�[
        // $1=param_key, $2=param_value
        typedef	std::multimap<std::string, std::string>	_T_OptParamHashType;
        // �I�v�V�����p�����[�^�p�n�b�V���e�[�u���̃C�^���[�^�^��
        typedef CommandLine::_T_OptParamHashType::iterator	_T_It_OptParamHashType;
		// �I�v�V�����p�����[�^�p�n�b�V���e�[�u���\��
		typedef struct
		{
			CommandLine::_T_OptParamHashType	m_optprm_hash;
			CommandLine::_T_It_OptParamHashType	m_it;
		} _T_OptPrmHashStrct;
		// �I�v�V�����p�n�b�V���e�[�u��
		// $1=optstr, $2=hash strct
		typedef std::map<std::string, CommandLine::_T_OptPrmHashStrct> _T_OptHashType;
		// �I�v�V�����p�n�b�V���e�[�u���̃R���e�i�̗v�f�^��
		// $1=optstr, $2=hash
		typedef std::pair<std::string, CommandLine::_T_OptPrmHashStrct>	_T_p_OptHashType;
		// �I�v�V�����p�����[�^�p�n�b�V���e�[�u���̃R���e�i�̗v�f�^��
		typedef std::pair<std::string, std::string>	_T_p_OptParamHashType;
    private:
        // �I�v�V�����p�����[�^��؂�
        // --param=xxx=yyy=... �ȂǂɑΉ�
        std::string optprm_sep;
		// �I�v�V��������������
		bool	valid_none_opt_flg;
        // ���я� - used by usage(). set by setOption()
        int m_idx;
        // �R�}���h���C��������I���W�i���f�[�^�ۑ�
        CommandLine::_T_ArgumentType    m_agv;
        // �R�}���h���C��������I���W�i���f�[�^���ۑ�
        int m_agc;
        // �N���v���O�������ۑ�
        std::string m_prgname;
        // �R�}���h���C���C�^���[�^
        CommandLine::_T_It_CmdLineType  m_Itcmdline;
        // �I�v�V���������w��q
        std::string    m_sep;
        // �I�v�V��������
        std::string m_optstr;
        // �I�v�V�����p�����[�^
        std::string m_optparam;
        // �K�{�I�v�V�����̎w��`�F�b�N
        bool chk_need_option( void );
        // �w���v�I�v�V����������
        // -�w���v�I�v�V�����w�莞�ɕK�{�I�v�V�������w��G���[��
        // -�\�����Ȃ�����O�𔭐������Ȃ��B
        std::string m_help_option;
    protected:
		// �I�v�V�����p�n�b�V���e�[�u��
		CommandLine::_T_OptHashType		m_opthash;
        // �R�}���h���C��������ۑ�����R���e�i
        CommandLine::_T_mmap_CmdLineType  m_cmdline;
        // ���j�[�N�ȃI�v�V�����L�[����ۑ�����R���e�i
        CommandLine::_T_map_OptionType    m_option;
        // �R�}���h�w���v�p�e�[�u��
        CommandLine::_T_mmap_CmdHelpType m_cmd_help;
        // �w��̗v�f���I�v�V�����������ǂ����𔻒�
        bool IsOptStr(const int) const;
        // �w��̗v�f���I�v�V�����p�����[�^�������ǂ����𔻒�
        bool IsOptParam(const int) const;
    public:
		///////////////////////////////////////////////////////
		// ���\�b�h
		///////////////////////////////////////////////////////
        // �N���I�v�V��������������
        bool is(std::string optstr);
        // �I�v�V�����n�b�V���e�[�u����prmkey���擾
		std::string param_key( std::string optkey );
		// �I�v�V�����n�b�V���e�[�u����prmvalue���擾
		std::string hvalue( std::string optkey );
		// �I�v�V�����n�b�V���e�[�u���Ƀp�����[�^�ƒl��o�^
		void set_optprmhash( std::string optstr, std::string optprm_key, std::string value );
		// �I�v�V�����n�b�V���e�[�u���̑��݃`�F�b�N
		bool is_hash( std::string optkey, const std::string prmkey );
		// �I�v�V�����n�b�V���e�[�u���̎擾�J�n
		void start_hash( std::string optkey );
		// �I�v�V�����n�b�V���e�[�u���̎擾�i�����擾�^�j
		void next_hash( std::string optkey );
		// �I�v�V�����n�b�V���e�[�u���̌����擾
		ptrdiff_t count_hash( std::string optkey ) const;
		// �I�v�V�����n�b�V���e�[�u���̌����擾�i�p�����[�^�L�[�w��j
		ptrdiff_t count_hash( std::string optkey, std::string prmkey );

        // �I�v�V�����p�����[�^��؂�ݒ�
        // --param=xxx=yyy=... �ȂǂɑΉ�
        void setoptprm_sep(const std::string&);
        // �I�v�V�����p�����[�^��؂�擾
        std::string getoptprm_sep() const;
        // �Z�p���[�^�t���I�v�V���������擾
        std::string get_OptWithSep( const std::string&, std::string );
        // �Z�p���[�^�t���I�v�V�����p�����[�^���擾
        size_t count_OptPrmWithSep( std::string&, std::string );
        // �Z�p���[�^�t���I�v�V�����p�����[�^�擾
		typedef std::vector< std::string > _T_OptPrmList;
        _T_OptPrmList get_OptPrmWithSep( std::string&, std::string );
        // �R�}���h�p�����[�^�t�@�C������荞�� - used "ParamReader.h"
#if USED_BOOSTLIB
        void setm_agv(ptree_type::const_iterator pos);
        // �R�}���h�p�����[�^�t�@�C���̑������擾 - used "ParamReader.h"
        std::string get_attr(ptIT_type pos, std::string attr_name, std::string tag);
#else
        void setm_agv(stdxml::XMLRead* pos);
#endif // USED_BOOSTLIB
        // get agv
        CommandLine::_T_ArgumentType getm_agv();
        // �������ǂ����𔻒�
        bool is_number( const char* );
        // �������ǂ����𔻒�i�����^�j
        bool is_number(void);
        // Usage�\��
        virtual void usage( const char* );
        ///////////////////////////////////////////////////
        /// <summary>
        /// Usage�_�C�A���O�\��
        /// </summary>
        /// <param name="">�Ȃ�</param>
        virtual void usageDlg(void);
        virtual void usage(void);
//        This is free software; see the source for copying conditions.  There is NO
//        warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
        virtual void version() const;
        // �I�v�V�������o�^
        virtual void setOption(
                const char* key,
                const char* desc = "",       // �I�v�V��������
                const int opt_need_flg = _free_param, // �C��
                const int param_flg = _free_param, // �C��
                const char* default_opt = "" // �f�t�H���g�l
                );
        // �R�}���h�w���v���o�^
        virtual CommandLine& setCmdHelp( const char* key, const char* help );
        //////////////////////////////////////////////////////////
        /// <summary>
        /// �R�}���h�w���v���o�́i�_�C�A���O�\���j
        /// </summary>
        /// <param name="key">�I�v�V����������</param>
        /// <param name="help_str">�w���v�\��������</param>
        virtual void getCmdHelpDlg(
            const char* key,
            std::string& help_str
        );
        // �R�}���h�w���v���o��
        virtual void getCmdHelp( const char* key );
        virtual void getCmdHelp( const std::string key );
        // �R�}���h���C��������I���W�i���f�[�^�ۑ�
        void setAgv(const int agc, const char* agv[]);
        void setAgv(int agc, char* agv[]);
        /////////////////////////////////////////////////////////
        /// <summary>
        /// �R�}���h�p�����[�^�ϊ�
        /// Windows�v���O�����p
        /// </summary>
        /// <param name="lpCmdLine">�R�}���h�p�����[�^</param>
        void setAgv(LPWSTR    lpCmdLine);
        // �I�v�V�������擾
        virtual _T_OptionInfo   getOption( const char* key );
        // �I�v�V�����L�[����
        virtual bool search( const char* key );
        // �I�v�V���������w��q�ݒ�
        void setSep( const char* sp );
        // �I�v�V���������w��q
        std::string getSep( void ) const;
        /// <summary>
        /// �R�}���h���C���������
        /// </summary>
        /// <param name="ChkNeedFlg">
        /// true:�K�{�I�v�V�����`�F�b�N�A
        /// false:�K�{�I�v�V�����`�F�b�N�Ȃ�
        /// �i�f�t�H���g�F�`�F�b�N����j
        /// </param>
        virtual void options( bool ChkNeedFlg = true );
        // �R�}���h�p�����[�^�t�@�C���������
        virtual void options( const std::string param_file );
        // �R�}���h�e�[�u���폜
        void clear();
        // �R�}���h���C���擾�̏�����
        virtual void open( void );
        virtual void start( void );
        // �R�}���h���C���擾
        virtual bool get( void );
        virtual bool get( const char* );
        // �T�v�I�v�V�����̎w�萔��Ԃ�
        virtual int getCount(const char*);
        // �I�v�V���������擾�i�����^�j
        virtual std::string str( void ) const;
        // �I�v�V���������擾�i�����^�j
        virtual bool str( std::string );
        // �I�v�V�����p�����[�^�擾(������^)
        virtual std::string param( void ) const;
        // �I�v�V�����p�����[�^�擾(������^)
        virtual std::string param( const char* );
        // �f�t�H���g�I�v�V���������擾
        virtual std::string def( const char* );
		///////////////////////////////////////////////////////
		// �e���v���[�g
		///////////////////////////////////////////////////////
        // �I�v�V�����p�����[�^�擾�i�^�w��-������ȊO�j
        template <typename T1>
        T1 param( void ) const
        {
            T1 retv;
            std::stringstream    ss;
            if(this->m_optparam == "") // �����̃`�F�b�N
            {
				std::string err = "option parameter is invalid null string";
				CmdLineException obj(err.c_str());
				throw obj;
            }
            ss.str(this->m_optparam);
            ss >> retv;
            return retv;
        }
        // �I�v�V�����p�����[�^�p�n�b�V���e�[�u���擾�i�^�w��-������ȊO�j
        template <typename T1>
        T1 hvalue( const std::string prmkey )
        {
            T1 retv;
			std::stringstream    ss;
			ss.str(this->hvalue(prmkey));
			ss >> retv;
            return retv;
        }
        // �I�v�V�����p�����[�^�擾�i�^�w��-������ȊO�j
        template <typename T1>
        T1 param( const char* key )
        {
            T1 retv;
            _T_It_CmdLineType  It;
            if(this->is_param(key) == true)
            {
                std::stringstream    ss;
                It = this->m_cmdline.find(key);
                ss.str(It->second);
                ss >> retv;
            }
            else
            {
				std::string err = "option parameter is invalid null string";
				CmdLineException obj(err.c_str());
				throw obj;
			}
            return retv;
        }
        // �I�v�V�����̃f�t�H���g�擾�i�^�w��j
        template <typename T1>
        T1 def( const char* key )
        {
            T1 retv;
            _T_OptionInfo	tg;
            // �L�[����
            if( this->search(key) == true)
            {
                std::stringstream    ss;
                tg = this->getOption(key);
                ss.str(tg.default_opt);
                ss >> retv;
            }
            else
            {
				// error
				std::string err = key;
				err += ": �Y���I�v�V�����͂���܂���B";
				CmdLineException obj(err.c_str());
				throw obj;
            }
            return retv;
        }
        // �I�v�V�����p�����[�^�̗L��
        virtual bool is_param( const char* );
        ///////////////////////////////////////////////////////
        /// <summary>
        /// �R�}���h�w���v�i�_�C�A���O�\���j
        /// </summary>
        /// <param name="key">�I�v�V����������</param>
        virtual void cmd_helpDlg(const std::string);
        // �R�}���h�w���v
        virtual void cmd_help(const std::string);
		///////////////////////////////////////////////////////
        // �I�y���[�^
        ///////////////////////////////////////////////////////
        // �I�v�V�����o�^
        CommandLine& operator() (
			const char* key,
			const char* desc = "",       // �I�v�V��������
			const int opt_need_flg = _free_param, // �C��
			const int param_flg = _free_param, // �C��
			const char* default_opt = "" // �f�t�H���g�l
		 );
    };
	///////////////////////////////////////////////////////
    // �w���v���o�^�I�y���[�^
    ///////////////////////////////////////////////////////
    template <typename T1 = CommandLine&>
	class HelpDesc
	{
		public:
			typedef T1	first_type;
		private:
			first_type	m_obj;
		public:
			explicit
			HelpDesc(first_type obj):
				m_obj(obj){}
		public:
			HelpDesc& operator()( const char* key, const char* help )
			{
				this->m_obj.setCmdHelp( key, help );
				return *this;
			}
	};
	///////////////////////////////////////////////////////
    // �t�@���N�^
    ///////////////////////////////////////////////////////
    // �R�}���h�o��
    template <typename T1>
    struct cmd_print
    {
    	void operator()( T1 val )
    	{
    	    std::cout << "[" << val << "]" << " ";
    	}
    };
    ///////////////////////////////////////////////////////
    // �I�v�V�����L�[�ɊY������I�v�V������O����v�Ō���
    ///////////////////////////////////////////////////////
    template <typename T1>
    struct SearchKey
    {
        CommandLine::_T_OptionInfo    optinfo;
        std::list<std::string>  m_list;
        std::string m_cstr;
        size_t m_count;
        SearchKey():
            m_cstr(""), m_count(0){}
        SearchKey( const char* key ):
            m_cstr(key), m_count(0){}

        void operator()( T1 val )
        {
            size_t length = m_cstr.size();
            int retv = val.first.compare(0, length, this->m_cstr );
            if (retv == 0)
            {
                m_list.push_back(val.first);
                optinfo = val.second;
                this->m_count++;
            }
        }
    };
    //////////////////////////////////////////////////////////////////////
    // �R�}���h���C������I�v�V�����L�[�ɊY������I�v�V������O����v�Ō���
    //////////////////////////////////////////////////////////////////////
    template <typename T1>
    struct CompKey
    {
        std::string m_cstr;
        CompKey():
            m_cstr(""){}
        CompKey( const char* key ):
            m_cstr(key){}
        bool operator()( T1 val )
        {
            size_t length = m_cstr.size();
            return (val.first.compare(0, length, this->m_cstr ) == 0);
        }
    };
    //////////////////////////////////////////////////////////////////////
    // �R�}���h�w���v�o��
    //////////////////////////////////////////////////////////////////////
    struct CmdHelpDisp
    {
        std::string repstr;
        CmdHelpDisp(std::string val):
            repstr(val){}
        void operator()( const char c )
        {
            std::string l_str;
            l_str = c;
            if (std::string(REPSTR_MARK) == l_str)
            {
                std::cout << repstr;
            }
            else
            {
                std::cout << c;
            }
        }
    };
    //////////////////////////////////////////////////////////////////////
    // �w���v���쐬�e���v���[�g
    // �I�v�V��������(�K�{/�C��): �p�����[�^�i�K�{/�C��/�Ȃ��j[�ȗ��l=...] - ����
    //////////////////////////////////////////////////////////////////////
    template <typename T1>
    struct disp_usage_msg
    {
        std::string* m_usage_str;
        CString opt_str = "";
        std::string sepstr;
        disp_usage_msg(
            const char* cmd,
            std::string sepval,
            std::string* usage_str
        )
        {
            // �w���v������
            this->m_usage_str = usage_str;
            // �w���v���
            this->sepstr = sepval;
            *this->m_usage_str = "Usage: " + std::string(cmd) + " [opition]\n";
        }
        void operator()(T1 val)
        {
            opt_str.Format(
                "%s%-10s",
                this->sepstr.c_str(),
                val.first.c_str()
            );
            *this->m_usage_str += this->opt_str.GetString();
            //std::cout << this->sepstr << std::setw(10) << std::left << val.first;
            *this->m_usage_str += (val.second.opt_need_flg & _NEEDP ? "(�K�{):" : "(�C��):");
            //std::cout << (val.second.opt_need_flg & _NEEDP ? "(�K�{):" : "(�C��):");
            *this->m_usage_str += " �p�����[�^";
            //std::cout << " �p�����[�^";
            std::string para;
            std::string def_para = "";
            if (val.second.param_flg & _NEEDP)
            {
                para = "(�K�{";
                if (val.second.param_flg & _ULMTP)
                {
                    para += "+������)";
                }
                else
                {
                    para += ")";
                }
            }
            else if (val.second.param_flg & _FREEP)
            {
                para = "(�C��";
                def_para = val.second.default_opt;
                if (val.second.param_flg & _ULMTP)
                {
                    para += "+������)";
                }
                else
                {
                    para += ")";
                }
            }
            else if (val.second.param_flg & _NONEP)
            {
                para = "(�Ȃ�)";
            }
            *this->m_usage_str += para;
            //std::cout << para;
            if (val.second.param_flg & _FREEP)
            {
                if (def_para != "")
                {
                    *this->m_usage_str += "[�ȗ��l=" + val.second.default_opt + "]";
                    //std::cout << "[�ȗ��l=" << val.second.default_opt << "]";
                }
                else
                {
                    *this->m_usage_str += "[�ȗ��l=�Ȃ�]";
                    //std::cout << "[�ȗ��l=�Ȃ�]";
                }
            }
            *this->m_usage_str += " - " + val.second.opt_desc + "\n";
            //std::cout << " - " << val.second.opt_desc << std::endl;
        }
    };
    //////////////////////////////////////////////////////////////////////
    // �I�v�V��������(�K�{/�C��): �p�����[�^�i�K�{/�C��/�Ȃ��j[�ȗ��l=...] - ����
    //////////////////////////////////////////////////////////////////////
    template <typename T1>
    struct disp_usage
    {
        std::string sepstr;
    	disp_usage(const char* cmd, std::string sepval)
    	{
    	    this->sepstr = sepval;
    	    std::cout << "Usage: " << cmd << " [opition]" << std::endl;
    	}
    	void operator()( T1 val )
    	{
            std::cout << this->sepstr << std::setw(10) << std::left << val.first;
            std::cout << (val.second.opt_need_flg & _NEEDP? "(�K�{):": "(�C��):");
            std::cout << " �p�����[�^";
            std::string para;
            std::string def_para = "";
            if ( val.second.param_flg & _NEEDP )
            {
                para = "(�K�{";
				if ( val.second.param_flg & _ULMTP )
				{
					para += "+������)";
				}
				else
				{
					para += ")";
				}
            }
            else if ( val.second.param_flg & _FREEP )
            {
                para = "(�C��";
                def_para = val.second.default_opt;
				if ( val.second.param_flg & _ULMTP )
				{
					para += "+������)";
				}
				else
				{
					para += ")";
				}
            }
            else if ( val.second.param_flg & _NONEP )
            {
                para = "(�Ȃ�)";
            }
            std::cout << para;
            if( val.second.param_flg & _FREEP )
            {
                if(def_para != "")
                {
                    std::cout << "[�ȗ��l=" << val.second.default_opt << "]";
                }
                else
                {
                    std::cout << "[�ȗ��l=�Ȃ�]";
                }
            }
            std::cout << " - " << val.second.opt_desc << std::endl;
    	}
    };
    //////////////////////////////////////////////////////////////////////
    // usage�o�͎��̃\�[�g��������
    //////////////////////////////////////////////////////////////////////
    template <typename T1>
    struct Cmp_mdisp
    {
        bool	operator()( T1 lh, T1 rh ) const
        {
            return lh.second.idx < rh.second.idx;
        }
    };
    //////////////////////////////////////////////////////////////////////
    // �����̔���
    //////////////////////////////////////////////////////////////////////
    struct IsNumber
    {
        std::string cstr;
        IsNumber():
            cstr("0123456789-+."){}
    	bool operator()( char c )
    	{
            // ��v�����Ƃ�pos != std::string::npos
            return (this->cstr.find(c) != std::string::npos);
    	}
    };
    //////////////////////////////////////////////////////////////////////
    // �I�v�V�����Z�p���[�^�̃J�E���g�Ɏg�p
    //////////////////////////////////////////////////////////////////////
    struct CountSep
    {
        const std::string sepstr;
        CountSep( const std::string& sep ):
            sepstr(sep){}
    	bool operator()( char c )
    	{
            // sep�Ɠ������Ƃ��J�E���g�A�b�v
            std::string tmpc;
            tmpc = c;
            return (this->sepstr == tmpc);
    	}
    };
    //////////////////////////////////////////////////////////////////////
    // �I�v�V�����p�����[�^�p�n�b�V���e�[�u���̔�r
    //////////////////////////////////////////////////////////////////////
    template <typename T1>
    struct Comp_Prm
    {
        const std::string m_str;
        Comp_Prm( const std::string& prmkey ):
            m_str(prmkey){}
    	bool operator()( T1 lh )
    	{
            return (this->m_str == lh.first);
    	}
    };
}
#endif // COMMANDLINE_H














