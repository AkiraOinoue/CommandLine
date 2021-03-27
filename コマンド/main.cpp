/*
	update:2014.8.9
	created:2012.8.4
*/
#include <iostream>
/////////////////////////////////////////
/* �R�}���h���C���w�b�_ */
/////////////////////////////////////////
#include "CommandLine.h"
int main( const int agc, const char** agv )
{
    /////////////////////////////////////////
    // CommandLine�̖��O��Ԃ��w��
    /////////////////////////////////////////
    using namespace locallib;
    /////////////////////////////////////////
    // �R�}���h���C���I�u�W�F�N�g�𐶐��i�K�{�j
    /////////////////////////////////////////
    CommandLine     cmd(agc, agv);
    /////////////////////////////////////////
    // �R�}���h�w���v���o�^�p�I�u�W�F�N�g�i�C�Ӂj
    /////////////////////////////////////////
    HelpDesc<>  help(cmd);
    /////////////////////////////////////////
    // ��O�������擾�ł���悤�ɂ���B
    // try { } catch () { }
    /////////////////////////////////////////
    try
    {
        /////////////////////////////////////////
        // �I�v�V�����p�����[�^�̃Z�p���[�^��ݒ�
        // --param=xxx=yyy=... �ȂǂɑΉ�
        // �f�t�H���g�F���ݒ� cmd.setoptprm_sep("")
        /////////////////////////////////////////
        cmd.setoptprm_sep("=");
        /////////////////////////////////////////
        // �I�v�V�����Z�p���[�^�ݒ�
        /////////////////////////////////////////
        cmd.setSep("--");
        /////////////////////////////////////////
        // �R�}���h�I�v�V������`
        // $1=const char* key,   // �I�v�V�����L�[
        // $2=const char* desc,  // �I�v�V�����̐���
        // $3=const int opt_need_flg, // �C��(�f�t�H���g=_FREEP)
        // $4=const int param_flg, // �C��(�f�t�H���g=_FREEP)
        // $5=const char* default_opt // �f�t�H���g�l
        // �C�ӂ̒l�F
        // #define _NONEP  �I�v�V�����p�����[�^�Ȃ�
        // #define _FREEP  �C�ӃI�v�V�����p�����[�^(�f�t�H���g)
        // #define _NEEDP  �K�{�I�v�V�����p�����[�^
        // #define _ULMTP  �����I�v�V�����p�����[�^
        /////////////////////////////////////////
        cmd
            ("vnum","���q(���l)", _FREEP, _FREEP, "1.0")
            ("vden","����(���l)", _FREEP, _FREEP, "2.0")
            ("need","�K�{�I�v�V����",_NEEDP)
            ("param", "�p�����[�^File���w��", _FREEP, _FREEP, "param.xml")
            ("multi","�����I�v�V����", _FREEP, _FREEP|_ULMTP, "sample")
            ("type", "�����I�v�V����", _FREEP, _FREEP | _ULMTP, "Japanese")
            ("single","�P���I�v�V����", _FREEP, _FREEP, "simple")
            ("none","�p�����[�^�Ȃ�", _FREEP, _NONEP)
            ("help","�I�v�V��������", _FREEP, _FREEP|_ULMTP);
        /////////////////////////////////////////
        // �R�}���h�w���v���o�^
        /////////////////////////////////////////
        help
            ("vnum","@vnum ���q(���l)")
            ("vden","@vden ����(���l)")
			("need","@need �C�Ӄp�����[�^�̕K�{�I�v�V����")
            ("param","@param �p�����[�^File.xml (�f�t�H���g�Fparam.xml)")
            ("multi","@multi �����I�v�V�����p�����[�^ ...")
            ("type", "@type �����I�v�V�����p�����[�^ ...")
            ("single","@single �P���I�v�V�����p�����[�^")
            ("none","@none �p�����[�^�Ȃ�")
            ("help","@help [command_name ...]");
        /////////////////////////////////////////
        // �R�}���h���C�������擾
        /////////////////////////////////////////
        // �p�����[�^�t�@�C�����w�肳��Ă�����K�{�I�v�V�����`�F�b�N�͖����ɂ���B
        bool ChkNeedFlg = true;
        if (cmd.is("param") == true )
        {
            ChkNeedFlg = false;
        }
        cmd.options(ChkNeedFlg);
        //***************************************
        /* prg /param�I�v�V�����̏��� */
        //***************************************
        if ( cmd.str("param") )
        {
            // �p�����[�^�t�@�C�����ȗ����̓f�t�H���g�t�@�C�������Z�b�g�����
            cmd.options( cmd.param() );
            // �w��I�v�V�����o��
            CommandLine::_T_ArgumentType    obj;
            obj = cmd.getm_agv();
            std::copy(obj.begin(), obj.end(), std::ostream_iterator<std::string>(std::cout, " "));
            std::cout << std::endl;
            // �ȍ~�̃I�v�V���������̓R�}���h���C���Ɠ���
        }
        //***************************************
        /* prg aaa bbb ... ����`�I�v�V�����̏��� */
        //***************************************
        if (cmd.str(UDP))
        {
            cmd.start();
            while ( cmd.get(UDP) )
            {
                std::cout << "Undef Option: " << cmd.param() << std::endl;
            }
        }
        //***************************************
        /* prg /need �K�{�I�v�V�����̏��� */
        //***************************************
        if (cmd.str("need"))
        {
            std::cout << "need: ";
            if ( cmd.is_param("need") )
            {
            	std::cout << cmd.param();
            }
			std::cout << std::endl;
        }
        //***************************************
        /* prg /�}���`�I�v�V�����̏��� */
        //***************************************
        const char* Opts[] = {
            "multi",
            "type"
        };
        for (auto& var : Opts)
        {
            if (cmd.str(var))
            {
                /////////////////////////////////////////
                // �����̃I�v�V�����p�����[�^���擾
                /////////////////////////////////////////
                std::cout << var << "{" << std::endl;
                // �ʏ�̃p�����[�^��\��
                cmd.start();
                cmd.start_hash(var);
                while (cmd.get(var))
                {
                    /////////////////////////////////////////
                    // �}���`�I�v�V�������o��
                    /////////////////////////////////////////
                    std::cout << " " << cmd.param();
                    // �n�b�V���e�[�u����\��
                    // --multi env=xxxx os=Windows office=2003=2007=2010 ... �Ȃǂ�\��
                    if (cmd.is_hash(var, cmd.param()))
                    {
                        if (cmd.param() == "size")
                        {
                            double sized = cmd.hvalue<double>(var);
                            std::cout << " double= " << sized << " ";
                        }
                        else
                        {
                            std::cout << " = " << cmd.hvalue(var) << " ";
                        }
                        // next data setting
                        cmd.next_hash(var);
                    }
                    std::cout << std::endl;
                }
                std::cout << var << " hash = " << cmd.count_hash(var) << std::endl;
                std::cout << "}" << std::endl;
            }
        }
        //***************************************
        /* prg /single�I�v�V�����̏��� */
        //***************************************
        if ( cmd.str("single") )
        {
            std::cout << "single " << cmd.param();
            cmd.start_hash("single");
            if ( cmd.is_hash("single", cmd.param()) )
            {
                if( cmd.param() == "size" )
                {
                    double sized = cmd.hvalue<double>("single");
                    std::cout << " double= " << sized << " ";
                }
                else
                {
                    std::cout << " = " << cmd.hvalue("single") << " ";
                }
            }
            std::cout << std::endl;
        }
        //***************************************
        /* prg /none�I�v�V�����̏��� */
        //***************************************
        if ( cmd.str("none") )
        {
            std::cout << "none���w�肳��܂����B" << std::endl;
        }
        //***************************************
        /* prg /help�I�v�V�����̏��� */
        //***************************************
        if ( cmd.str("help") )
        {
            if ( !cmd.is_param("help"))
            {
                /////////////////////////////////////////
                // usage�o��
                /////////////////////////////////////////
                cmd.usage();
            }
            else
            {
                /////////////////////////////////////////
                // �����̃I�v�V�����p�����[�^���擾
                /////////////////////////////////////////
                cmd.start();
                while(cmd.get("help"))
                {
                    std::cout << "help:" << std::endl;
                    /////////////////////////////////////////
                    // �R�}���h�̃w���v���o��
                    /////////////////////////////////////////
                    cmd.cmd_help( cmd.param() );
                }
            }
        }
        //***************************************
        /* prg /vnum xxx  /vden xxx �I�v�V�����̏��� */
        //***************************************
        double ln,ld;
        if ( cmd.str("vnum") )
        {
            // �I�v�V�����p�����[�^���ȗ����ꂽ�ꍇ�͎����I�ɏȗ��l���g�p�����
            ln = cmd.param<double>("vnum");
        }
        else
        {
            ln = cmd.def<double>("vnum");
            std::cout << "vnum���ȗ�����Ă��܂��̂ŏȗ��l�F" << ln << "���g�p���܂��B" << std::endl;
        }

        if ( cmd.str("vden") )
        {
            ld = cmd.param<double>("vden");
        }
        else
        {
            ld = cmd.def<double>("vden");
            std::cout << "vden���ȗ�����Ă��܂��̂ŏȗ��l�F" << ld << "���g�p���܂��B" << std::endl;
        }
        if ( cmd.str("vnum") && cmd.str("vden") )
        {
            /////////////////////////////////////////
            // �I�v�V�����Ŏw�肳�ꂽ�l��<double>�^�Ŏ󂯎��
            /////////////////////////////////////////
            std::cout << ln << "/" << ld << " = " << ln/ld << std::endl;
        }
    }
    /////////////////////////////////////////
    // CommandLine����̗�O�������󂯎��
    /////////////////////////////////////////
    catch ( const std::exception& e )
    {
        /////////////////////////////////////////
        // ��O�����̏����o��
        /////////////////////////////////////////
        std::cerr << e.what() << std::endl;
        /////////////////////////////////////////
        // �I�v�V�����̎g�p���@���o��
        /////////////////////////////////////////
        cmd.usage();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
