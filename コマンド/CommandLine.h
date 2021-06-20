/*
    コマンドラインオプションを取得
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
#define UDP "_udp" // 未定義パラメータキー
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
		// コンストラクタ
		///////////////////////////////////////////////////////
        CommandLine() throw();
        ////////////////////////////////////////////////////////////////
        /// <summary>
        /// コマンドラインコンストラクタ
        /// （コンソールプログラム用）
        /// </summary>
        /// <param name="agc">パラメータ個数</param>
        /// <param name="agv">パラメータテーブル</param>
        /// <param name="valid_flg">オプションなしの許可フラグ</param>
        /// <param name="hlp_opt">ヘルプオプション文字列</param>
        /// <returns>なし</returns>
        CommandLine(
            const int agc,
            const char* agv[],
            bool valid_flg = false,
            std::string hlp_opt = "help"
        ) throw();
        ////////////////////////////////////////////////////////////////
        /// <summary>
        /// コマンドラインコンストラクタ
        /// （Windowsプログラム用）
        /// </summary>
        /// <param name="lpCmdLine">パラメータテーブル</param>
        /// <param name="valid_flg">オプションなしの許可フラグ</param>
        /// <param name="hlp_opt">ヘルプオプション文字列</param>
        /// <returns>なし</returns>
        CommandLine(
            LPWSTR    lpCmdLine,
            bool valid_flg = false,
            std::string hlp_opt = "help"
        ) throw();
        virtual ~CommandLine() throw();
    public:
		///////////////////////////////////////////////////////
        // オプションパラメータ指定方法
        ///////////////////////////////////////////////////////
        enum _T_e_ParamNum
        {
            _none_param = 1,                        // なし
            _need_param = _none_param << 1,     // 必須
            _free_param = _need_param << 1,     // 任意
            _unlimit_param = _free_param << 1   // 無制限パラメータ
        };
        ///////////////////////////////////////////////////////
        // オプション情報コンテナ
        ///////////////////////////////////////////////////////
        struct _T_OptionInfo
        {
            // オプション文字
            std::string optstr;
            // オプションの指定方法:enum _T_e_ParamNum
            // 必須：_need_param、任意：_free_param、無制限パラメータ:_unlimit_param
            int opt_need_flg;
            // オプションパラメータ指定方法:enum _T_e_ParamNum
            // 必須：_need_param、任意：_free_param、無制限パラメータ:_unlimit_param、なし：_none_param
            int param_flg;
            // オプションパラメータ任意の場合のデフォルト値
            std::string default_opt;
            // オプション説明
            std::string opt_desc;
            // 並び順
            int idx;
        };
        ///////////////////////////////////////////////////////
        // 型定義
        ///////////////////////////////////////////////////////
        // コマンドライン文字列オリジナルデータ保存の型名
        typedef std::vector<std::string>                    _T_ArgumentType;
        // コマンドライン引数を保存するコンテナの型名
        typedef std::multimap<std::string, std::string>     _T_mmap_CmdLineType;
        // コマンドライン引数を保存するコンテナの要素型名
        typedef std::pair<std::string, std::string>         _T_p_CmdLineType;
        // コマンドライン引数の保存コンテナのイタレータ型名
        typedef CommandLine::_T_mmap_CmdLineType::iterator  _T_It_CmdLineType;
        // コマンドライン引数の検索レンジの型名
        typedef std::pair<_T_It_CmdLineType,_T_It_CmdLineType>  _T_pr_CmdLineType;
        // ユニークなオプションキー情報を保存するコンテナの型名
        typedef std::map<std::string, _T_OptionInfo>          _T_map_OptionType;
        // ユニークなオプションキー情報を保存するコンテナのイタレータ型名
        typedef CommandLine::_T_map_OptionType::iterator      _T_It_OptionType;
        // ユニークなオプションキー情報を保存するコンテナの要素型名
        typedef std::pair<std::string, _T_OptionInfo>         _T_p_OptionType;
        // コマンドヘルプ用テーブル
        typedef std::multimap<std::string, std::string>     _T_mmap_CmdHelpType;
        /////////////////////////////////////////////////////////////////////////////
        // オプションパラメータ用ハッシュテーブル
        /////////////////////////////////////////////////////////////////////////////
        // --env os=windows ie=ver9.0など。envに対するosとwindows等を格納
        // $1=param_key, $2=param_value
        typedef	std::multimap<std::string, std::string>	_T_OptParamHashType;
        // オプションパラメータ用ハッシュテーブルのイタレータ型名
        typedef CommandLine::_T_OptParamHashType::iterator	_T_It_OptParamHashType;
		// オプションパラメータ用ハッシュテーブル構造
		typedef struct
		{
			CommandLine::_T_OptParamHashType	m_optprm_hash;
			CommandLine::_T_It_OptParamHashType	m_it;
		} _T_OptPrmHashStrct;
		// オプション用ハッシュテーブル
		// $1=optstr, $2=hash strct
		typedef std::map<std::string, CommandLine::_T_OptPrmHashStrct> _T_OptHashType;
		// オプション用ハッシュテーブルのコンテナの要素型名
		// $1=optstr, $2=hash
		typedef std::pair<std::string, CommandLine::_T_OptPrmHashStrct>	_T_p_OptHashType;
		// オプションパラメータ用ハッシュテーブルのコンテナの要素型名
		typedef std::pair<std::string, std::string>	_T_p_OptParamHashType;
    private:
        // オプションパラメータ区切り
        // --param=xxx=yyy=... などに対応
        std::string optprm_sep;
		// オプション無しを許す
		bool	valid_none_opt_flg;
        // 並び順 - used by usage(). set by setOption()
        int m_idx;
        // コマンドライン文字列オリジナルデータ保存
        CommandLine::_T_ArgumentType    m_agv;
        // コマンドライン文字列オリジナルデータ個数保存
        int m_agc;
        // 起動プログラム名保存
        std::string m_prgname;
        // コマンドラインイタレータ
        CommandLine::_T_It_CmdLineType  m_Itcmdline;
        // オプション文字指定子
        std::string    m_sep;
        // オプション文字
        std::string m_optstr;
        // オプションパラメータ
        std::string m_optparam;
        // 必須オプションの指定チェック
        bool chk_need_option( void );
        // ヘルプオプション文字列
        // -ヘルプオプション指定時に必須オプション未指定エラーを
        // -表示しない＆例外を発生させない。
        std::string m_help_option;
    protected:
		// オプション用ハッシュテーブル
		CommandLine::_T_OptHashType		m_opthash;
        // コマンドライン引数を保存するコンテナ
        CommandLine::_T_mmap_CmdLineType  m_cmdline;
        // ユニークなオプションキー情報を保存するコンテナ
        CommandLine::_T_map_OptionType    m_option;
        // コマンドヘルプ用テーブル
        CommandLine::_T_mmap_CmdHelpType m_cmd_help;
        // 指定の要素がオプション文字かどうかを判定
        bool IsOptStr(const int) const;
        // 指定の要素がオプションパラメータ文字かどうかを判定
        bool IsOptParam(const int) const;
    public:
		///////////////////////////////////////////////////////
		// メソッド
		///////////////////////////////////////////////////////
        // 起動オプション文字を検索
        bool is(std::string optstr);
        // オプションハッシュテーブルのprmkeyを取得
		std::string param_key( std::string optkey );
		// オプションハッシュテーブルのprmvalueを取得
		std::string hvalue( std::string optkey );
		// オプションハッシュテーブルにパラメータと値を登録
		void set_optprmhash( std::string optstr, std::string optprm_key, std::string value );
		// オプションハッシュテーブルの存在チェック
		bool is_hash( std::string optkey, const std::string prmkey );
		// オプションハッシュテーブルの取得開始
		void start_hash( std::string optkey );
		// オプションハッシュテーブルの取得（逐次取得型）
		void next_hash( std::string optkey );
		// オプションハッシュテーブルの個数を取得
		ptrdiff_t count_hash( std::string optkey ) const;
		// オプションハッシュテーブルの個数を取得（パラメータキー指定）
		ptrdiff_t count_hash( std::string optkey, std::string prmkey );

        // オプションパラメータ区切り設定
        // --param=xxx=yyy=... などに対応
        void setoptprm_sep(const std::string&);
        // オプションパラメータ区切り取得
        std::string getoptprm_sep() const;
        // セパレータ付きオプション文字取得
        std::string get_OptWithSep( const std::string&, std::string );
        // セパレータ付きオプションパラメータ個数取得
        size_t count_OptPrmWithSep( std::string&, std::string );
        // セパレータ付きオプションパラメータ取得
		typedef std::vector< std::string > _T_OptPrmList;
        _T_OptPrmList get_OptPrmWithSep( std::string&, std::string );
        // コマンドパラメータファイル情報取り込み - used "ParamReader.h"
#if USED_BOOSTLIB
        void setm_agv(ptree_type::const_iterator pos);
        // コマンドパラメータファイルの属性情報取得 - used "ParamReader.h"
        std::string get_attr(ptIT_type pos, std::string attr_name, std::string tag);
#else
        void setm_agv(stdxml::XMLRead* pos);
#endif // USED_BOOSTLIB
        // get agv
        CommandLine::_T_ArgumentType getm_agv();
        // 数字かどうかを判定
        bool is_number( const char* );
        // 数字かどうかを判定（逐次型）
        bool is_number(void);
        // Usage表示
        virtual void usage( const char* );
        ///////////////////////////////////////////////////
        /// <summary>
        /// Usageダイアログ表示
        /// </summary>
        /// <param name="">なし</param>
        virtual void usageDlg(void);
        virtual void usage(void);
//        This is free software; see the source for copying conditions.  There is NO
//        warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
        virtual void version() const;
        // オプション情報登録
        virtual void setOption(
                const char* key,
                const char* desc = "",       // オプション説明
                const int opt_need_flg = _free_param, // 任意
                const int param_flg = _free_param, // 任意
                const char* default_opt = "" // デフォルト値
                );
        // コマンドヘルプ情報登録
        virtual CommandLine& setCmdHelp( const char* key, const char* help );
        //////////////////////////////////////////////////////////
        /// <summary>
        /// コマンドヘルプ情報出力（ダイアログ表示）
        /// </summary>
        /// <param name="key">オプション文字列</param>
        /// <param name="help_str">ヘルプ表示文字列</param>
        virtual void getCmdHelpDlg(
            const char* key,
            std::string& help_str
        );
        // コマンドヘルプ情報出力
        virtual void getCmdHelp( const char* key );
        virtual void getCmdHelp( const std::string key );
        // コマンドライン文字列オリジナルデータ保存
        void setAgv(const int agc, const char* agv[]);
        void setAgv(int agc, char* agv[]);
        /////////////////////////////////////////////////////////
        /// <summary>
        /// コマンドパラメータ変換
        /// Windowsプログラム用
        /// </summary>
        /// <param name="lpCmdLine">コマンドパラメータ</param>
        void setAgv(LPWSTR    lpCmdLine);
        // オプション情報取得
        virtual _T_OptionInfo   getOption( const char* key );
        // オプションキー検索
        virtual bool search( const char* key );
        // オプション文字指定子設定
        void setSep( const char* sp );
        // オプション文字指定子
        std::string getSep( void ) const;
        /// <summary>
        /// コマンドライン情報を解析
        /// </summary>
        /// <param name="ChkNeedFlg">
        /// true:必須オプションチェック、
        /// false:必須オプションチェックなし
        /// （デフォルト：チェックあり）
        /// </param>
        virtual void options( bool ChkNeedFlg = true );
        // コマンドパラメータファイル情報を解析
        virtual void options( const std::string param_file );
        // コマンドテーブル削除
        void clear();
        // コマンドライン取得の初期化
        virtual void open( void );
        virtual void start( void );
        // コマンドライン取得
        virtual bool get( void );
        virtual bool get( const char* );
        // 概要オプションの指定数を返す
        virtual int getCount(const char*);
        // オプション文字取得（逐次型）
        virtual std::string str( void ) const;
        // オプション文字取得（検索型）
        virtual bool str( std::string );
        // オプションパラメータ取得(文字列型)
        virtual std::string param( void ) const;
        // オプションパラメータ取得(文字列型)
        virtual std::string param( const char* );
        // デフォルトオプション文字取得
        virtual std::string def( const char* );
		///////////////////////////////////////////////////////
		// テンプレート
		///////////////////////////////////////////////////////
        // オプションパラメータ取得（型指定-文字列以外）
        template <typename T1>
        T1 param( void ) const
        {
            T1 retv;
            std::stringstream    ss;
            if(this->m_optparam == "") // 数字のチェック
            {
				std::string err = "option parameter is invalid null string";
				CmdLineException obj(err.c_str());
				throw obj;
            }
            ss.str(this->m_optparam);
            ss >> retv;
            return retv;
        }
        // オプションパラメータ用ハッシュテーブル取得（型指定-文字列以外）
        template <typename T1>
        T1 hvalue( const std::string prmkey )
        {
            T1 retv;
			std::stringstream    ss;
			ss.str(this->hvalue(prmkey));
			ss >> retv;
            return retv;
        }
        // オプションパラメータ取得（型指定-文字列以外）
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
        // オプションのデフォルト取得（型指定）
        template <typename T1>
        T1 def( const char* key )
        {
            T1 retv;
            _T_OptionInfo	tg;
            // キー検索
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
				err += ": 該当オプションはありません。";
				CmdLineException obj(err.c_str());
				throw obj;
            }
            return retv;
        }
        // オプションパラメータの有無
        virtual bool is_param( const char* );
        ///////////////////////////////////////////////////////
        /// <summary>
        /// コマンドヘルプ（ダイアログ表示）
        /// </summary>
        /// <param name="key">オプション文字列</param>
        virtual void cmd_helpDlg(const std::string);
        // コマンドヘルプ
        virtual void cmd_help(const std::string);
		///////////////////////////////////////////////////////
        // オペレータ
        ///////////////////////////////////////////////////////
        // オプション登録
        CommandLine& operator() (
			const char* key,
			const char* desc = "",       // オプション説明
			const int opt_need_flg = _free_param, // 任意
			const int param_flg = _free_param, // 任意
			const char* default_opt = "" // デフォルト値
		 );
    };
	///////////////////////////////////////////////////////
    // ヘルプ情報登録オペレータ
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
    // ファンクタ
    ///////////////////////////////////////////////////////
    // コマンド出力
    template <typename T1>
    struct cmd_print
    {
    	void operator()( T1 val )
    	{
    	    std::cout << "[" << val << "]" << " ";
    	}
    };
    ///////////////////////////////////////////////////////
    // オプションキーに該当するオプションを前方一致で検索
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
    // コマンドラインからオプションキーに該当するオプションを前方一致で検索
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
    // コマンドヘルプ出力
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
    // ヘルプ情報作成テンプレート
    // オプション文字(必須/任意): パラメータ（必須/任意/なし）[省略値=...] - 説明
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
            // ヘルプ文字列
            this->m_usage_str = usage_str;
            // ヘルプ情報
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
            *this->m_usage_str += (val.second.opt_need_flg & _NEEDP ? "(必須):" : "(任意):");
            //std::cout << (val.second.opt_need_flg & _NEEDP ? "(必須):" : "(任意):");
            *this->m_usage_str += " パラメータ";
            //std::cout << " パラメータ";
            std::string para;
            std::string def_para = "";
            if (val.second.param_flg & _NEEDP)
            {
                para = "(必須";
                if (val.second.param_flg & _ULMTP)
                {
                    para += "+無制限)";
                }
                else
                {
                    para += ")";
                }
            }
            else if (val.second.param_flg & _FREEP)
            {
                para = "(任意";
                def_para = val.second.default_opt;
                if (val.second.param_flg & _ULMTP)
                {
                    para += "+無制限)";
                }
                else
                {
                    para += ")";
                }
            }
            else if (val.second.param_flg & _NONEP)
            {
                para = "(なし)";
            }
            *this->m_usage_str += para;
            //std::cout << para;
            if (val.second.param_flg & _FREEP)
            {
                if (def_para != "")
                {
                    *this->m_usage_str += "[省略値=" + val.second.default_opt + "]";
                    //std::cout << "[省略値=" << val.second.default_opt << "]";
                }
                else
                {
                    *this->m_usage_str += "[省略値=なし]";
                    //std::cout << "[省略値=なし]";
                }
            }
            *this->m_usage_str += " - " + val.second.opt_desc + "\n";
            //std::cout << " - " << val.second.opt_desc << std::endl;
        }
    };
    //////////////////////////////////////////////////////////////////////
    // オプション文字(必須/任意): パラメータ（必須/任意/なし）[省略値=...] - 説明
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
            std::cout << (val.second.opt_need_flg & _NEEDP? "(必須):": "(任意):");
            std::cout << " パラメータ";
            std::string para;
            std::string def_para = "";
            if ( val.second.param_flg & _NEEDP )
            {
                para = "(必須";
				if ( val.second.param_flg & _ULMTP )
				{
					para += "+無制限)";
				}
				else
				{
					para += ")";
				}
            }
            else if ( val.second.param_flg & _FREEP )
            {
                para = "(任意";
                def_para = val.second.default_opt;
				if ( val.second.param_flg & _ULMTP )
				{
					para += "+無制限)";
				}
				else
				{
					para += ")";
				}
            }
            else if ( val.second.param_flg & _NONEP )
            {
                para = "(なし)";
            }
            std::cout << para;
            if( val.second.param_flg & _FREEP )
            {
                if(def_para != "")
                {
                    std::cout << "[省略値=" << val.second.default_opt << "]";
                }
                else
                {
                    std::cout << "[省略値=なし]";
                }
            }
            std::cout << " - " << val.second.opt_desc << std::endl;
    	}
    };
    //////////////////////////////////////////////////////////////////////
    // usage出力時のソート順を処理
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
    // 数字の判定
    //////////////////////////////////////////////////////////////////////
    struct IsNumber
    {
        std::string cstr;
        IsNumber():
            cstr("0123456789-+."){}
    	bool operator()( char c )
    	{
            // 一致したときpos != std::string::npos
            return (this->cstr.find(c) != std::string::npos);
    	}
    };
    //////////////////////////////////////////////////////////////////////
    // オプションセパレータのカウントに使用
    //////////////////////////////////////////////////////////////////////
    struct CountSep
    {
        const std::string sepstr;
        CountSep( const std::string& sep ):
            sepstr(sep){}
    	bool operator()( char c )
    	{
            // sepと等しいときカウントアップ
            std::string tmpc;
            tmpc = c;
            return (this->sepstr == tmpc);
    	}
    };
    //////////////////////////////////////////////////////////////////////
    // オプションパラメータ用ハッシュテーブルの比較
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














