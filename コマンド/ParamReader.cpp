// ----------------------------------------------------------------------------
// Copyright (C) 2019 davinci1958@docomonet.jp
// ----------------------------------------------------------------------------
#include "ParamReader.h"
using namespace locallib;
ParamReader::ParamReader()
{
    //dtor
#if USED_BOOSTLIB == false
    this->lpxml = new stdxml::XMLRead;
#endif    
}
ParamReader::~ParamReader()
{
    //dtor
}
void ParamReader::set_tag( string_type val)
{
    this->m_tag = val;
}
ParamReader::string_type ParamReader::get_tag()
{
    return this->m_tag;
}
void ParamReader::set_attr_value( string_type val)
{
    this->m_attr_value = val;
}
ParamReader::string_type ParamReader::get_attr_value()
{
    return this->m_attr_value;
}
