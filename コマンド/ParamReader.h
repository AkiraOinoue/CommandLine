// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#include <cstdlib>
#include <string>
#include <vector>
#include <limits>
#include <exception>
/* ------------------------------------------------------------------------- */
/*
 *  gcc (cygwin) �� wstring �����݂��Ȃ����ߎb�菈���D
 *  �ŏI�I�ɂ́CBOOST_NO_CWCHAR ���`����Ηǂ��Ȃ�͗l�D
 *  wstring ����`����Ă���R���p�C���̏ꍇ�́C�R�����g�A�E�g�D
 */
/* ------------------------------------------------------------------------- */
//namespace std {
//    typedef basic_string<wchar_t> wstring;
//}
// boost���C�u������
#define USED_BOOSTLIB false
#if USED_BOOSTLIB
    #include <boost/property_tree/ptree.hpp>
    #include <boost/property_tree/xml_parser.hpp>
#else
// Windows�l�C�e�B�u��
    #include "XmlRead.hpp"
#endif // USED_BOOSTLIB

#ifndef PARAMREADER_H
#define PARAMREADER_H
// XML format example
/*
<Command>
    <opt name="multi"/>
		<d>france</d>
		<d>japan</d>
    <opt name="type"/>
		<d>Amereican</d>
</Command>
*/
/* ------------------------------------------------------------------------- */
//  ParamReader
/* ------------------------------------------------------------------------- */
namespace locallib {
    class ParamReader
    {
    public:
        typedef std::string string_type;
#if USED_BOOSTLIB
        typedef boost::property_tree::ptree ptree_type;
        typedef ptree_type::key_type key_type;
        typedef ptree_type::value_type value_type;
        typedef ptree_type::const_assoc_iterator    ptIT_type;
#else
        stdxml::XMLRead* lpxml;
#endif
    public:
        ParamReader();
        virtual ~ParamReader();
    private:
        string_type m_tag;
        string_type m_attr_value;
    public:
        /* ------------------------------------------------------------------------- */
        //  child_count
        /* ------------------------------------------------------------------------- */
        template <class PTree>
        size_t child_count(const PTree& x) {
            return x.size() - x.count("<xmlattr>");
        }
        virtual void set_tag( string_type val);
        virtual string_type get_tag();
        virtual void set_attr_value( string_type val);
        virtual string_type get_attr_value();
    };
}
#endif // PARAMREADER_H









