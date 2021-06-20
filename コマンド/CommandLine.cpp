// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#include <Windows.h>
#include <shellapi.h>
#include "CommandLine.h"
#include "Utility.h"
using namespace std;
using namespace locallib;
CommandLine::CommandLine() throw()
{
    this->m_idx = 0;
    this->m_sep = OPTSEP;
	this->valid_none_opt_flg = false;
	this->m_help_option = "help";
}
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
CommandLine::CommandLine(
    const int agc, 
    const char* vagv[], 
    bool valid_flg, 
    std::string hlp_opt
) throw()
{
    this->m_idx = 0;
    this->m_sep = OPTSEP;
    this->m_agc = agc;
    this->m_prgname = vagv[0];
    this->setAgv(agc, vagv);
    this->valid_none_opt_flg = valid_flg;
    this->m_help_option = hlp_opt;
}
////////////////////////////////////////////////////////////////
/// <summary>
/// コマンドラインコンストラクタ
/// （Windowsプログラム用）
/// </summary>
/// <param name="lpCmdLine">パラメータテーブル</param>
/// <param name="valid_flg">オプションなしの許可フラグ</param>
/// <param name="hlp_opt">ヘルプオプション文字列</param>
/// <returns>なし</returns>
CommandLine::CommandLine(
    LPWSTR    lpCmdLine,
    bool valid_flg,
    std::string hlp_opt
) throw()
{
    this->m_idx = 0;
    this->m_sep = OPTSEP;
    this->m_agc = 0;
    // lpCmdLineをchar *agv[]形式に変換
    // Windowsアプリ用コマンドライン設定
    this->setAgv(lpCmdLine);
    this->valid_none_opt_flg = valid_flg;
    this->m_help_option = hlp_opt;
}
CommandLine::~CommandLine() throw()
{
    //dtor
    this->m_cmdline.clear();
    this->m_option.clear();
}
///////////////////////////////////////////////////
/// <summary>
/// Usageダイアログ表示
/// </summary>
/// <param name="">なし</param>
void CommandLine::usageDlg(void)
{
    vector<_T_p_OptionType> mdisp(this->m_option.size());
    // オプションのコピー
    copy(
        this->m_option.begin(),
        this->m_option.end(),
        mdisp.begin()
    );
    // オプションをm_idxでソート
    stable_sort(
        mdisp.begin(),
        mdisp.end(),
        Cmp_mdisp<_T_p_OptionType>()
    );
    // オプション文字(必須/任意): 引数（必須/任意）　- 意味
    std::string usage_str = "";
    disp_usage_msg <_T_p_OptionType>  disp_msg(
        this->m_prgname.c_str(),
        this->getSep(),
        &usage_str
    );
    for_each(
        mdisp.begin(),
        mdisp.end(),
        // disp_msgはmdispの要素を引数に繰り返し呼ばれる
        disp_msg
    );
    // 使用方法をダイアログ表示
    ::MessageBox(
        nullptr,
        usage_str.c_str(),
        "使用方法",
        MB_ICONINFORMATION
    );
}
// Usage表示
void CommandLine::usage( const char* cmd )
{
    vector<_T_p_OptionType> mdisp(this->m_option.size());
    // オプションのコピー
    copy(
        this->m_option.begin(), 
        this->m_option.end(), 
        mdisp.begin()
    );
    // オプションをm_idxでソート
    stable_sort(
        mdisp.begin(), 
        mdisp.end(), 
        Cmp_mdisp<_T_p_OptionType>()
    );
    // オプション文字(必須/任意): 引数（必須/任意）　- 意味
    for_each(
        mdisp.begin(), 
        mdisp.end(), 
        // disp_usageはmdispの要素を引数に繰り返し呼ばれる
        disp_usage<_T_p_OptionType>(cmd, this->getSep())
    );
}
void CommandLine::usage(void)
{
	this->usage( this->m_prgname.c_str() );
}
// get agv
CommandLine::_T_ArgumentType CommandLine::getm_agv()
{
    return this->m_agv;
}
// コマンドライン文字列オリジナルデータ保存
void CommandLine::setAgv(const int agc, const char* agv[])
{
    // コンテナにagv[]の値をコピー
    for(int ii = 0; ii < agc; ii++)
    {
        this->m_agv.push_back(agv[ii]);
    }
}
/////////////////////////////////////////////////////////
/// <summary>
/// コマンドパラメータ変換
/// Windowsプログラム用
/// </summary>
/// <param name="lpCmdLine">コマンドパラメータ</param>
void CommandLine::setAgv(LPWSTR lpCmdLine)
{
    LPWSTR* szArglist;
    int nArgs;

    this->m_agv.clear();
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if (NULL == szArglist)
    {
        return;
    }
    else
    {
        // パラメータカウント
        this->m_agc = nArgs;
        // パラメータテーブルに格納
        for (int i = 0; i < nArgs; i++)
        {
            // WからAに変換
            CString wstr = szArglist[i];
            this->m_agv.push_back(wstr.GetString());
        }
        // プログラム名
        this->m_prgname = this->m_agv[0];
    }
    // Free memory allocated for CommandLineToArgvW arguments.
    LocalFree(szArglist);
}
// コマンドヘルプ情報登録
CommandLine& CommandLine::setCmdHelp( const char* key, const char* help )
{
    string strkey = key;
    string strhelp = help;
    // @ -> option string converting
    strhelp = StrReplace(strhelp)(REPSTR_MARK, this->getSep());
    this->m_cmd_help.insert(_T_p_CmdLineType(strkey, strhelp));
    return *this;
}
//////////////////////////////////////////////////////////
/// <summary>
/// コマンドヘルプ情報出力（ダイアログ表示）
/// </summary>
/// <param name="key">オプション文字列</param>
/// <param name="help_str">ヘルプ表示文字列</param>
void CommandLine::getCmdHelpDlg(
    const char* key,
    std::string& help_str
)
{
    _T_mmap_CmdHelpType::iterator    it = this->m_cmd_help.begin();
    bool flg = false;
    for (; it != this->m_cmd_help.end();)
    {
        it = find_if(it, this->m_cmd_help.end(), CompKey<_T_p_CmdLineType>(key));
        if ((it != this->m_cmd_help.end()))
        {
            help_str += it->second;
            //cout << it->second << endl;
            it++;
            flg = true;
        }
    }
    if (flg == false)
    {
        help_str = std::string(key) + " is no description.";
        //cout << "is no description." << endl;
    }
}
// コマンドヘルプ情報出力
void CommandLine::getCmdHelp( const char* key )
{
    _T_mmap_CmdHelpType::iterator    it = this->m_cmd_help.begin();
    bool flg = false;
    for(; it != this->m_cmd_help.end();)
    {
        it = find_if(it, this->m_cmd_help.end(), CompKey<_T_p_CmdLineType>(key));
        if( (it != this->m_cmd_help.end()) )
        {
            cout << it->second << endl;
            it++;
            flg = true;
        }
    }
    if(flg == false)
    {
        cout << "is no description." << endl;
    }
}
void CommandLine::getCmdHelp( const std::string key )
{
    this->getCmdHelp(key.c_str());
}
///////////////////////////////////////////////////////
/// <summary>
/// コマンドヘルプ（ダイアログ表示）
/// </summary>
/// <param name="key">オプション文字列</param>
void CommandLine::cmd_helpDlg(const std::string key)
{
    std::string help_str = "";
    CommandLine::_T_OptionInfo retv;
    string keystr = key;
    if (this->search(key.c_str()))
    {
        // 対応するオプションがあれば情報取得
        retv = this->getOption(key.c_str());
        keystr = retv.optstr;
    }
    else
    {
        keystr += " is not command.";
        help_str = keystr;
        ::MessageBox(
            nullptr,
            help_str.c_str(),
            "",
            MB_ICONINFORMATION
        );
        //cout << keystr << " is not command." << endl;
        return;
    }
    keystr += " command format:";
    help_str = keystr;
    //cout << keystr << " command format:" << endl;
    this->getCmdHelpDlg(key.c_str(), help_str);
    ::MessageBox(
        nullptr,
        help_str.c_str(),
        "コマンドオプションヘルプ",
        MB_ICONINFORMATION
    );
}
// コマンドヘルプ
void CommandLine::cmd_help(const std::string key)
{
    CommandLine::_T_OptionInfo retv;
    string keystr = key;
    if ( this->search(key.c_str()) )
    {
        // 対応するオプションがあれば情報取得
        retv = this->getOption(key.c_str());
        keystr = retv.optstr;
    }
    else
    {
        cout << keystr << " is not command." << endl;
        return;
    }
    cout << keystr << " command format:" << endl;
    this->getCmdHelp(keystr);
}
// オプション情報登録
void CommandLine::setOption(
        const char* key,
        const char* desc,  // オプションの説明
        const int opt_need_flg, // 任意
        const int param_flg, // 任意
        const char* default_opt // デフォルト値
        )
{
    _T_OptionInfo tmpcont;
    if (*key == (char)NULL)
    {
        string err = desc;
        err += ",";
        err += default_opt;
        err += " :オプションキーが未指定です。";
        throw CmdLineException(err.c_str());
    }
    else if (opt_need_flg & param_flg & _ULMTP)
    {
        string err = key;
        err += ": ";
        err += desc;
        err += ": ";
        err += "オプション文字とパラメータの両方とも複数指定はできません。\n";
        err += "該当するキーをCommandLine::setOption(...)で再度設定して下さい。";
        throw CmdLineException(err.c_str());
    }
    tmpcont.optstr = key;
    tmpcont.opt_need_flg = opt_need_flg;
    tmpcont.param_flg = param_flg;
    tmpcont.default_opt = default_opt;
    tmpcont.opt_desc = desc;
    // @ -> option string converting
    tmpcont.opt_desc = StrReplace(tmpcont.opt_desc)(REPSTR_MARK, this->getSep());
    tmpcont.idx = this->m_idx;
    this->m_option[tmpcont.optstr] = tmpcont;
    this->m_idx++;
}
// オプションキー検索
bool CommandLine::search( const char* key )
{
    // 前方一致でキーを検索
    typedef SearchKey< _T_p_OptionType >  _T_SrchType;
    _T_SrchType   res =
        for_each(this->m_option.begin(), this->m_option.end(), _T_SrchType(key));
    // 複数一致する場合は、エラーとする
    return res.m_count == 1;
}
// オプション情報取得
CommandLine::_T_OptionInfo CommandLine::getOption( const char* key )
{
    // 前方一致でキーを検索
    typedef SearchKey< _T_p_OptionType >  _T_SrchType;
    _T_SrchType   res =
        for_each(this->m_option.begin(), this->m_option.end(), _T_SrchType(key));
    return res.optinfo;
}
// オプション文字指定子設定
void CommandLine::setSep( const char* sp )
{
    this->m_sep = sp;
}
// オプション文字指定子
string CommandLine::getSep( void ) const
{
    return this->m_sep;
}
// 指定の要素がオプション文字かどうかを判定
bool CommandLine::IsOptStr(const int idx) const
{
    bool retv;
    if(this->m_agc > idx)
    {
        string optparam = this->m_agv[idx];
        string sepstr = this->getSep();
        // 先頭の文字と部分比較
        retv = (optparam.compare(0, sepstr.size(), sepstr) == 0);
    }
    else
    {
        retv = false;
    }
    return retv;
}
// 指定の要素がオプションパラメータ文字かどうかを判定
bool CommandLine::IsOptParam(const int idx) const
{
    bool retv;
    if(this->m_agc > idx)
    {
        string optparam = this->m_agv[idx];
        string sepstr = this->getSep();
        // 先頭の文字と部分比較
        retv = optparam.compare(0, sepstr.size(), sepstr) != 0;
    }
    else
    {
        retv = false;
    }
    return retv;
}
// コマンドテーブル削除
void CommandLine::clear()
{
    this->m_cmdline.clear();
    this->m_agv.clear();
}
// コマンドパラメータファイル情報取り込み by "ParamReader.h"
#if USED_BOOSTLIB
void CommandLine::setm_agv(ptree_type::const_iterator pos)
{
    const ptree_type& node =  pos->second;

    // 1. タグ名の取得
    this->set_tag(pos->first);

    // 2. 属性の取得
    if (node.find("<xmlattr>") != node.not_found())
    {
        this->m_agv.push_back( this->get_attr(node.find("<xmlattr>"), "name", "opt") );
    }
    else if ( this->child_count(node) == 0 )
    {
        this->m_agv.push_back(pos->second.data());
    }
    else
    {
        // 3. テキスト，または子ノードの出力．
		// コメント以外を取得
        for (ptree_type::const_iterator child = node.begin(); child != node.end(); ++child) {
            if (child->first == "<xmlattr>" || child->first == "<xmlcomment>")
            {
                continue;
            }
            this->setm_agv(child);
        }
    }
}
// コマンドパラメータファイルの属性情報取得
string CommandLine::get_attr(ptIT_type pos, string attr_name, string ltag)
{
    const ptree_type& node = pos->second;
    this->set_attr_value("");
    for (ptree_type::const_iterator attr = node.begin(); attr != node.end(); ++attr) {
        if (attr->first == attr_name)
        {
            if (!attr->second.data().empty()) {
                this->set_attr_value(attr->second.data());
                if (this->get_tag() == ltag)
                {
                    this->set_attr_value(this->getSep() + this->get_attr_value());
                }
                break;
            }
        }
    }
    return this->get_attr_value();
}
#else
// Windowsネイティブ版
void CommandLine::setm_agv(stdxml::XMLRead* pos)
{
    // オプション文字列に展開
    for (auto& var : this->lpxml->get_xml())
    {
        if (var.ntype == "XmlOption")
        {
            // オプション指定文字列設定
            this->set_attr_value(this->getSep() + var.value.GetString());
        }
        else
        {
            // オプション値文字列設定
            this->set_attr_value(var.value.GetString());
        }
        this->m_agv.push_back(this->get_attr_value());
    }
}
#endif
// コマンドパラメータファイル情報を解析
void CommandLine::options( const string param_file )
{
    // コマンドライン情報クリア
    this->clear();
    // プログラム名設定
    this->m_agv.push_back(this->m_prgname);
    // オプション設定
#if USED_BOOSTLIB
    boost::property_tree::ptree root;
    boost::property_tree::read_xml(param_file, root);
    // オプション文字と値を設定
    this->setm_agv(root.begin());
#else
    // パラメータファイルパスを設定する
    this->lpxml->SetFilePath(param_file.c_str());
    // XML解析
    if (this->lpxml->GenXML() == FALSE)
    {
        return;
    }
    this->lpxml->XMLReader();
    // オプション文字と値を設定
    this->setm_agv(this->lpxml);
#endif // USED_BOOSTLIB
    this->m_agc = this->m_agv.size();
    // コマンドラインのオプション情報を取得
    this->options();
}
/// <summary>
/// コマンドライン情報を解析
/// </summary>
/// <param name="ChkNeedFlg">
/// true:必須オプションチェック、
/// false:必須オプションチェックなし
/// （デフォルト：チェックあり）
/// </param>
void CommandLine::options(bool ChkNeedFlg)
{
    // コマンドラインカウント分
    string optstr;
    string optparam;
    string sepstr = this->getSep();
    int agc = this->m_agv.size();
    // オプションセパレータ文字数設定
    size_t sep_length = this->getSep().size();
    for( int ii = 1; ii < agc; ii++ )
    {
        optstr = this->m_agv[ii];
        // オプション指定子があるか
        if (this->IsOptStr(ii))
        {
            if (optstr.size() > sep_length)
            {
                optstr = optstr.substr(sep_length, string::npos);
            }
            else
            {
            	// オプションセパレータのみの場合は
            	// 複数オプションとの区切りとして扱う
				continue;
            }
        }
        else
        {
            // オプション文字以外が指定されている。
            string optkey = UDP;
            optparam = this->m_agv[ii];
            this->m_cmdline.insert(_T_p_CmdLineType(optkey, optparam));
            continue;
        }
        // 登録済みオプション文字を検索　- パラメータを取得するため
        if ( this->search( this->get_OptWithSep(optstr, this->getoptprm_sep()).c_str()) )
        {
            // 対応するオプションがあれば登録
            CommandLine::_T_OptionInfo retv =
				this->getOption(this->get_OptWithSep(optstr, this->getoptprm_sep()).c_str());
            // オプションを登録
            // 単一オプションのチェック
            if( (retv.opt_need_flg & _unlimit_param) == 0 )
            {
                // コマンドラインthis->m_cmdlineに登録されたオプションを検索
                if(this->str(retv.optstr))
                {
                    // error
                    string err = sepstr + retv.optstr + "は複数指定できません。";
                    throw CmdLineException(err.c_str());
                }
            }
            //////////////////////////////////////////////////////
            // オプションパラメータの指定チェック開始			//
            //////////////////////////////////////////////////////
            // パラメータは単一指定で必須
            if ( retv.param_flg == _NEEDP )
            {
            	// セパレータ付きオプションパラメータを取得 --param=xxx=yyy=... などに対応
            	if ( this->getoptprm_sep().size() > 0 &&
					this->count_OptPrmWithSep( optstr, this->getoptprm_sep()) > 0 )
				{
					_T_OptPrmList	optprm_list =
						this->get_OptPrmWithSep( optstr, this->getoptprm_sep() );
					_T_OptPrmList::iterator	It = optprm_list.begin();
					if ( optprm_list.size() > 0 )
					{
						string l_optparam = *It;
						this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, l_optparam));
					}
					continue;
				}

                // 次の文字がオプションパラメータ文字か判定
                if (this->IsOptParam(ii+1))
                {
                    optparam = this->m_agv[ii+1];
                    // オプションパラメータ用ハッシュデータ登録
                    if ( this->count_OptPrmWithSep( optparam, this->getoptprm_sep()) > 0 )
                    {
						string prmkey;
						string prmvalue;
						prmkey = this->get_OptWithSep(optparam, this->getoptprm_sep());
						_T_OptPrmList	prmv_list =
							this->get_OptPrmWithSep( optparam, this->getoptprm_sep() );
						_T_OptPrmList::iterator	It = prmv_list.begin();
						if ( prmv_list.size() > 0 )
						{
							prmvalue = *It;
							this->set_optprmhash( retv.optstr, prmkey, prmvalue );
							this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
						}
						// 通常オプションパラメータ登録
						else
						{
							this->set_optprmhash( retv.optstr, prmkey, "" );
							this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
						}
                    }
                    // 通常オプションパラメータ登録
                    else
                    {
						this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, optparam));
                    }
                    ii++;
                }
                else
                {
                    // error
                    string err = sepstr + retv.optstr + ":オプションパラメータを指定してください。";
                    throw CmdLineException(err.c_str());
                }
            }
            //////////////////////////////////////////////////////
            // オプションパラメータの指定チェック開始			//
            //////////////////////////////////////////////////////
            // パラメータは無制限（０個以上）
            else if (retv.param_flg & _ULMTP)
            {
				// セパレータ付きオプションパラメータを取得 --param=xxx=yyy=... などに対応
            	if ( this->getoptprm_sep().size() > 0 &&
					this->count_OptPrmWithSep( optstr, this->getoptprm_sep()) > 0 )
				{
					_T_OptPrmList	optprm_list =
						this->get_OptPrmWithSep( optstr, this->getoptprm_sep() );
					_T_OptPrmList::iterator	It = optprm_list.begin();
					// オプションパラメータがある場合
					if ( optprm_list.size() > 0 )
					{
                        for ( ;It != optprm_list.end(); It++ )
                        {
                            string l_optparam = *It;
                            this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, l_optparam));
                        }
					}
                    // オプションパラメータがない
                    // オプションパラメータが必須の場合
                    else if (retv.param_flg & _NEEDP)
                    {
                        string err = sepstr + retv.optstr + ":オプションパラメータを指定してください。";
                        throw CmdLineException(err.c_str());
                    }
                    // オプションパラメータが任意の場合
                    else
                    {
                        // デフォルトパラメータを設定
                        this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
                    }
					continue;
				}

                // オプションパラメータがある
                if(this->IsOptParam(ii+1))
                {
					int jj;
					for(jj = ii+1; jj < agc; jj++)
					{
						// パラメータである
						if(this->IsOptParam(jj))
						{
							string l_optparam = this->m_agv[jj];
							// オプションパラメータ用ハッシュデータ登録
							if ( this->count_OptPrmWithSep( l_optparam, this->getoptprm_sep()) > 0 )
							{
								string prmkey;
								string prmvalue;
								prmkey = this->get_OptWithSep(l_optparam, this->getoptprm_sep());
								_T_OptPrmList	prmv_list =
									this->get_OptPrmWithSep( l_optparam, this->getoptprm_sep() );
								_T_OptPrmList::iterator	It = prmv_list.begin();
								for ( ;It != prmv_list.end(); It++ )
								{
									prmvalue = *It;
									this->set_optprmhash( retv.optstr, prmkey, prmvalue );
									this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
								}
								// 通常オプションパラメータ登録
								if ( prmv_list.size() == 0 )
								{
									this->set_optprmhash( retv.optstr, prmkey, "" );
									this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
								}
							}
							// 通常オプションパラメータ登録
							else
							{
								this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, l_optparam));
							}
							ii = jj;
						}
						// オプション文字
						else
						{
							break;
						}
					}
                }
                // オプションパラメータがない
                // オプションパラメータが必須の場合
                else if (retv.param_flg & _NEEDP)
                {
                    string err = sepstr + retv.optstr + ":オプションパラメータを指定してください。";
                    throw CmdLineException(err.c_str());
                }
                // オプションパラメータが任意の場合
                else
                {
                    // デフォルトパラメータを設定
                    this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
                }
            }
            //////////////////////////////////////////////////////
            // オプションパラメータの指定チェック開始			//
            //////////////////////////////////////////////////////
            // パラメータは単一で任意
            else if ( retv.param_flg == _FREEP )
            {
            	// セパレータ付きオプションパラメータを取得 --param=xxx=yyy=... などに対応
            	if ( this->getoptprm_sep().size() > 0 &&
					this->count_OptPrmWithSep( optstr, this->getoptprm_sep()) > 0 )
				{
					_T_OptPrmList	optprm_list =
						this->get_OptPrmWithSep( optstr, this->getoptprm_sep() );
					_T_OptPrmList::iterator	It = optprm_list.begin();
					if ( optprm_list.size() > 0 )
					{
						string l_optparam = *It;
						this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, l_optparam));
					}
					else
					{
					    // デフォルトパラメータを設定
					    this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
					}
					continue;
				}

                // オプションパラメータ
                if ( this->IsOptParam(ii+1) )
                {
                    optparam = this->m_agv[ii+1];
                    // オプションパラメータ用ハッシュデータ登録
                    if ( this->count_OptPrmWithSep( optparam, this->getoptprm_sep()) > 0 )
                    {
						string prmkey;
						string prmvalue;
						prmkey = this->get_OptWithSep(optparam, this->getoptprm_sep());
						_T_OptPrmList	prmv_list =
							this->get_OptPrmWithSep( optparam, this->getoptprm_sep() );
						_T_OptPrmList::iterator	It = prmv_list.begin();
						if ( prmv_list.size() > 0 )
						{
							prmvalue = *It;
							this->set_optprmhash( retv.optstr, prmkey, prmvalue );
							this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
						}
						// 通常オプションパラメータ登録
						else
						{
							this->set_optprmhash( retv.optstr, prmkey, "" );
							this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
						}
                    }
                    // 通常オプションパラメータ登録
					else
					{
						this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, optparam));
					}
                    ii++;
                }
                else
                {
                    // デフォルトパラメータを設定
                    this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
                }
            }
            // パラメータなし
            else if ( retv.param_flg == _NONEP )
            {
                optparam = "";
                this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, optparam));
                continue;
            }
        }
        else
        {
            string err = sepstr + optstr + " :無効なオプションが指定されました。";
            throw CmdLineException(err.c_str());
        }
    }
    // ヘルプオプションが指定されていたらエラーとはならない。
    if (!this->str(this->m_help_option))
    {
        // 必須オプションチェックフラグがtrueの場合だけチェック
        if (ChkNeedFlg == true)
        {
            this->chk_need_option(); // if error then throw
        }
        if ( this->m_cmdline.size() == 0 && this->valid_none_opt_flg == false )
        {
            for(int ii = 0; ii < this->m_agc; ii++)
            {
                cout << this->m_agv[ii] << " ";
            }
            cout << endl;
            throw CmdLineException("有効なオプションが指定されておりません。");
        }
    }
}
// 必須オプションの指定チェック
bool CommandLine::chk_need_option( void )
{
    // コマンドラインに必須オプションが指定されているかをチェック
    CommandLine::_T_It_OptionType   it_opt = this->m_option.begin();
    CommandLine::_T_It_OptionType   end_opt = this->m_option.end();
    CommandLine::_T_It_CmdLineType  end_cmd = this->m_cmdline.end();
    bool retv = true;
    string  need_opt = "";
    string sepstr = this->getSep();
    for(; it_opt != end_opt; it_opt++)
    {
        if( it_opt->second.opt_need_flg & this->_need_param )
        {
            if(this->m_cmdline.find(it_opt->first) != end_cmd)
            {
                continue;
            }
            else
            {
                retv = false;
                need_opt += sepstr + it_opt->first + " ";
            }
        }
    }
    if(retv == false)
    {
        string err = need_opt + ":必須オプションが指定されていません。";
        throw CmdLineException(err.c_str());
    }
    else
    {
        retv = true;
    }
    return retv;
}
// コマンドライン取得の初期化
void CommandLine::open( void )
{
    this->m_Itcmdline = this->m_cmdline.begin();
}
void CommandLine::start( void )
{
    this->m_Itcmdline = this->m_cmdline.begin();
}
// コマンドライン取得
bool CommandLine::get( void )
{
    bool retv;
    if ( this->m_Itcmdline != this->m_cmdline.end() )
    {
        this->m_optstr = this->m_Itcmdline->first;
        this->m_optparam = this->m_Itcmdline->second;
        this->m_Itcmdline++;
        retv = true;
    }
    else
    {
        retv = false;
    }
    return retv;
}
// 該当オプションの指定数を返す
int CommandLine::getCount(const char* key)
{
    return count_if(this->m_cmdline.begin(), this->m_cmdline.end(), CompKey<_T_p_CmdLineType>(key));
}
bool CommandLine::get( const char* key )
{
    bool retv = false;
    _T_It_CmdLineType It_retv =
    find_if(this->m_Itcmdline, this->m_cmdline.end(), CompKey<_T_p_CmdLineType>(key));
    if(It_retv != this->m_cmdline.end())
    {
        this->m_optstr = It_retv->first;
        this->m_optparam = It_retv->second;
        this->m_Itcmdline = ++It_retv;
        retv = true;
    }
    return retv;
}
// オプション文字取得（逐次型）
std::string CommandLine::str( void ) const
{
    return this->m_optstr;
}
// オプション文字取得（検索型+逐次型）
bool CommandLine::str( std::string vstr )
{
	this->start();
    return this->get(vstr.c_str());
}
// オプションパラメータ取得
string CommandLine::param( void ) const
{
	return this->m_optparam;
}
string CommandLine::def( const char* key )
{
    string retv;
    _T_OptionInfo	tg;
    // キー検索
    if( this->search(key) == true)
    {
        tg = this->getOption(key);
        retv = tg.default_opt;
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

// オプションパラメータ取得
string CommandLine::param( const char* key)
{
	string retv = "";
	_T_It_CmdLineType  It;
	if(this->is_param(key) == true)
	{
		It = this->m_cmdline.find(key);
		retv = It->second;
	}
	else
	{
		string err = "option parameter is invalid null string";
		throw CmdLineException(err.c_str());
	}
	return retv;
}
// オプションパラメータの有無
bool CommandLine::is_param( const char* key)
{
    _T_It_CmdLineType  It = this->m_cmdline.find(key);
    bool retv = (It != this->m_cmdline.end());
    if(retv)
    {
        retv = (It->second != "");
    }
    return retv;
}
// 数字かどうかを判定
bool CommandLine::is_number( const char* key )
{
    if(this->is_param(key) == false)
    {
        // 該当するキーがない
        string err = key;
        err += ": 該当するキーがありません。";
        throw CmdLineException(err.c_str());
    }
    // パラメータは数字かどうかを判別
    string param_str = this->param(key);
    bool retv = (find_if(param_str.begin(), param_str.end(), IsNumber()) == param_str.end());
    return retv;
}
// 数字かどうかを判定（逐次型）
bool CommandLine::is_number(void)
{
    // パラメータは数字かどうかを判別
    string param_str = this->param();
    bool retv = (find_if(param_str.begin(), param_str.end(), IsNumber()) == param_str.end());
    return retv;
}
//        This is free software; see the source for copying conditions.  There is NO
//        warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
void CommandLine::version() const
{
    cout << "FileSystem Command Colection version 1.0.0" << endl;
    cout << "Copyright (C) 2012 by davinci1958@jcom.home.ne.jp" << endl;
    cout << "This is free software; see the source for copying conditions." << endl;
    cout << "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl;
}
// オプション登録
CommandLine& CommandLine::operator()(
        const char* key,
        const char* desc,       // オプション説明
        const int opt_need_flg, // 任意
        const int param_flg, // 任意
        const char* default_opt // デフォルト値
)
{
    this->setOption(key, desc, opt_need_flg, param_flg, default_opt);
    return *this;
}
// オプションパラメータ区切り
// --param=xxx=yyy=... などに対応
void CommandLine::setoptprm_sep(const std::string& sep)
{
    this->optprm_sep = sep;
}
// オプションパラメータ区切り取得
string CommandLine::getoptprm_sep() const
{
    return this->optprm_sep;
}
// オプションパラメータ区切り設定
// セパレータ付きオプション文字取得
string CommandLine::get_OptWithSep( const string& optstr, string sep )
{
	string retv = optstr;

	// セパレータが指定
	if ( sep.size() > 0 )
	{
		size_t idx = optstr.find( sep );
		if ( idx != string::npos )
		{
			retv = optstr.substr( 0, idx );
		}
	}
	return retv;
}
// セパレータ付きオプションパラメータ個数取得
size_t CommandLine::count_OptPrmWithSep( string& optstr, string sep )
{
	return (size_t)count_if( optstr.begin(), optstr.end(), CountSep(sep));
}
// セパレータ付きオプションパラメータ取得
CommandLine::_T_OptPrmList CommandLine::get_OptPrmWithSep( string& optstr, string sep )
{
	CommandLine::_T_OptPrmList	retv;
	StrReplace	repobj;
	string tmpstr;
	size_t idx = optstr.find( sep, 0 );
	if ( this->count_OptPrmWithSep( optstr, sep ) > 0 )
	{
		// オプションパラメータ以降をコピー
		if ( idx+1 < optstr.size() )
		{
			tmpstr = optstr.substr( idx+1, string::npos );
		}
		else
		{
			return retv;
		}
	}
	else
	{
		return retv;
	}
	//文字列中のブランクを0x1Fに変換
	char cnvstr[2];
	cnvstr[0] = 0x1F;
	cnvstr[1] = static_cast<char>(NULL);
	repobj.replace( tmpstr, " ", cnvstr );
	// 文字列中のsepをブランクに変換
	repobj.replace( tmpstr, sep, " " );

	size_t length = tmpstr.size();
	// ブランクで区切られたオプションパラメータを登録
	size_t last_pos = 0;
	for ( size_t pos = 0; pos < length; last_pos = pos = idx+1 )
	{
		idx = tmpstr.find(" ", pos);
		if ( idx != string::npos && pos < length )
		{
			stringstream ss;
			string tmp2 = tmpstr.substr( pos, string::npos );
			ss.str(tmp2);
			ss >> tmp2;
			repobj.replace( tmp2, cnvstr, " ");
			retv.push_back( tmp2 );
		}
		else
		{
			if ( last_pos < length )
			{
				pos = last_pos;
				stringstream ss;
				string tmp2 = tmpstr.substr( pos, string::npos );
				ss.str(tmp2);
				ss >> tmp2;
				repobj.replace( tmp2, cnvstr, " ");
				retv.push_back( tmp2 );
			}
			break;
		}
	}
	return retv;
}
// オプションハッシュテーブルにパラメータと値を登録
void CommandLine::set_optprmhash( std::string optstr, std::string optprm_key, std::string value )
{
	this->m_opthash[optstr].m_optprm_hash.insert( CommandLine::_T_p_OptParamHashType( optprm_key, value ));
	this->m_opthash[optstr].m_it =	this->m_opthash[optstr].m_optprm_hash.end();
}
// オプションハッシュテーブルのprmkey存在チェック
bool CommandLine::is_hash( std::string optkey, const std::string prmkey )
{
	bool retv = false;
	_T_OptHashType::iterator it_opthash;
	if ( (it_opthash = this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		// 現在位置からサイクリックに検索
		_T_It_OptParamHashType	it_prm;
		for ( int ii = 0; ii < 2; ii++ )
		{
			it_opthash->second.m_it =
				std::find_if(it_opthash->second.m_it, it_opthash->second.m_optprm_hash.end(),
						Comp_Prm<_T_p_OptParamHashType>(prmkey));
			retv = (it_opthash->second.m_it != it_opthash->second.m_optprm_hash.end());

			if ( retv == false )
			{
				it_opthash->second.m_it = it_opthash->second.m_optprm_hash.begin();
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		retv = false;
	}
	return retv;
}
// オプションハッシュテーブルの取得開始
void CommandLine::start_hash( std::string optkey )
{
	_T_OptHashType::iterator it_opthash;
	if ( (it_opthash=this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		if ( it_opthash->second.m_it == it_opthash->second.m_optprm_hash.end() )
		{
			it_opthash->second.m_it = it_opthash->second.m_optprm_hash.begin();
		}
	}
}
// オプションハッシュテーブルの取得（逐次取得型）
// サイクリックにまわす
void CommandLine::next_hash( std::string optkey )
{
	_T_OptHashType::iterator it_opthash;
	if ( (it_opthash = this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		if ( it_opthash->second.m_it != it_opthash->second.m_optprm_hash.end() )
		{
			it_opthash->second.m_it++;
		}
		else
		{
			it_opthash->second.m_it = it_opthash->second.m_optprm_hash.begin();
		}
	}
}
// オプションハッシュテーブルのprmkeyを取得
std::string CommandLine::param_key( std::string optkey )
{
	std::string	retv = "";
	_T_OptHashType::iterator it_opthash;
	if ( (it_opthash = this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		if ( it_opthash->second.m_it != it_opthash->second.m_optprm_hash.end() )
		{
			retv = it_opthash->second.m_it->first;
		}
	}
	return retv;
}
// オプションハッシュテーブルのprmvalueを取得
std::string CommandLine::hvalue( std::string optkey )
{
	std::string	retv = "";
	_T_OptHashType::iterator it_opthash;
	if ( (it_opthash = this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		if ( it_opthash->second.m_it != it_opthash->second.m_optprm_hash.end() )
		{
			retv = it_opthash->second.m_it->second;
		}
	}
	return retv;
}
// オプションハッシュテーブルの個数を取得
// return >= 0 見つかった個数
ptrdiff_t CommandLine::count_hash( std::string optkey ) const
{
	ptrdiff_t retv;

	_T_OptHashType::const_iterator it_opthash;
	if ( (it_opthash = this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		retv = it_opthash->second.m_optprm_hash.size();
	}
	else
	{
		retv = 0;
	}
	return retv;
}
// オプションハッシュテーブルの個数を取得（パラメータキー指定）
// return >= 0 見つかった個数
ptrdiff_t CommandLine::count_hash( std::string optkey, std::string prmkey )
{
	ptrdiff_t retv;

	_T_OptHashType::iterator it_opthash;
	if ( (it_opthash = this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		retv = std::count_if( it_opthash->second.m_optprm_hash.begin(), it_opthash->second.m_optprm_hash.end(),
							Comp_Prm<_T_p_OptParamHashType>(prmkey));
	}
	else
	{
		retv = 0;
	}
	return retv;
}
// 起動オプション文字を検索
bool CommandLine::is(std::string optstr)
{
    bool retb = false;
    std::string search_opt = this->getSep() + optstr;
    for (auto& var : this->m_agv)
    {
        if (var == search_opt)
        {
            retb = true;
            break;
        }
    }
    return retb;
}
