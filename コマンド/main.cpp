/*
	update:2014.8.9
	created:2012.8.4
*/
#include <iostream>
/////////////////////////////////////////
/* コマンドラインヘッダ */
/////////////////////////////////////////
#include "CommandLine.h"
int main( const int agc, const char** agv )
{
    /////////////////////////////////////////
    // CommandLineの名前空間を指定
    /////////////////////////////////////////
    using namespace locallib;
    /////////////////////////////////////////
    // コマンドラインオブジェクトを生成（必須）
    /////////////////////////////////////////
    CommandLine     cmd(agc, agv);
    /////////////////////////////////////////
    // コマンドヘルプ情報登録用オブジェクト（任意）
    /////////////////////////////////////////
    HelpDesc<>  help(cmd);
    /////////////////////////////////////////
    // 例外処理を取得できるようにする。
    // try { } catch () { }
    /////////////////////////////////////////
    try
    {
        /////////////////////////////////////////
        // オプションパラメータのセパレータを設定
        // --param=xxx=yyy=... などに対応
        // デフォルト：未設定 cmd.setoptprm_sep("")
        /////////////////////////////////////////
        cmd.setoptprm_sep("=");
        /////////////////////////////////////////
        // オプションセパレータ設定
        /////////////////////////////////////////
        cmd.setSep("--");
        /////////////////////////////////////////
        // コマンドオプション定義
        // $1=const char* key,   // オプションキー
        // $2=const char* desc,  // オプションの説明
        // $3=const int opt_need_flg, // 任意(デフォルト=_FREEP)
        // $4=const int param_flg, // 任意(デフォルト=_FREEP)
        // $5=const char* default_opt // デフォルト値
        // 任意の値：
        // #define _NONEP  オプションパラメータなし
        // #define _FREEP  任意オプションパラメータ(デフォルト)
        // #define _NEEDP  必須オプションパラメータ
        // #define _ULMTP  複数オプションパラメータ
        /////////////////////////////////////////
        cmd
            ("vnum","分子(数値)", _FREEP, _FREEP, "1.0")
            ("vden","分母(数値)", _FREEP, _FREEP, "2.0")
            ("need","必須オプション",_NEEDP)
            ("param", "パラメータFileを指定", _FREEP, _FREEP, "param.xml")
            ("multi","複数オプション", _FREEP, _FREEP|_ULMTP, "sample")
            ("type", "複数オプション", _FREEP, _FREEP | _ULMTP, "Japanese")
            ("single","単数オプション", _FREEP, _FREEP, "simple")
            ("none","パラメータなし", _FREEP, _NONEP)
            ("help","オプション説明", _FREEP, _FREEP|_ULMTP);
        /////////////////////////////////////////
        // コマンドヘルプ情報登録
        /////////////////////////////////////////
        help
            ("vnum","@vnum 分子(数値)")
            ("vden","@vden 分母(数値)")
			("need","@need 任意パラメータの必須オプション")
            ("param","@param パラメータFile.xml (デフォルト：param.xml)")
            ("multi","@multi 複数オプションパラメータ ...")
            ("type", "@type 複数オプションパラメータ ...")
            ("single","@single 単数オプションパラメータ")
            ("none","@none パラメータなし")
            ("help","@help [command_name ...]");
        /////////////////////////////////////////
        // コマンドライン情報を取得
        /////////////////////////////////////////
        // パラメータファイルが指定されていたら必須オプションチェックは無効にする。
        bool ChkNeedFlg = true;
        if (cmd.is("param") == true )
        {
            ChkNeedFlg = false;
        }
        cmd.options(ChkNeedFlg);
        //***************************************
        /* prg /paramオプションの処理 */
        //***************************************
        if ( cmd.str("param") )
        {
            // パラメータファイル名省略時はデフォルトファイル名がセットされる
            cmd.options( cmd.param() );
            // 指定オプション出力
            CommandLine::_T_ArgumentType    obj;
            obj = cmd.getm_agv();
            std::copy(obj.begin(), obj.end(), std::ostream_iterator<std::string>(std::cout, " "));
            std::cout << std::endl;
            // 以降のオプション処理はコマンドラインと同じ
        }
        //***************************************
        /* prg aaa bbb ... 未定義オプションの処理 */
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
        /* prg /need 必須オプションの処理 */
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
        /* prg /マルチオプションの処理 */
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
                // 複数のオプションパラメータを取得
                /////////////////////////////////////////
                std::cout << var << "{" << std::endl;
                // 通常のパラメータを表示
                cmd.start();
                cmd.start_hash(var);
                while (cmd.get(var))
                {
                    /////////////////////////////////////////
                    // マルチオプションを出力
                    /////////////////////////////////////////
                    std::cout << " " << cmd.param();
                    // ハッシュテーブルを表示
                    // --multi env=xxxx os=Windows office=2003=2007=2010 ... などを表示
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
        /* prg /singleオプションの処理 */
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
        /* prg /noneオプションの処理 */
        //***************************************
        if ( cmd.str("none") )
        {
            std::cout << "noneが指定されました。" << std::endl;
        }
        //***************************************
        /* prg /helpオプションの処理 */
        //***************************************
        if ( cmd.str("help") )
        {
            if ( !cmd.is_param("help"))
            {
                /////////////////////////////////////////
                // usage出力
                /////////////////////////////////////////
                cmd.usage();
            }
            else
            {
                /////////////////////////////////////////
                // 複数のオプションパラメータを取得
                /////////////////////////////////////////
                cmd.start();
                while(cmd.get("help"))
                {
                    std::cout << "help:" << std::endl;
                    /////////////////////////////////////////
                    // コマンドのヘルプを出力
                    /////////////////////////////////////////
                    cmd.cmd_help( cmd.param() );
                }
            }
        }
        //***************************************
        /* prg /vnum xxx  /vden xxx オプションの処理 */
        //***************************************
        double ln,ld;
        if ( cmd.str("vnum") )
        {
            // オプションパラメータが省略された場合は自動的に省略値が使用される
            ln = cmd.param<double>("vnum");
        }
        else
        {
            ln = cmd.def<double>("vnum");
            std::cout << "vnumが省略されていますので省略値：" << ln << "を使用します。" << std::endl;
        }

        if ( cmd.str("vden") )
        {
            ld = cmd.param<double>("vden");
        }
        else
        {
            ld = cmd.def<double>("vden");
            std::cout << "vdenが省略されていますので省略値：" << ld << "を使用します。" << std::endl;
        }
        if ( cmd.str("vnum") && cmd.str("vden") )
        {
            /////////////////////////////////////////
            // オプションで指定された値を<double>型で受け取る
            /////////////////////////////////////////
            std::cout << ln << "/" << ld << " = " << ln/ld << std::endl;
        }
    }
    /////////////////////////////////////////
    // CommandLineからの例外処理を受け取る
    /////////////////////////////////////////
    catch ( const std::exception& e )
    {
        /////////////////////////////////////////
        // 例外発生の情報を出力
        /////////////////////////////////////////
        std::cerr << e.what() << std::endl;
        /////////////////////////////////////////
        // オプションの使用方法を出力
        /////////////////////////////////////////
        cmd.usage();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
