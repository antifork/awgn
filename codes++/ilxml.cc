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

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include <xmlParser.hh>

#include <cstdio>
#include <err.h>

#define INLINE_XML  "__xml__"

extern char *__progname;

static char usage[] = "\
Usage: %s [options] file\n\
Options:\n\
  -o <file>           place the output into <file>\n\
  -h                  help\n";

enum
{
    S_cpp = 0,
    S_xml,
    S_pp,
};

std::ostream* outstream = &std::cout;

std::string 
str2var(const std::string &var)
{
    if (var.empty())
        return "";
    if (var[0] == '$')
        return std::string("ilx::addr(ilx::str(").append(var.substr(1)).append("))");
    return std::string("\"").append(var).append("\"");
}

void 
addNode(const XMLNode &node) 
{
    *outstream << "({ XMLNode ret = XMLNode::createXMLTopNode(" << str2var(node.getName()) << ");\n";
    for(int i=0; i <node.nAttribute(); ++i)
        *outstream << "   ret.addAttribute(" << str2var(node.getAttributeName(i))  << "," << 
        str2var(node.getAttributeValue(i)) << ");\n"; 
    for(int i=0; i <node.nText(); i++) {
        *outstream << "   ret.addText(" << str2var(node.getText(i)) << ");\n";
    }
    for(int i=0; i <node.nChildNode(); ++i) {
        *outstream << "   ret.addChild(\n";
        addNode(node.getChildNode(i));
        *outstream << "   );\n";
    }
    *outstream << "   ret; })\n";    
}

void
preprocessor(const std::string &xml) 
{
    XMLNode root = XMLNode::parseString(xml.c_str());

    *outstream << "({ XMLNode ret = XMLNode::createXMLTopNode(\"xml\",true);\n";
    *outstream << "   ret.addAttribute(\"version\",\"1.0\");\n";
    *outstream << "   ret.addChild(\n";
    addNode(root);
    *outstream << "   );\n";
    *outstream << "   ret; })\n";
}


int
parser(const std::string &f) 
{
    std::ifstream source(f.c_str());
    if (!source)
        err(1,"ifstream");

    std::string __xml__;
    std::string buffer;

    int state = S_cpp;
    int line=0; 

    while( std::getline(source,buffer) ) {
        unsigned int ph;
        ++line;

restart:
        switch (state) {

        case S_cpp:
            ph = buffer.find(INLINE_XML);
            if (ph == std::string::npos) {
                *outstream << buffer << std::endl;
                continue;
            }

            // dump the line until __xml__
            *outstream << buffer.substr(0,ph) << std::endl;
            buffer = buffer.substr(ph+7);

            // find the first occurrence of (
            ph = buffer.find('(');
            if (ph == std::string::npos)
                errx(2, "parse error at line %d", line);

            buffer = buffer.substr(ph+1);
            state  = S_xml;
            goto restart;

        case S_xml:
            ph = buffer.find(')');
            if (ph == std::string::npos) {
                __xml__.append(buffer).append("\n");
                continue;
            }
            __xml__.append(buffer.substr(0,ph));
            buffer = buffer.substr(ph+1);
            state  = S_pp;
            goto restart;

        case S_pp:
            preprocessor(__xml__);
            state = S_cpp;
            __xml__.clear();
            goto restart;
        }
    }

    return 0;
}


int 
main(int argc, char *argv[])
{
    const char *out = NULL;

    int opt;

    while ( (opt=getopt(argc, argv, ":o:a:h")) != EOF )
        switch(opt) {
        case 'h':
            printf(usage,__progname);
            exit(0);
        case 'o':
            out = optarg;
            break;
        case ':':
            errx(1,"option requires an argument -- %c",optopt);
        case '?':
            errx(2, "invalid option -- %c",optopt);
        }

    argc -= optind;
    argv += optind;

    if (!argc)
        errx(0,"no input file");

    std::ofstream file;

    if (out) {
        file.open(out);
        if (!file)
            errx(3, "ofstream");
        outstream = &file; 
    }

    parser(argv[0]);

    return 0;
}
