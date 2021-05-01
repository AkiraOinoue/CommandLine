# CommandLine</br>
Command Line Processor</br>
<<<CommandLineの概要>>></br>
#■c++のプログラムで容易にコマンドラインの情報を取得する事ができます。</br>
◆特徴</br>
　○必須オプションと任意オプションを自動チェック</br>
</br>
　○指定オプションの値を型指定して取得可能</br>
	 例）int i = cmd.param<int>("opt");</br>
</br>
　○オプションパラメータファイルに対応</br>
	 コマンドラインの限界を超えるような長いオプション指定にも</br>
	 簡単に対応可能。</br>
	 cmd.options(パラメータファイルパス);</br>
	 xml形式のファイルにオプションやパラメータを記述し、解析が可能</br>
	 以下はフォーマット。<Command>～</Command>で挟む。</br>
	 optタグのnameにオプション名を指定。</br>
	 オプションパラメータは任意のタグ<file>値</file>などで指定可能。</br>
	コメントとして<!-- ～ -->が使用できます。</br>
	<Command></br>
		<!-- option--></br>
		<opt name="copy"/></br>
			<opt name="from"/></br>
				<file>france</file></br>
			<opt name="to"/></br>
				<file>japan</file></br>
		<!-- comment --></br>
		<opt name="type"/></br>
			<file>Amereican</file></br>
			<file>Canada</file></br>
	</Command></br>
	※）Utf-8のエンコードファイルか半角英数文字だけのShift-JISのXMLファイルでないと正しく読めません。</br>
</br>
　○任意オプションの値省略時にデフォルト値を設定</br>
	 例）--opt で値を省略したときにデフォルト値"."を設定。</br>
	 フォルダ指定の省略値にカレントフォルダを指定などが</br>
	 可能。</br>
</br>
　○長いオプション名を省略してマッチング</br>
	 例）option_nameは。。。</br>
		--o or --opti など他のオプションと識別できる最小文字数で</br>
		自動的にマッチング</br>
</br>
　○オプションに複数の値を指定可能</br>
	 例）--opt aaa bbb ccc ...</br>
	 	--opt=aaa=bbb=ccc ...</br>
	 	オプションパラメータ区切りを指定可能です。</br>
	 	CommandLine::setoptprm_sep("=");</br>
</br>
　○オプションパラメータにハッシュデータを指定できます。</br>
    例）--opt os="Windows 7" cpu=coreI7 "office tools"=Word=Excel=Powerpoint</br>
    以下のように解釈できます。</br>
    optのパラメータとして、</br>
    osの値はWindows 7</br>
    cpuの値はcoreI7</br>
    office toolsの値はWordとExcelとPowerpoint</br>
</br>
    あらかじめCommandLine::setoptprm_sep("=");で指定する必要があります。</br>
	※）キーワードや値にブランクを含む場合は「""」（Wクォート）で囲む必要があります。</br>
</br>
　○同一オプションを複数指定可能</br>
	 例）--opt xxx --opt bbb ...</br>
</br>
	optのname="オプション名を指定します"</br>
	<タグ名>オプションパラメータを指定します。</タグ名></br>
    タグ名は任意の名称を指定します。</br>
</br>
　○複数の非オプション文字を取得可能</br>
</br>
　○オプションに登録した省略値を設定できます。</br>
　　オプションのパラメータを省略した場合の省略値を指定しておくと</br>
　　値を省略した時に省略値が設定されます。</br>
　　例）--param （省略値=.\param.xml）のときに</br>
　　1) --param</br>
　　2) --param defparam.xml</br>
</br>
　　1)の場合のパラメータ値はparam.xmlになります。</br>
　　2)の場合のパラメータ値はdefparam.xmlになります。</br>
</br>
　○オプションの区切り文字列を指定可能</br>
	 他のOS(Windows以外)への移植がしやすい</br>
	 例） --opt or /opt or ::opt など任意の文字列が指定できる。</br>
</br>
	該当メソッド：CommandLine::setSep("::");</br>
</br>
　○help表示情報を登録・簡単に参照可能</br>
　　　・メッセージ中の"@"をオプション区切り文字に自動変換表示</br>
</br>
　○オプションエラー発生時はstd::exception& eでcatch可能</br>
	　・CmdLineException& eで個別にcatchが可能</br>
		e.what()でメッセージ出力</br>
</br>
#■コンパイル方法</br>
以下のヘッダファイルをインクルードできるようにパスを指定します。</br>
1."CommandLine.h"</br>
2."CmdLineException.h"</br>
3."ParamReader.h"</br>
　（boost未使用の場合はこのファイル内の#define USED_BOOSTLIBをfalseにします（現在の設定：false））</br>
4."Utility.h"</br>
5."boost/property_tree/ptree.hpp"</br>
6."boost/property_tree/xml_parser.hpp""</br>
  boostヘッダの入手は　http:/www.boost.org をご参照下さい。</br>
  ヘッダをインクルードするだけで使用可能。</br>
  （デフォルトではこのヘッダは不要です。）</br>
</br>
#■リンク方法</br>
CommandLine.lib(アーカイブファイル)をリンクします。</br>
このライブラリーは上記のソースとヘッダファイルで作成できます。</br>
</br>
#■開発環境</br>
Visual Studio 2019 C++/Windows10</br>
