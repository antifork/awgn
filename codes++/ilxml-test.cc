/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

/*
 * NOTE: ilxml relies on the XML library v2.33 written by 
 *       Frank Vanden Berghen and available from: 
 *       http://www.applied-mathematics.net/tools/xmlParser.html
 */

#include <xmlParser.hh>
#include <string>
#include <iostream>

#include <ilxml.hh>

int main()
{
    int i = 1;
    double j = 0.1;

    std::string fix("value");
    std::string msg("hello world!");
    
    const char node[]="node";
    std::string attr("attr_name");

    XMLNode n = __xml__ (
                <root attr_1="$i" attr_2="$j" >
                    <test attr="$fix"/>
                    <$node $attr="0" > $msg </$node> 
                    <empty/> 
                </root>);

    std::cout << n.createXMLString() << std::endl;        
        
    return 0;
}
