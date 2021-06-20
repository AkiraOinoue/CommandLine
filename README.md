# CommandLine Class Library</br>
## Command Line Processor</br>
## &lt;&lt;&lt;CommandLineの概要>>></br>
## ■c++のプログラムで容易にコマンドラインの情報を取得する事ができます。</br>
### ◆特徴
○必須オプションと任意オプションを自動チェック
○指定オプションの値を型指定して取得可能
```cpp
 例）int i = cmd.param<int>("opt");
```
○オプションパラメータファイルに対応
```
 コマンドラインの限界を超えるような長いオプション指定にも
 簡単に対応可能。
 ```cpp
 cmd.options("パラメータファイルパス");
 ```
 xml形式のファイルにオプションやパラメータを記述し、解析が可能
 以下はフォーマット。<Command>～</Command>で挟む。
 optタグのnameにオプション名を指定。
 オプションパラメータは任意のタグ<file>値</file>などで指定可能。
 コメントとして<!-- ～ -->が使用できます。
 ```xml
	<Command>
		<!-- option-->
		<opt name="copy"/>
			<opt name="from"/>
				<file>france</file>
			<opt name="to"/>
				<file>japan</file>
		<!-- comment -->
		<opt name="type"/>
			<file>Amereican</file>
			<file>Canada</file>
	</Command>
```
	※）Utf-8のエンコードファイルか半角英数文字だけのShift-JISのXMLファイルでないと正しく読めません。
```
○任意オプションの値省略時にデフォルト値を設定
```
 例）--opt で値を省略したときにデフォルト値"."を設定。
 フォルダ指定の省略値にカレントフォルダを指定などが
 可能。
```
○長いオプション名を省略してマッチング
```
 例）option_nameは。。。
	--o or --opti など他のオプションと識別できる最小文字数で
	自動的にマッチング
```
○オプションに複数の値を指定可能
```
 例）--opt aaa bbb ccc ...
     --opt=aaa=bbb=ccc ...
     オプションパラメータ区切りを指定可能です。
     CommandLine::setoptprm_sep("=");
```
○オプションパラメータにハッシュデータを指定できます。
```
    例）--opt os="Windows 7" cpu=coreI7 "office tools"=Word=Excel=Powerpoint
    以下のように解釈できます。
    optのパラメータとして、
    osの値はWindows 7
    cpuの値はcoreI7
    office toolsの値はWordとExcelとPowerpoint
    あらかじめCommandLine::setoptprm_sep("=");で指定する必要があります。
	※）キーワードや値にブランクを含む場合は「""」（Wクォート）で囲む必要があります。
```
○同一オプションを複数指定可能
```
 例）--opt xxx --opt bbb ...
 optのname="オプション名を指定します"
 <タグ名>オプションパラメータを指定します。</タグ名>
 タグ名は任意の名称を指定します。
```
○複数の非オプション文字を取得可能
○オプションに登録した省略値を設定できます。
```
　オプションのパラメータを省略した場合の省略値を指定しておくと
　値を省略した時に省略値が設定されます。
　例）--param （省略値=.\param.xml）のときに
　1) --param
　2) --param defparam.xml
　1)の場合のパラメータ値はparam.xmlになります。
　2)の場合のパラメータ値はdefparam.xmlになります。
```
○オプションの区切り文字列を指定可能
```
 他のOS(Windows以外)への移植がしやすい
 例） --opt or /opt or ::opt など任意の文字列が指定できる。
 該当メソッド：CommandLine::setSep("::");
```
○help表示情報を登録・簡単に参照可能
```
・メッセージ中の"@"をオプション区切り文字に自動変換表示
```
○オプションエラー発生時はstd::exception& eでcatch可能
```
 ・CmdLineException& eで個別にcatchが可能
 e.what()でメッセージ出力
```
## ■コンパイル方法
```
以下のヘッダファイルをインクルードできるようにパスを指定します。
1."CommandLine.h"
2."CmdLineException.h"
3."ParamReader.h"
　（boost未使用の場合はこのファイル内の#define USED_BOOSTLIBをfalseにします（現在の設定：false））
4."Utility.h"
5."boost/property_tree/ptree.hpp"
6."boost/property_tree/xml_parser.hpp""
  boostヘッダの入手は　http:/www.boost.org をご参照下さい。
  ヘッダをインクルードするだけで使用可能。
  （デフォルトではboostのヘッダは不要です。）
```
## ■リンク方法
```
CommandLine.lib(アーカイブファイル)をリンクします。
このライブラリーは上記のソースとヘッダファイルで作成できます。
```
## ■開発環境
Visual Studio 2019 C++/Windows10
## コンソールプログラムのサンプルコード</br>
```c++
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
```
## Windowsデスクトッププログラムのサンプルコード</br>
```cpp
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
```
