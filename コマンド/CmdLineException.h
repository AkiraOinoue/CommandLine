/*
    コマンドラインクラス用例外クラスの定義
    ・コマンドライン処理中の例外発生をキャッチさせ、エラーメッセージを表示する。
*/
// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#ifndef CMDLINEEXCEPTION_H
#define CMDLINEEXCEPTION_H

#include <exception>
#include <string>

namespace locallib {
    class CmdLineException : public std::exception
    {
        public:
            CmdLineException() throw();
            CmdLineException(const char* error) throw();
            virtual ~CmdLineException() throw();
            // error message
            virtual const char* what() const throw();
        protected:
        private:
            std::string m_error;
    };
}
#endif // CMDLINEEXCEPTION_H
