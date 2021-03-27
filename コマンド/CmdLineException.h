/*
    �R�}���h���C���N���X�p��O�N���X�̒�`
    �E�R�}���h���C���������̗�O�������L���b�`�����A�G���[���b�Z�[�W��\������B
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
