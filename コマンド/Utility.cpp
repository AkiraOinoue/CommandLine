// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#include "Utility.h"
using namespace std;
using namespace locallib;
// fstr�̒���rstr��S��tstr�ɕϊ����܂��B
void StrReplace::replace( std::string& fstr, const char* rstr, std::string tstr )
{
    size_t rlen = string(rstr).size();
    for(;;)
    {
        size_t  pos = fstr.find( rstr );
        if ( pos != fstr.npos )
        {
            fstr.replace(pos, rlen, tstr);
        }
        else
        {
            break;
        }
    }
}
// fstr�̒���rstr��S��tstr�ɕϊ����܂��B
void StrReplace::replace( std::string& fstr, std::string rstr, std::string tstr )
{
    this->replace(fstr, rstr.c_str(), tstr);
}
// this->m_fstr�̒���m_rstr��S��m_tstr�ɕϊ����܂��B
std::string StrReplace::operator()()
{
    this->replace(this->m_fstr, this->m_rstr, this->m_tstr);
    return this->m_fstr;
}
// this->m_fstr�̒���rstr��S��tstr�ɕϊ����܂��B
std::string StrReplace::operator()(const char* rstr, std::string tstr)
{
    this->replace(this->m_fstr, rstr, tstr);
    return m_fstr;
}
// this->m_fstr�̒���rstr��S��tstr�ɕϊ����܂��B
std::string StrReplace::operator()(std::string rstr, std::string tstr)
{
    this->replace(this->m_fstr, rstr, tstr);
    return m_fstr;
}
// cfstr��this->m_crstr�Ȃ��this->m_ctstr�ɕϊ����܂��B
void StrReplace::operator()(char& cfstr)
{
    if(this->m_crstr == cfstr)
    {
        cfstr = this->m_ctstr;
    }
}








