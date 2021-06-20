// WinApp.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "CommandLine.h"
#include "WinApp.h"

void MsgBox(const char* msg)
{
    ::MessageBox(
        nullptr,
        msg,
        "コマンドラインパラメータ",
        MB_ICONINFORMATION
    );
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // コマンドラインオブジェクト生成
    /////////////////////////////////////////
    // CommandLineの名前空間を指定
    /////////////////////////////////////////
    using namespace locallib;
    /////////////////////////////////////////
    // コマンドラインオブジェクトを生成（必須）
    /////////////////////////////////////////
    CommandLine     cmd(lpCmdLine);
    /////////////////////////////////////////
    // コマンドヘルプ情報登録用オブジェクト（任意）
    /////////////////////////////////////////
    HelpDesc<>  help(cmd);
    /////////////////////////////////////////
    // 例外処理を取得できるようにする。
    // try { } catch () { }
    /////////////////////////////////////////
    std::string strtbl = "";
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
        ("vnum", "分子(数値)", _FREEP, _FREEP, "1.0")
            ("vden", "分母(数値)", _FREEP, _FREEP, "2.0")
            ("need", "必須オプション", _NEEDP)
            ("param", "パラメータFileを指定", _FREEP, _FREEP, "param.xml")
            ("multi", "複数オプション", _FREEP, _FREEP | _ULMTP, "sample")
            ("type", "複数オプション", _FREEP, _FREEP | _ULMTP, "Japanese")
            ("single", "単数オプション", _FREEP, _FREEP, "simple")
            ("none", "パラメータなし", _FREEP, _NONEP)
            ("help", "オプション説明", _FREEP, _FREEP | _ULMTP);
        /////////////////////////////////////////
        // コマンドヘルプ情報登録
        /////////////////////////////////////////
        help
        ("vnum", "@vnum 分子(数値)")
            ("vden", "@vden 分母(数値)")
            ("need", "@need 任意パラメータの必須オプション")
            ("param", "@param パラメータFile.xml (デフォルト：param.xml)")
            ("multi", "@multi 複数オプションパラメータ ...")
            ("type", "@type 複数オプションパラメータ ...")
            ("single", "@single 単数オプションパラメータ")
            ("none", "@none パラメータなし")
            ("help", "@help [command_name ...]");
        /////////////////////////////////////////
        // コマンドライン情報を取得
        /////////////////////////////////////////
        // パラメータファイルが指定されていたら必須オプションチェックは無効にする。
        bool ChkNeedFlg = true;
        if (cmd.is("param") == true)
        {
            ChkNeedFlg = false;
        }
        cmd.options(ChkNeedFlg);
        //***************************************
        /* prg /paramオプションの処理 */
        //***************************************
        if (cmd.str("param"))
        {
            // パラメータファイル名省略時はデフォルトファイル名がセットされる
            cmd.options(cmd.param());
            // 指定オプション出力
            CommandLine::_T_ArgumentType    obj;
            obj = cmd.getm_agv();
            strtbl = "paramオプション: ";
            for (const auto& var : obj)
            {
                strtbl += var + " ";
            }
            ::MsgBox(strtbl.c_str());
            // 以降のオプション処理はコマンドラインと同じ
        }
        //***************************************
        /* prg aaa bbb ... 未定義オプションの処理 */
        //***************************************
        if (cmd.str(UDP))
        {
            cmd.start();
            strtbl = "Undef Option: ";
            while (cmd.get(UDP))
            {
                strtbl += cmd.param() + " ";
            }
            ::MsgBox(strtbl.c_str());
        }
        //***************************************
        /* prg /need 必須オプションの処理 */
        //***************************************
        if (cmd.str("need"))
        {
            strtbl = "needオプション: ";
            if (cmd.is_param("need"))
            {
                strtbl += cmd.param();
                ::MsgBox(strtbl.c_str());
            }
        }
        //***************************************
        /* prg /マルチオプションの処理 */
        //***************************************
        const char* Opts[] = {
            "multi",
            "type"
        };
        for (const auto& var : Opts)
        {
            if (cmd.str(var))
            {
                strtbl = "マルチオプション：";
                strtbl += var + std::string(" ");
                /////////////////////////////////////////
                // 複数のオプションパラメータを取得
                /////////////////////////////////////////
                strtbl += "{ ";
                // 通常のパラメータを表示
                cmd.start();
                cmd.start_hash(var);
                while (cmd.get(var))
                {
                    /////////////////////////////////////////
                    // マルチオプションを出力
                    /////////////////////////////////////////
                    strtbl += " " + cmd.param() + " ";
                    // ハッシュテーブルを表示
                    // --multi env=xxxx os=Windows office=2003=2007=2010 ... などを表示
                    if (cmd.is_hash(var, cmd.param()))
                    {
                        if (cmd.param() == "size")
                        {
                            double sized = cmd.hvalue<double>(var);
                            strtbl += " double= " + std::to_string(sized) + ", ";
                        }
                        else
                        {
                            strtbl += " = " + cmd.hvalue(var) + ", ";
                        }
                        // next data setting
                        cmd.next_hash(var);
                    }
                }
                strtbl += "}";
                strtbl += var + std::string(" hash = ") + std::to_string(cmd.count_hash(var));
                ::MsgBox(strtbl.c_str());
            }
        }
        //***************************************
        /* prg /singleオプションの処理 */
        //***************************************
        if (cmd.str("single"))
        {
            strtbl = "singleオプション：";
            cmd.start_hash("single");
            if (cmd.is_hash("single", cmd.param()))
            {
                if (cmd.param() == "size")
                {
                    double sized = cmd.hvalue<double>("single");
                    strtbl += " double= " + std::to_string(sized) + " ";
                }
                else
                {
                    strtbl += " = " + cmd.hvalue("single");
                }
            }
            ::MsgBox(strtbl.c_str());
        }
        //***************************************
        /* prg /noneオプションの処理 */
        //***************************************
        if (cmd.str("none"))
        {
            strtbl = "noneが指定されました。";
            ::MsgBox(strtbl.c_str());
        }
        //***************************************
        /* prg /helpオプションの処理 */
        //***************************************
        if (cmd.str("help"))
        {
            if (!cmd.is_param("help"))
            {
                /////////////////////////////////////////
                // usage出力（ダイアログ表示）
                /////////////////////////////////////////
                cmd.usageDlg();
            }
            else
            {
                /////////////////////////////////////////
                // 複数のオプションパラメータを取得
                /////////////////////////////////////////
                cmd.start();
                while (cmd.get("help"))
                {
                    /////////////////////////////////////////
                    // コマンドのヘルプを出力
                    /////////////////////////////////////////
                    cmd.cmd_helpDlg(cmd.param());
                }
            }
        }
        //***************************************
        /* prg /vnum xxx  /vden xxx オプションの処理 */
        //***************************************
        double ln, ld;
        if (cmd.str("vnum"))
        {
            strtbl = "vnumオプション：";
            // オプションパラメータが省略された場合は自動的に省略値が使用される
            ln = cmd.param<double>("vnum");
            strtbl += std::to_string(ln);
            ::MsgBox(strtbl.c_str());
        }
        else
        {
            strtbl = "vnumが未指定";
            ln = cmd.def<double>("vnum");
            strtbl += "vnumが省略されていますので省略値：" + std::to_string(ln) + "を使用します。";
            ::MsgBox(strtbl.c_str());
        }

        if (cmd.str("vden"))
        {
            ld = cmd.param<double>("vden");
            strtbl = "vdenオプション：";
            strtbl += std::to_string(ld);
        }
        else
        {
            strtbl = "vdenが未指定";
            ld = cmd.def<double>("vden");
            strtbl += "vdenが省略されていますので省略値：" + std::to_string(ld) + "を使用します。";
        }
        if (cmd.str("vnum") && cmd.str("vden"))
        {
            /////////////////////////////////////////
            // オプションで指定された値を<double>型で受け取る
            /////////////////////////////////////////
            strtbl = "vnum / vden：";
            strtbl += std::to_string(ln) + "/" + std::to_string(ld) + " = " + std::to_string(ln / ld);
            ::MsgBox(strtbl.c_str());
        }
    }
    /////////////////////////////////////////
    // CommandLineからの例外処理を受け取る
    /////////////////////////////////////////
    catch (const std::exception& e)
    {
        /////////////////////////////////////////
        // 例外発生の情報を出力
        /////////////////////////////////////////
        ::MessageBox(
            nullptr,
            e.what(),
            "異常発生",
            MB_ICONERROR
        );
        //std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
