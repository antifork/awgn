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
#include <itoa.hh>

#define IPTABLES "/sbin/iptables"
#define IPTassert(cond, val) CTassert <!(cond) || (val)>

template <bool val> struct CTassert;
template <>
struct CTassert<true>{};

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

    static void CTtable_check() {
        IPTassert(TABLE == filter,  CHAIN == INPUT      || 
                                    CHAIN == OUTPUT     || 
                                    CHAIN == FORWARD)       chain_for_table_filter __attribute__((unused)); 
        IPTassert(TABLE == nat,     CHAIN == PREROUTING || 
                                    CHAIN == OUTPUT     || 
                                    CHAIN == POSTROUTING)   chain_for_nat_filter   __attribute__((unused)); 
        IPTassert(TABLE == mangle,  CHAIN == PREROUTING || 
                                    CHAIN == OUTPUT     || 
                                    CHAIN == INPUT      || 
                                    CHAIN == FORWARD    || 
                                    CHAIN == POSTROUTING)   chain_for_mangle_filter __attribute__((unused));  
        IPTassert(TABLE == raw,     CHAIN == PREROUTING || 
                                    CHAIN == OUTPUT)        chain_for_raw_filter __attribute__((unused));
        CTassert<   TABLE == filter || 
                    TABLE == nat    || 
                    TABLE == mangle || 
                    TABLE == raw> wrong_table __attribute__((unused));
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
        CTtable_check();
        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        return exec( "-A" , chain , rule, std::string("-j ").append(target).append(" ").append(opt));
    }

    template <ipt_target TARGET>
    static int del(const std::string &rule, const std::string &opt="") {
        CTtable_check();
        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        return exec( "-D" , chain ,rule, std::string("-j ").append(target).append(" ").append(opt));
    }

    static int del(unsigned int rulenum, const std::string &opt="") {
        CTtable_check();
        std::string chain(ipt_chain_str[CHAIN]);
        return exec( "-D", chain, extra::itoa(rulenum), opt);
    }

    template <ipt_target TARGET>
    static int insert(const std::string &rule, const std::string &opt="") {
        CTtable_check();
        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        return exec( "-I" , chain , rule, std::string("-j ").append(target).append(" ").append(opt));
    }

    template <ipt_target TARGET>
    static int insert(const std::string &rule, unsigned int rulenum, const std::string &opt="") {
        CTtable_check();
        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        std::string ruln( rulenum ? extra::itoa(rulenum) : std::string("") );
        return exec( "-I" , chain , ruln.append(" ").append(rule), std::string("-j ").append(target).append(" ").append(opt));
    }

    template <ipt_target TARGET>
    static int replace(const std::string &rule, unsigned int rulenum, const std::string &opt="") {
        CTtable_check();
        std::string chain(ipt_chain_str[CHAIN]);
        std::string target(ipt_target_str[TARGET]);
        std::string ruln( rulenum ? extra::itoa(rulenum) : std::string("") );
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
