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
 *  gcc (cygwin) に wstring が存在しないため暫定処理．
 *  最終的には，BOOST_NO_CWCHAR を定義すれば良くなる模様．
 *  wstring が定義されているコンパイラの場合は，コメントアウト．
 */
/* ------------------------------------------------------------------------- */
//namespace std {
//    typedef basic_string<wchar_t> wstring;
//}
// boostライブラリ版
#define USED_BOOSTLIB false
#if USED_BOOSTLIB
    #include <boost/property_tree/ptree.hpp>
    #include <boost/property_tree/xml_parser.hpp>
#else
// Windowsネイティブ版
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









