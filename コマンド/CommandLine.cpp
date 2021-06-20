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
/// �R�}���h���C���R���X�g���N�^
/// �i�R���\�[���v���O�����p�j
/// </summary>
/// <param name="agc">�p�����[�^��</param>
/// <param name="agv">�p�����[�^�e�[�u��</param>
/// <param name="valid_flg">�I�v�V�����Ȃ��̋��t���O</param>
/// <param name="hlp_opt">�w���v�I�v�V����������</param>
/// <returns>�Ȃ�</returns>
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
/// �R�}���h���C���R���X�g���N�^
/// �iWindows�v���O�����p�j
/// </summary>
/// <param name="lpCmdLine">�p�����[�^�e�[�u��</param>
/// <param name="valid_flg">�I�v�V�����Ȃ��̋��t���O</param>
/// <param name="hlp_opt">�w���v�I�v�V����������</param>
/// <returns>�Ȃ�</returns>
CommandLine::CommandLine(
    LPWSTR    lpCmdLine,
    bool valid_flg,
    std::string hlp_opt
) throw()
{
    this->m_idx = 0;
    this->m_sep = OPTSEP;
    this->m_agc = 0;
    // lpCmdLine��char *agv[]�`���ɕϊ�
    // Windows�A�v���p�R�}���h���C���ݒ�
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
/// Usage�_�C�A���O�\��
/// </summary>
/// <param name="">�Ȃ�</param>
void CommandLine::usageDlg(void)
{
    vector<_T_p_OptionType> mdisp(this->m_option.size());
    // �I�v�V�����̃R�s�[
    copy(
        this->m_option.begin(),
        this->m_option.end(),
        mdisp.begin()
    );
    // �I�v�V������m_idx�Ń\�[�g
    stable_sort(
        mdisp.begin(),
        mdisp.end(),
        Cmp_mdisp<_T_p_OptionType>()
    );
    // �I�v�V��������(�K�{/�C��): �����i�K�{/�C�Ӂj�@- �Ӗ�
    std::string usage_str = "";
    disp_usage_msg <_T_p_OptionType>  disp_msg(
        this->m_prgname.c_str(),
        this->getSep(),
        &usage_str
    );
    for_each(
        mdisp.begin(),
        mdisp.end(),
        // disp_msg��mdisp�̗v�f�������ɌJ��Ԃ��Ă΂��
        disp_msg
    );
    // �g�p���@���_�C�A���O�\��
    ::MessageBox(
        nullptr,
        usage_str.c_str(),
        "�g�p���@",
        MB_ICONINFORMATION
    );
}
// Usage�\��
void CommandLine::usage( const char* cmd )
{
    vector<_T_p_OptionType> mdisp(this->m_option.size());
    // �I�v�V�����̃R�s�[
    copy(
        this->m_option.begin(), 
        this->m_option.end(), 
        mdisp.begin()
    );
    // �I�v�V������m_idx�Ń\�[�g
    stable_sort(
        mdisp.begin(), 
        mdisp.end(), 
        Cmp_mdisp<_T_p_OptionType>()
    );
    // �I�v�V��������(�K�{/�C��): �����i�K�{/�C�Ӂj�@- �Ӗ�
    for_each(
        mdisp.begin(), 
        mdisp.end(), 
        // disp_usage��mdisp�̗v�f�������ɌJ��Ԃ��Ă΂��
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
// �R�}���h���C��������I���W�i���f�[�^�ۑ�
void CommandLine::setAgv(const int agc, const char* agv[])
{
    // �R���e�i��agv[]�̒l���R�s�[
    for(int ii = 0; ii < agc; ii++)
    {
        this->m_agv.push_back(agv[ii]);
    }
}
/////////////////////////////////////////////////////////
/// <summary>
/// �R�}���h�p�����[�^�ϊ�
/// Windows�v���O�����p
/// </summary>
/// <param name="lpCmdLine">�R�}���h�p�����[�^</param>
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
        // �p�����[�^�J�E���g
        this->m_agc = nArgs;
        // �p�����[�^�e�[�u���Ɋi�[
        for (int i = 0; i < nArgs; i++)
        {
            // W����A�ɕϊ�
            CString wstr = szArglist[i];
            this->m_agv.push_back(wstr.GetString());
        }
        // �v���O������
        this->m_prgname = this->m_agv[0];
    }
    // Free memory allocated for CommandLineToArgvW arguments.
    LocalFree(szArglist);
}
// �R�}���h�w���v���o�^
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
/// �R�}���h�w���v���o�́i�_�C�A���O�\���j
/// </summary>
/// <param name="key">�I�v�V����������</param>
/// <param name="help_str">�w���v�\��������</param>
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
// �R�}���h�w���v���o��
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
/// �R�}���h�w���v�i�_�C�A���O�\���j
/// </summary>
/// <param name="key">�I�v�V����������</param>
void CommandLine::cmd_helpDlg(const std::string key)
{
    std::string help_str = "";
    CommandLine::_T_OptionInfo retv;
    string keystr = key;
    if (this->search(key.c_str()))
    {
        // �Ή�����I�v�V����������Ώ��擾
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
        "�R�}���h�I�v�V�����w���v",
        MB_ICONINFORMATION
    );
}
// �R�}���h�w���v
void CommandLine::cmd_help(const std::string key)
{
    CommandLine::_T_OptionInfo retv;
    string keystr = key;
    if ( this->search(key.c_str()) )
    {
        // �Ή�����I�v�V����������Ώ��擾
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
// �I�v�V�������o�^
void CommandLine::setOption(
        const char* key,
        const char* desc,  // �I�v�V�����̐���
        const int opt_need_flg, // �C��
        const int param_flg, // �C��
        const char* default_opt // �f�t�H���g�l
        )
{
    _T_OptionInfo tmpcont;
    if (*key == (char)NULL)
    {
        string err = desc;
        err += ",";
        err += default_opt;
        err += " :�I�v�V�����L�[�����w��ł��B";
        throw CmdLineException(err.c_str());
    }
    else if (opt_need_flg & param_flg & _ULMTP)
    {
        string err = key;
        err += ": ";
        err += desc;
        err += ": ";
        err += "�I�v�V���������ƃp�����[�^�̗����Ƃ������w��͂ł��܂���B\n";
        err += "�Y������L�[��CommandLine::setOption(...)�ōēx�ݒ肵�ĉ������B";
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
// �I�v�V�����L�[����
bool CommandLine::search( const char* key )
{
    // �O����v�ŃL�[������
    typedef SearchKey< _T_p_OptionType >  _T_SrchType;
    _T_SrchType   res =
        for_each(this->m_option.begin(), this->m_option.end(), _T_SrchType(key));
    // ������v����ꍇ�́A�G���[�Ƃ���
    return res.m_count == 1;
}
// �I�v�V�������擾
CommandLine::_T_OptionInfo CommandLine::getOption( const char* key )
{
    // �O����v�ŃL�[������
    typedef SearchKey< _T_p_OptionType >  _T_SrchType;
    _T_SrchType   res =
        for_each(this->m_option.begin(), this->m_option.end(), _T_SrchType(key));
    return res.optinfo;
}
// �I�v�V���������w��q�ݒ�
void CommandLine::setSep( const char* sp )
{
    this->m_sep = sp;
}
// �I�v�V���������w��q
string CommandLine::getSep( void ) const
{
    return this->m_sep;
}
// �w��̗v�f���I�v�V�����������ǂ����𔻒�
bool CommandLine::IsOptStr(const int idx) const
{
    bool retv;
    if(this->m_agc > idx)
    {
        string optparam = this->m_agv[idx];
        string sepstr = this->getSep();
        // �擪�̕����ƕ�����r
        retv = (optparam.compare(0, sepstr.size(), sepstr) == 0);
    }
    else
    {
        retv = false;
    }
    return retv;
}
// �w��̗v�f���I�v�V�����p�����[�^�������ǂ����𔻒�
bool CommandLine::IsOptParam(const int idx) const
{
    bool retv;
    if(this->m_agc > idx)
    {
        string optparam = this->m_agv[idx];
        string sepstr = this->getSep();
        // �擪�̕����ƕ�����r
        retv = optparam.compare(0, sepstr.size(), sepstr) != 0;
    }
    else
    {
        retv = false;
    }
    return retv;
}
// �R�}���h�e�[�u���폜
void CommandLine::clear()
{
    this->m_cmdline.clear();
    this->m_agv.clear();
}
// �R�}���h�p�����[�^�t�@�C������荞�� by "ParamReader.h"
#if USED_BOOSTLIB
void CommandLine::setm_agv(ptree_type::const_iterator pos)
{
    const ptree_type& node =  pos->second;

    // 1. �^�O���̎擾
    this->set_tag(pos->first);

    // 2. �����̎擾
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
        // 3. �e�L�X�g�C�܂��͎q�m�[�h�̏o�́D
		// �R�����g�ȊO���擾
        for (ptree_type::const_iterator child = node.begin(); child != node.end(); ++child) {
            if (child->first == "<xmlattr>" || child->first == "<xmlcomment>")
            {
                continue;
            }
            this->setm_agv(child);
        }
    }
}
// �R�}���h�p�����[�^�t�@�C���̑������擾
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
// Windows�l�C�e�B�u��
void CommandLine::setm_agv(stdxml::XMLRead* pos)
{
    // �I�v�V����������ɓW�J
    for (auto& var : this->lpxml->get_xml())
    {
        if (var.ntype == "XmlOption")
        {
            // �I�v�V�����w�蕶����ݒ�
            this->set_attr_value(this->getSep() + var.value.GetString());
        }
        else
        {
            // �I�v�V�����l������ݒ�
            this->set_attr_value(var.value.GetString());
        }
        this->m_agv.push_back(this->get_attr_value());
    }
}
#endif
// �R�}���h�p�����[�^�t�@�C���������
void CommandLine::options( const string param_file )
{
    // �R�}���h���C�����N���A
    this->clear();
    // �v���O�������ݒ�
    this->m_agv.push_back(this->m_prgname);
    // �I�v�V�����ݒ�
#if USED_BOOSTLIB
    boost::property_tree::ptree root;
    boost::property_tree::read_xml(param_file, root);
    // �I�v�V���������ƒl��ݒ�
    this->setm_agv(root.begin());
#else
    // �p�����[�^�t�@�C���p�X��ݒ肷��
    this->lpxml->SetFilePath(param_file.c_str());
    // XML���
    if (this->lpxml->GenXML() == FALSE)
    {
        return;
    }
    this->lpxml->XMLReader();
    // �I�v�V���������ƒl��ݒ�
    this->setm_agv(this->lpxml);
#endif // USED_BOOSTLIB
    this->m_agc = this->m_agv.size();
    // �R�}���h���C���̃I�v�V���������擾
    this->options();
}
/// <summary>
/// �R�}���h���C���������
/// </summary>
/// <param name="ChkNeedFlg">
/// true:�K�{�I�v�V�����`�F�b�N�A
/// false:�K�{�I�v�V�����`�F�b�N�Ȃ�
/// �i�f�t�H���g�F�`�F�b�N����j
/// </param>
void CommandLine::options(bool ChkNeedFlg)
{
    // �R�}���h���C���J�E���g��
    string optstr;
    string optparam;
    string sepstr = this->getSep();
    int agc = this->m_agv.size();
    // �I�v�V�����Z�p���[�^�������ݒ�
    size_t sep_length = this->getSep().size();
    for( int ii = 1; ii < agc; ii++ )
    {
        optstr = this->m_agv[ii];
        // �I�v�V�����w��q�����邩
        if (this->IsOptStr(ii))
        {
            if (optstr.size() > sep_length)
            {
                optstr = optstr.substr(sep_length, string::npos);
            }
            else
            {
            	// �I�v�V�����Z�p���[�^�݂̂̏ꍇ��
            	// �����I�v�V�����Ƃ̋�؂�Ƃ��Ĉ���
				continue;
            }
        }
        else
        {
            // �I�v�V���������ȊO���w�肳��Ă���B
            string optkey = UDP;
            optparam = this->m_agv[ii];
            this->m_cmdline.insert(_T_p_CmdLineType(optkey, optparam));
            continue;
        }
        // �o�^�ς݃I�v�V���������������@- �p�����[�^���擾���邽��
        if ( this->search( this->get_OptWithSep(optstr, this->getoptprm_sep()).c_str()) )
        {
            // �Ή�����I�v�V����������Γo�^
            CommandLine::_T_OptionInfo retv =
				this->getOption(this->get_OptWithSep(optstr, this->getoptprm_sep()).c_str());
            // �I�v�V������o�^
            // �P��I�v�V�����̃`�F�b�N
            if( (retv.opt_need_flg & _unlimit_param) == 0 )
            {
                // �R�}���h���C��this->m_cmdline�ɓo�^���ꂽ�I�v�V����������
                if(this->str(retv.optstr))
                {
                    // error
                    string err = sepstr + retv.optstr + "�͕����w��ł��܂���B";
                    throw CmdLineException(err.c_str());
                }
            }
            //////////////////////////////////////////////////////
            // �I�v�V�����p�����[�^�̎w��`�F�b�N�J�n			//
            //////////////////////////////////////////////////////
            // �p�����[�^�͒P��w��ŕK�{
            if ( retv.param_flg == _NEEDP )
            {
            	// �Z�p���[�^�t���I�v�V�����p�����[�^���擾 --param=xxx=yyy=... �ȂǂɑΉ�
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

                // ���̕������I�v�V�����p�����[�^����������
                if (this->IsOptParam(ii+1))
                {
                    optparam = this->m_agv[ii+1];
                    // �I�v�V�����p�����[�^�p�n�b�V���f�[�^�o�^
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
						// �ʏ�I�v�V�����p�����[�^�o�^
						else
						{
							this->set_optprmhash( retv.optstr, prmkey, "" );
							this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
						}
                    }
                    // �ʏ�I�v�V�����p�����[�^�o�^
                    else
                    {
						this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, optparam));
                    }
                    ii++;
                }
                else
                {
                    // error
                    string err = sepstr + retv.optstr + ":�I�v�V�����p�����[�^���w�肵�Ă��������B";
                    throw CmdLineException(err.c_str());
                }
            }
            //////////////////////////////////////////////////////
            // �I�v�V�����p�����[�^�̎w��`�F�b�N�J�n			//
            //////////////////////////////////////////////////////
            // �p�����[�^�͖������i�O�ȏ�j
            else if (retv.param_flg & _ULMTP)
            {
				// �Z�p���[�^�t���I�v�V�����p�����[�^���擾 --param=xxx=yyy=... �ȂǂɑΉ�
            	if ( this->getoptprm_sep().size() > 0 &&
					this->count_OptPrmWithSep( optstr, this->getoptprm_sep()) > 0 )
				{
					_T_OptPrmList	optprm_list =
						this->get_OptPrmWithSep( optstr, this->getoptprm_sep() );
					_T_OptPrmList::iterator	It = optprm_list.begin();
					// �I�v�V�����p�����[�^������ꍇ
					if ( optprm_list.size() > 0 )
					{
                        for ( ;It != optprm_list.end(); It++ )
                        {
                            string l_optparam = *It;
                            this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, l_optparam));
                        }
					}
                    // �I�v�V�����p�����[�^���Ȃ�
                    // �I�v�V�����p�����[�^���K�{�̏ꍇ
                    else if (retv.param_flg & _NEEDP)
                    {
                        string err = sepstr + retv.optstr + ":�I�v�V�����p�����[�^���w�肵�Ă��������B";
                        throw CmdLineException(err.c_str());
                    }
                    // �I�v�V�����p�����[�^���C�ӂ̏ꍇ
                    else
                    {
                        // �f�t�H���g�p�����[�^��ݒ�
                        this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
                    }
					continue;
				}

                // �I�v�V�����p�����[�^������
                if(this->IsOptParam(ii+1))
                {
					int jj;
					for(jj = ii+1; jj < agc; jj++)
					{
						// �p�����[�^�ł���
						if(this->IsOptParam(jj))
						{
							string l_optparam = this->m_agv[jj];
							// �I�v�V�����p�����[�^�p�n�b�V���f�[�^�o�^
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
								// �ʏ�I�v�V�����p�����[�^�o�^
								if ( prmv_list.size() == 0 )
								{
									this->set_optprmhash( retv.optstr, prmkey, "" );
									this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
								}
							}
							// �ʏ�I�v�V�����p�����[�^�o�^
							else
							{
								this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, l_optparam));
							}
							ii = jj;
						}
						// �I�v�V��������
						else
						{
							break;
						}
					}
                }
                // �I�v�V�����p�����[�^���Ȃ�
                // �I�v�V�����p�����[�^���K�{�̏ꍇ
                else if (retv.param_flg & _NEEDP)
                {
                    string err = sepstr + retv.optstr + ":�I�v�V�����p�����[�^���w�肵�Ă��������B";
                    throw CmdLineException(err.c_str());
                }
                // �I�v�V�����p�����[�^���C�ӂ̏ꍇ
                else
                {
                    // �f�t�H���g�p�����[�^��ݒ�
                    this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
                }
            }
            //////////////////////////////////////////////////////
            // �I�v�V�����p�����[�^�̎w��`�F�b�N�J�n			//
            //////////////////////////////////////////////////////
            // �p�����[�^�͒P��ŔC��
            else if ( retv.param_flg == _FREEP )
            {
            	// �Z�p���[�^�t���I�v�V�����p�����[�^���擾 --param=xxx=yyy=... �ȂǂɑΉ�
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
					    // �f�t�H���g�p�����[�^��ݒ�
					    this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
					}
					continue;
				}

                // �I�v�V�����p�����[�^
                if ( this->IsOptParam(ii+1) )
                {
                    optparam = this->m_agv[ii+1];
                    // �I�v�V�����p�����[�^�p�n�b�V���f�[�^�o�^
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
						// �ʏ�I�v�V�����p�����[�^�o�^
						else
						{
							this->set_optprmhash( retv.optstr, prmkey, "" );
							this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, prmkey));
						}
                    }
                    // �ʏ�I�v�V�����p�����[�^�o�^
					else
					{
						this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, optparam));
					}
                    ii++;
                }
                else
                {
                    // �f�t�H���g�p�����[�^��ݒ�
                    this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, retv.default_opt));
                }
            }
            // �p�����[�^�Ȃ�
            else if ( retv.param_flg == _NONEP )
            {
                optparam = "";
                this->m_cmdline.insert(_T_p_CmdLineType(retv.optstr, optparam));
                continue;
            }
        }
        else
        {
            string err = sepstr + optstr + " :�����ȃI�v�V�������w�肳��܂����B";
            throw CmdLineException(err.c_str());
        }
    }
    // �w���v�I�v�V�������w�肳��Ă�����G���[�Ƃ͂Ȃ�Ȃ��B
    if (!this->str(this->m_help_option))
    {
        // �K�{�I�v�V�����`�F�b�N�t���O��true�̏ꍇ�����`�F�b�N
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
            throw CmdLineException("�L���ȃI�v�V�������w�肳��Ă���܂���B");
        }
    }
}
// �K�{�I�v�V�����̎w��`�F�b�N
bool CommandLine::chk_need_option( void )
{
    // �R�}���h���C���ɕK�{�I�v�V�������w�肳��Ă��邩���`�F�b�N
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
        string err = need_opt + ":�K�{�I�v�V�������w�肳��Ă��܂���B";
        throw CmdLineException(err.c_str());
    }
    else
    {
        retv = true;
    }
    return retv;
}
// �R�}���h���C���擾�̏�����
void CommandLine::open( void )
{
    this->m_Itcmdline = this->m_cmdline.begin();
}
void CommandLine::start( void )
{
    this->m_Itcmdline = this->m_cmdline.begin();
}
// �R�}���h���C���擾
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
// �Y���I�v�V�����̎w�萔��Ԃ�
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
// �I�v�V���������擾�i�����^�j
std::string CommandLine::str( void ) const
{
    return this->m_optstr;
}
// �I�v�V���������擾�i�����^+�����^�j
bool CommandLine::str( std::string vstr )
{
	this->start();
    return this->get(vstr.c_str());
}
// �I�v�V�����p�����[�^�擾
string CommandLine::param( void ) const
{
	return this->m_optparam;
}
string CommandLine::def( const char* key )
{
    string retv;
    _T_OptionInfo	tg;
    // �L�[����
    if( this->search(key) == true)
    {
        tg = this->getOption(key);
        retv = tg.default_opt;
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

// �I�v�V�����p�����[�^�擾
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
// �I�v�V�����p�����[�^�̗L��
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
// �������ǂ����𔻒�
bool CommandLine::is_number( const char* key )
{
    if(this->is_param(key) == false)
    {
        // �Y������L�[���Ȃ�
        string err = key;
        err += ": �Y������L�[������܂���B";
        throw CmdLineException(err.c_str());
    }
    // �p�����[�^�͐������ǂ����𔻕�
    string param_str = this->param(key);
    bool retv = (find_if(param_str.begin(), param_str.end(), IsNumber()) == param_str.end());
    return retv;
}
// �������ǂ����𔻒�i�����^�j
bool CommandLine::is_number(void)
{
    // �p�����[�^�͐������ǂ����𔻕�
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
// �I�v�V�����o�^
CommandLine& CommandLine::operator()(
        const char* key,
        const char* desc,       // �I�v�V��������
        const int opt_need_flg, // �C��
        const int param_flg, // �C��
        const char* default_opt // �f�t�H���g�l
)
{
    this->setOption(key, desc, opt_need_flg, param_flg, default_opt);
    return *this;
}
// �I�v�V�����p�����[�^��؂�
// --param=xxx=yyy=... �ȂǂɑΉ�
void CommandLine::setoptprm_sep(const std::string& sep)
{
    this->optprm_sep = sep;
}
// �I�v�V�����p�����[�^��؂�擾
string CommandLine::getoptprm_sep() const
{
    return this->optprm_sep;
}
// �I�v�V�����p�����[�^��؂�ݒ�
// �Z�p���[�^�t���I�v�V���������擾
string CommandLine::get_OptWithSep( const string& optstr, string sep )
{
	string retv = optstr;

	// �Z�p���[�^���w��
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
// �Z�p���[�^�t���I�v�V�����p�����[�^���擾
size_t CommandLine::count_OptPrmWithSep( string& optstr, string sep )
{
	return (size_t)count_if( optstr.begin(), optstr.end(), CountSep(sep));
}
// �Z�p���[�^�t���I�v�V�����p�����[�^�擾
CommandLine::_T_OptPrmList CommandLine::get_OptPrmWithSep( string& optstr, string sep )
{
	CommandLine::_T_OptPrmList	retv;
	StrReplace	repobj;
	string tmpstr;
	size_t idx = optstr.find( sep, 0 );
	if ( this->count_OptPrmWithSep( optstr, sep ) > 0 )
	{
		// �I�v�V�����p�����[�^�ȍ~���R�s�[
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
	//�����񒆂̃u�����N��0x1F�ɕϊ�
	char cnvstr[2];
	cnvstr[0] = 0x1F;
	cnvstr[1] = static_cast<char>(NULL);
	repobj.replace( tmpstr, " ", cnvstr );
	// �����񒆂�sep���u�����N�ɕϊ�
	repobj.replace( tmpstr, sep, " " );

	size_t length = tmpstr.size();
	// �u�����N�ŋ�؂�ꂽ�I�v�V�����p�����[�^��o�^
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
// �I�v�V�����n�b�V���e�[�u���Ƀp�����[�^�ƒl��o�^
void CommandLine::set_optprmhash( std::string optstr, std::string optprm_key, std::string value )
{
	this->m_opthash[optstr].m_optprm_hash.insert( CommandLine::_T_p_OptParamHashType( optprm_key, value ));
	this->m_opthash[optstr].m_it =	this->m_opthash[optstr].m_optprm_hash.end();
}
// �I�v�V�����n�b�V���e�[�u����prmkey���݃`�F�b�N
bool CommandLine::is_hash( std::string optkey, const std::string prmkey )
{
	bool retv = false;
	_T_OptHashType::iterator it_opthash;
	if ( (it_opthash = this->m_opthash.find( optkey )) != this->m_opthash.end() )
	{
		// ���݈ʒu����T�C�N���b�N�Ɍ���
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
// �I�v�V�����n�b�V���e�[�u���̎擾�J�n
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
// �I�v�V�����n�b�V���e�[�u���̎擾�i�����擾�^�j
// �T�C�N���b�N�ɂ܂킷
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
// �I�v�V�����n�b�V���e�[�u����prmkey���擾
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
// �I�v�V�����n�b�V���e�[�u����prmvalue���擾
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
// �I�v�V�����n�b�V���e�[�u���̌����擾
// return >= 0 ����������
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
// �I�v�V�����n�b�V���e�[�u���̌����擾�i�p�����[�^�L�[�w��j
// return >= 0 ����������
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
// �N���I�v�V��������������
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
