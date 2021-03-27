// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#include "Utility.h"
using namespace std;
using namespace locallib;
// fstrの中のrstrを全てtstrに変換します。
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
// fstrの中のrstrを全てtstrに変換します。
void StrReplace::replace( std::string& fstr, std::string rstr, std::string tstr )
{
    this->replace(fstr, rstr.c_str(), tstr);
}
// this->m_fstrの中のm_rstrを全てm_tstrに変換します。
std::string StrReplace::operator()()
{
    this->replace(this->m_fstr, this->m_rstr, this->m_tstr);
    return this->m_fstr;
}
// this->m_fstrの中のrstrを全てtstrに変換します。
std::string StrReplace::operator()(const char* rstr, std::string tstr)
{
    this->replace(this->m_fstr, rstr, tstr);
    return m_fstr;
}
// this->m_fstrの中のrstrを全てtstrに変換します。
std::string StrReplace::operator()(std::string rstr, std::string tstr)
{
    this->replace(this->m_fstr, rstr, tstr);
    return m_fstr;
}
// cfstrがthis->m_crstrならばthis->m_ctstrに変換します。
void StrReplace::operator()(char& cfstr)
{
    if(this->m_crstr == cfstr)
    {
        cfstr = this->m_ctstr;
    }
}








