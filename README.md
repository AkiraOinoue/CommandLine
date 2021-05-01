# CommandLine Class Library</br>
## Command Line Processor</br>
## &lt;&lt;&lt;CommandLineの概要>>></br>
## ■c++のプログラムで容易にコマンドラインの情報を取得する事ができます。</br>
### ◆特徴
○必須オプションと任意オプションを自動チェック
○指定オプションの値を型指定して取得可能
```
 例）int i = cmd.param<int>("opt");
```
○オプションパラメータファイルに対応
```
 コマンドラインの限界を超えるような長いオプション指定にも
 簡単に対応可能。
 cmd.options(パラメータファイルパス);
 xml形式のファイルにオプションやパラメータを記述し、解析が可能
 以下はフォーマット。<Command>～</Command>で挟む。
 optタグのnameにオプション名を指定。
 オプションパラメータは任意のタグ<file>値</file>などで指定可能。
 コメントとして<!-- ～ -->が使用できます。
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
