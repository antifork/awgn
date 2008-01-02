/* $Id$ */
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

template <bool val> struct CTassert;
template <>
struct CTassert<true>{};

template <int v>
struct int2type {
    enum { value = v };
};

namespace ipt {

    struct table {
        enum val { filter=1, nat, mangle, raw
        };
    };
    static const char *table_str[] = {
        NULL, "filter", "nat", "mangle", "raw"
    };

    struct chain {
        enum val { NONE, INPUT, FORWARD, OUTPUT, 
            PREROUTING, POSTROUTING, };
        enum { user = POSTROUTING+1 };
    };
    static const char *chain_str[16] = {
        NULL, "INPUT", "FORWARD", "OUTPUT", "PREROUTING", "POSTROUTING", };

    struct target {
        enum val {
            ACCEPT=1, DROP, QUEUE, RETURN, CLASSIFY, CLUSTERIP, CONNMARK, CONNSECMARK, DNAT, 
            DSCP, ECN, IPV4OPTSSTRIP, LOG, MARK, MASQUERADE, MIRROR, NETMAP, NFQUEUE, NOTRACK, 
            REDIRECT, REJECT, SAME, SECMARK, SET, SNAT, TARPIT, TCPMSS, TOS, TTL, ULOG
        };
    };
    static const char * target_str[] = {
        NULL, "ACCEPT", "DROP", "QUEUE", "RETURN", "CLASSIFY", "CLUSTERIP", "CONNMARK", "CONNSECMARK", "DNAT",
        "DSCP", "ECN", "IPV4OPTSSTRIP", "LOG", "MARK", "MASQUERADE", "MIRROR", "NETMAP", "NFQUEUE", "NOTRACK",
        "REDIRECT", "REJECT", "SAME", "SECMARK", "SET", "SNAT", "TARPIT", "TCPMSS", "TOS", "TTL", "ULOG"
    };

    template <table::val TABLE, chain::val CHAIN = chain::NONE>
    class iptables {

#define usr_chain(x) ( x >= static_cast<chain::val>(chain::user) )

        // tables check...
        //
        static void CTtable_check(int2type<table::filter>) {
            CTassert< CHAIN == chain::INPUT         ||
                      CHAIN == chain::OUTPUT        ||
                      CHAIN == chain::FORWARD       ||
                      usr_chain(CHAIN) > chain_for_table_filter __attribute__((unused));
        }

        static void CTtable_check(int2type<table::nat>) {
            CTassert< CHAIN == chain::PREROUTING    ||
                      CHAIN == chain::OUTPUT        ||
                      CHAIN == chain::POSTROUTING   ||
                      usr_chain(CHAIN) > chain_for_table_nat __attribute__((unused));
        }

        static void CTtable_check(int2type<table::mangle>) {
            CTassert< CHAIN == chain::PREROUTING    ||
                      CHAIN == chain::OUTPUT        ||
                      CHAIN == chain::INPUT         ||
                      CHAIN == chain::FORWARD       ||
                      CHAIN == chain::POSTROUTING   ||
                      usr_chain(CHAIN) > chain_for_table_mangle __attribute__((unused));
        }

        static void CTtable_check(int2type<table::raw>) {
            CTassert< CHAIN == chain::PREROUTING   ||
                      CHAIN == chain::OUTPUT       ||
                      usr_chain(CHAIN) >  chain_for_table_raw __attribute__((unused));
        }

        // targets check...
        //
        static void CTtarget_check(int2type<target::ACCEPT>) {
            CTassert< TABLE == table::filter > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::DROP>) {
            CTassert< TABLE == table::filter > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::RETURN>) {
            CTassert< TABLE == table::filter > table_for_target __attribute__((unused));
        }    
        static void CTtarget_check(int2type<target::REJECT>) {
            CTassert< TABLE == table::filter > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::QUEUE>) {
            CTassert< TABLE == table::filter > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::NFQUEUE>) {
            CTassert< TABLE == table::filter > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::CLASSIFY>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
            CTassert< CHAIN == chain::OUTPUT            || 
                      CHAIN == chain::FORWARD           || 
                      CHAIN == chain::POSTROUTING       ||
                      usr_chain(CHAIN) > chain_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::CONNSECMARK>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
        }    
        static void CTtarget_check(int2type<target::DNAT>) {
            CTassert< TABLE == table::nat > table_for_target __attribute__((unused));
            CTassert< CHAIN == chain::PREROUTING || 
                      CHAIN == chain::OUTPUT     ||
                      usr_chain(CHAIN) > chain_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::SNAT>) {
            CTassert< TABLE == table::nat > table_for_target __attribute__((unused));
            CTassert< CHAIN == chain::POSTROUTING ||
                      usr_chain(CHAIN) > chain_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::MASQUERADE>) {
            CTassert< TABLE == table::nat > table_for_target __attribute__((unused));
            CTassert< CHAIN == chain::POSTROUTING || 
                      usr_chain(CHAIN) > chain_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::NETMAP>) {
            CTassert< TABLE == table::nat > table_for_target __attribute__((unused));
            CTassert< CHAIN == chain::PREROUTING    || 
                      CHAIN == chain::POSTROUTING   || 
                      CHAIN == chain::OUTPUT ||
                      usr_chain(CHAIN) > chain_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::REDIRECT>) {
            CTassert< TABLE == table::nat > table_for_target __attribute__((unused));
            CTassert< CHAIN == chain::PREROUTING    || 
                      CHAIN == chain::OUTPUT || 
                      usr_chain(CHAIN) > chain_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::SAME>) {
            CTassert< TABLE == table::nat > table_for_target __attribute__((unused));
            CTassert< CHAIN == chain::PREROUTING    || 
                      CHAIN == chain::POSTROUTING   ||
                      usr_chain(CHAIN) > chain_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::DSCP>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::ECN>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::MARK>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::NOTRACK>) {
            CTassert< TABLE == table::raw > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::SECMARK>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::TOS>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(int2type<target::TTL>) {
            CTassert< TABLE == table::mangle > table_for_target __attribute__((unused));
        }
        static void CTtarget_check(...) {
        }

        static int exec(const std::string &c, const std::string chain, 
                        const std::string &rulespec, const std::string &opt="") {
            std::string com( more::sprint("%s -t %s %s %s %s %s", 
                             "/sbin/iptables", table_str[TABLE], c.c_str(), chain.c_str(), 
                                                                 rulespec.c_str(), opt.c_str()));
            std::cout << ".:[" << com << "]:.\n";
            int ret = system(com.c_str());
            if ( ret < 0 || WEXITSTATUS(ret) != 0  )
                std::clog << more::sprint("system(): WEXITSTATUS=%d", WEXITSTATUS(ret)) << std::endl;
            return WEXITSTATUS(ret); 
        }

    public:

        template <target::val TARGET>
        static int append(const std::string &rule, const std::string &opt="") {
            CTtable_check(int2type<TABLE>());
            CTtarget_check(int2type<TARGET>());
            std::string chain(chain_str[CHAIN]);
            std::string target(target_str[TARGET]);
            return exec("-A", chain, rule, std::string("-j ").append(target).append(" ").append(opt));
        }

        template <target::val TARGET>
        static int del(const std::string &rule, const std::string &opt="") {
            CTtable_check(int2type<TABLE>());
            CTtarget_check(int2type<TARGET>());
            std::string chain(chain_str[CHAIN]);
            std::string target(target_str[TARGET]);
            return exec("-D", chain, rule, std::string("-j ").append(target).append(" ").append(opt));
        }

        static int del(unsigned int rulenum, const std::string &opt="") {
            CTtable_check(int2type<TABLE>());
            std::string chain(chain_str[CHAIN]);
            return exec("-D", chain, more::sprint("%d", rulenum), opt);
        }

        template <target::val TARGET>
        static int insert(const std::string &rule, const std::string &opt="") {
            CTtable_check(int2type<TABLE>());
            CTtarget_check(int2type<TARGET>());
            std::string chain(chain_str[CHAIN]);
            std::string target(target_str[TARGET]);
            return exec("-I", chain, rule, std::string("-j ").append(target).append(" ").append(opt));
        }

        template <target::val TARGET>
        static int insert(const std::string &rule, unsigned int rulenum, const std::string &opt="") {
            CTtable_check(int2type<TABLE>());
            CTtarget_check(int2type<TARGET>());
            std::string chain(chain_str[CHAIN]);
            std::string target(target_str[TARGET]);
            std::string ruln( rulenum ? more::sprint("%d",rulenum) : std::string("") );
            return exec("-I", chain , ruln.append(" ").append(rule), 
                        std::string("-j ").append(target).append(" ").append(opt));
        }

        template <target::val TARGET>
        static int replace(const std::string &rule, unsigned int rulenum, const std::string &opt="") {
            CTtable_check(int2type<TABLE>());
            CTtarget_check(int2type<TARGET>());
            std::string chain(chain_str[CHAIN]);
            std::string target(target_str[TARGET]);
            std::string ruln( rulenum ? more::sprint("%d",rulenum) : std::string("") );
            return exec("-R", chain, ruln.append(" ").append(rule), 
                        std::string("-j ").append(target).append(" ").append(opt));
        }

        static int list(const std::string &opt="") {
            std::string c( CHAIN == 0 ? "" : chain_str[CHAIN]);
            return exec("-L", c, opt);
        }

        static int flush(const std::string &opt="") {
            std::string c( CHAIN == 0 ? "" : chain_str[CHAIN]);
            return exec("-F", c, opt);
        }

        static int zero(const std::string &opt="" ) {
            std::string c( CHAIN == 0 ? "" : chain_str[CHAIN]);
            return exec("-Z", c, opt);
        }

        static int policy(const std::string &target, const std::string &opt="") {
            CTassert<CHAIN != chain::NONE> chain;
            std::string c( CHAIN == 0 ? "" : chain_str[CHAIN]);
            return exec("-P", c, target, opt); 
        }

    };

}

#endif /* IPTABLES_HH */
