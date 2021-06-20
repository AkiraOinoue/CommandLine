// String replace functor
// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
namespace locallib {
    class StrReplace
    {
    private:
        std::string m_fstr;
        const char* m_rstr;
        const std::string m_tstr;
        char m_crstr;
        char m_ctstr;
    public:
        StrReplace():
            m_crstr(NULL),
            m_ctstr(NULL),
            m_rstr(nullptr),
            m_fstr(""),
            m_tstr("")
        {}
        // fstr=orginal string, rstr=replace string, tstr=to string
        StrReplace(std::string fstr, const char* rstr, std::string tstr):
            m_crstr(NULL),
            m_ctstr(NULL),
            m_fstr(fstr), m_rstr(rstr), m_tstr(tstr){}
        // fstr=orginal string, rstr=replace string, tstr=to string
        StrReplace(std::string fstr, std::string rstr, std::string tstr):
            m_crstr(NULL),
            m_ctstr(NULL),
            m_fstr(fstr), m_rstr(rstr.c_str()), m_tstr(tstr){}
        // this->m_fstr=orginal string, rstr=replace string, tstr=to string
        StrReplace(const char rstr, const char tstr):
            m_rstr(nullptr),
            m_crstr(rstr), m_ctstr(tstr){}
        // fstr=orginal string, rstr=replace string, tstr=to string
        StrReplace(std::string fstr):
            m_crstr(NULL),
            m_ctstr(NULL),
            m_rstr(nullptr),
            m_fstr(fstr){}
        // fstr=orginal string, rstr=replace string, tstr=to string
        void replace( std::string& fstr, const char* rstr, std::string tstr );
        // fstr=orginal string, rstr=replace string, tstr=to string
        void replace( std::string& fstr, std::string rstr, std::string tstr );
        // this->m_fstr=orginal string, this->m_rstr=replace string, this->m_tstr=to string
        std::string operator()();
        // this->m_fstr=orginal string, rstr=replace string, tstr=to string
        std::string operator()(const char* rstr, std::string tstr);
        // this->m_fstr=orginal string, rstr=replace string, tstr=to string
        std::string operator()(std::string rstr, std::string tstr);
        // fstr=orginal char, this->m_crstr=replace char, this->m_tstr=to char
        void operator()(char& fstr);
    };
}
#endif // UTILITY_H
