// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#include <string>
#include "CmdLineException.h"
using namespace std;
using namespace locallib;
CmdLineException::CmdLineException() throw()
{
    //ctor
}
CmdLineException::~CmdLineException() throw()
{
    //dtor
}
CmdLineException::CmdLineException(const char* error) throw()
{
    this->m_error = error;
}
// error message
const char* CmdLineException::what() const throw()
{
    return this->m_error.c_str();
}











