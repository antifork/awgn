/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#ifndef IPTABLES_HH
#define IPTABLES_HH

#include <iostream>
#include <string>
#include <sprint.hh>

#define IPTABLES "/sbin/iptables"

template <bool val> struct CTassert;
template <>
struct CTassert<true>{};

template <int v>
struct int2type {
    enum { value = v };
};

namespace ipt {

    enum ipt_table {
        filter=1,
        nat,
        mangle,
        raw
    };

    static const char *ipt_table_str[] = {
        NULL, "filter", "nat", "mangle", "raw"
    };

    enum ipt_chain {
        NONE, INPUT, FORWARD, OUTPUT, PREROUTING, POSTROUTING, };

    static const char *ipt_chain_str[] = {
        NULL, "INPUT", "FORWARD", "OUTPUT", "PREROUTING", "POSTROUTING", };

    enum ipt_target {
        ACCEPT=1, DROP, QUEUE, RETURN, CLASSIFY, CLUSTERIP, CONNMARK, CONNSECMARK, DNAT, 
        DSCP, ECN, IPV4OPTSSTRIP, LOG, MARK, MASQUERADE, MIRROR, NETMAP, NFQUEUE, NOTRACK, 
        REDIRECT, REJECT, SAME, SECMARK, SET, SNAT, TARPIT, TCPMSS, TOS, TTL, ULOG
    };

    static const char * ipt_target_str[] = {
        NULL, "ACCEPT", "DROP", "QUEUE", "RETURN", "CLASSIFY", "CLUSTERIP", "CONNMARK", "CONNSECMARK", "DNAT",
        "DSCP", "ECN", "IPV4OPTSSTRIP", "LOG", "MARK", "MASQUERADE", "MIRROR", "NETMAP", "NFQUEUE", "NOTRACK",
        "REDIRECT", "REJECT", "SAME", "SECMARK", "SET", "SNAT", "TARPIT", "TCPMSS", "TOS", "TTL", "ULOG"
    };

template <ipt_table TABLE, ipt_chain CHAIN = NONE>
class iptables {

    // tables check...
    //
    static void CTtable_check(int2type<filter>) {
        CTassert< CHAIN == INPUT  ||
                  CHAIN == OUTPUT ||
                  CHAIN == FORWARD> chain_for_table_filter __attribute__((unused));
    }

    static void CTtable_check(int2type<nat>) {
        CTassert< CHAIN == PREROUTING   ||
                  CHAIN == OUTPUT       ||
                  CHAIN == POSTROUTING> chain_for_table_nat __attribute__((unused));
    }

    static void CTtable_check(int2type<mangle>) {
        CTassert< CHAIN == PREROUTING   ||
                  CHAIN == OUTPUT       ||
                  CHAIN == INPUT        ||
                  CHAIN == FORWARD      ||
                  CHAIN == POSTROUTING> chain_for_table_mangle __attribute__((unused));
    }

    static void CTtable_check(int2type<raw>) {
        CTassert< CHAIN == PREROUTING   ||
                  CHAIN == OUTPUT>  chain_for_table_raw __attribute__((unused));
    }

    // targets check...
    //
    static void CTtarget_check(int2type<ACCEPT>) {
        CTassert< TABLE == filter > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<DROP>) {
        CTassert< TABLE == filter > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<RETURN>) {
        CTassert< TABLE == filter > table_for_target __attribute__((unused));
    }    
    static void CTtarget_check(int2type<REJECT>) {
        CTassert< TABLE == filter > table_for_target __attribute__((unused));
        CTassert< CHAIN == INPUT || CHAIN == FORWARD || CHAIN == OUTPUT > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<QUEUE>) {
        CTassert< TABLE == filter > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<NFQUEUE>) {
        CTassert< TABLE == filter > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<CLASSIFY>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
        CTassert< CHAIN == OUTPUT || CHAIN == FORWARD || CHAIN == POSTROUTING > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<CONNSECMARK>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
    }    
    static void CTtarget_check(int2type<DNAT>) {
        CTassert< TABLE == nat > table_for_target __attribute__((unused));
        CTassert< CHAIN == PREROUTING || CHAIN == OUTPUT > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<SNAT>) {
        CTassert< TABLE == nat > table_for_target __attribute__((unused));
        CTassert< CHAIN == POSTROUTING > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<MASQUERADE>) {
        CTassert< TABLE == nat > table_for_target __attribute__((unused));
        CTassert< CHAIN == POSTROUTING > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<NETMAP>) {
        CTassert< TABLE == nat > table_for_target __attribute__((unused));
        CTassert< CHAIN == PREROUTING || CHAIN == POSTROUTING || CHAIN == OUTPUT > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<REDIRECT>) {
        CTassert< TABLE == nat > table_for_target __attribute__((unused));
        CTassert< CHAIN == PREROUTING || CHAIN == OUTPUT > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<SAME>) {
        CTassert< TABLE == nat > table_for_target __attribute__((unused));
        CTassert< CHAIN == PREROUTING || CHAIN == POSTROUTING > chain_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<DSCP>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<ECN>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<MARK>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<NOTRACK>) {
        CTassert< TABLE == raw > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<SECMARK>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<TOS>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
    }
    static void CTtarget_check(int2type<TTL>) {
        CTassert< TABLE == mangle > table_for_target __attribute__((unused));
    }

    static void CTtarget_check(...) {
    }


    static int exec(const std::string &c, const std::string chain, const std::string &rulespec, const std::string &opt="") {
        std::string com( 
            extra::sprint("%s -t %s %s %s %s %s", 
            IPTABLES, ipt_table_str[TABLE], c.c_str(), chain.c_str(), rulespec.c_str(), opt.c_str()));

        std::cout << ".:[" << com << "]:.\n";
        int ret = system(com.c_str());
        if ( ret < 0 || WEXITSTATUS(ret) != 0  )
            std::clog << extra::sprint("system(): WEXITSTATUS=%d", WEXITSTATUS(ret)) << std::endl;
        return WEXITSTATUS(ret); 
    }

    public:

    template <ipt_target TARGET>
    static int append(const std::string &rule, const std::string &opt="") {

        CTtable_check(int2type<TABLE>());
        CTtarget_check(int2type<TARGET>());

        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        return exec( "-A" , chain , rule, std::string("-j ").append(target).append(" ").append(opt));
    }

    template <ipt_target TARGET>
    static int del(const std::string &rule, const std::string &opt="") {

        CTtable_check(int2type<TABLE>());
        CTtarget_check(int2type<TARGET>());

        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        return exec( "-D" , chain ,rule, std::string("-j ").append(target).append(" ").append(opt));
    }

    static int del(unsigned int rulenum, const std::string &opt="") {

        CTtable_check(int2type<TABLE>());

        std::string chain(ipt_chain_str[CHAIN]);
        return exec( "-D", chain, extra::sprint("%d", rulenum), opt);
    }

    template <ipt_target TARGET>
    static int insert(const std::string &rule, const std::string &opt="") {

        CTtable_check(int2type<TABLE>());
        CTtarget_check(int2type<TARGET>());

        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        return exec( "-I" , chain , rule, std::string("-j ").append(target).append(" ").append(opt));
    }

    template <ipt_target TARGET>
    static int insert(const std::string &rule, unsigned int rulenum, const std::string &opt="") {

        CTtable_check(int2type<TABLE>());
        CTtarget_check(int2type<TARGET>());

        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        std::string ruln( rulenum ? extra::sprint("%d",rulenum) : std::string("") );
        return exec( "-I" , chain , ruln.append(" ").append(rule), std::string("-j ").append(target).append(" ").append(opt));
    }

    template <ipt_target TARGET>
    static int replace(const std::string &rule, unsigned int rulenum, const std::string &opt="") {

        CTtable_check(int2type<TABLE>());
        CTtarget_check(int2type<TARGET>());

        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        std::string ruln( rulenum ? extra::sprint("%d",rulenum) : std::string("") );
        return exec( "-R" , chain , ruln.append(" ").append(rule), std::string("-j ").append(target).append(" ").append(opt));
    }

    static int list(const std::string &opt="") {
        std::string c( CHAIN == 0 ? "" : ipt_chain_str[CHAIN]);
        return exec( "-L" , c , opt);
    }

    static int flush(const std::string &opt="") {
        std::string c( CHAIN == 0 ? "" : ipt_chain_str[CHAIN]);
        return exec( "-F" , c , opt);
    }

    static int zero(const std::string &opt="" ) {
        std::string c( CHAIN == 0 ? "" : ipt_chain_str[CHAIN]);
        return exec( "-Z" , c , opt);
    }

    static int policy(const std::string &target, const std::string &opt="") {
        CTassert<CHAIN != NONE> chain;
        std::string c( CHAIN == 0 ? "" : ipt_chain_str[CHAIN]);
        return exec( "-P", c, target, opt); 
    }

};


}

#endif /* IPTABLES_HH */
