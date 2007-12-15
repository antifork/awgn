/* $Id$ */
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <bonelli@antifork.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli 
 * ----------------------------------------------------------------------------
 */

#ifndef TRISTATE_HH
#define TRISTATE_HH

#include <iostream>

namespace more {

    class tristate {

        public:
            enum state { Unknown=-1, False=0, True=1};

            tristate(const state &s = Unknown) : val(s) {}
            tristate operator!() {
                tristate tmp(*this);
                if (tmp.val == True) {
                    tmp.val = False;
                } else if (tmp.val == False) {
                    tmp.val = True;
                }    
                return tmp;
            }
            operator bool() { return val == True; }
            operator int()  { return val; } 

            tristate &operator=(const tristate &x) {
                val = x.val;
                return *this;
            }
            tristate &operator=(state x) {
                val = x;
                return *this;
            }
            tristate &operator=(bool x) {
                if (x == true) {
                    val = True;
                } 
                else if ( x == false) {
                    val = False;
                }
                return *this;       
            }

            bool operator==(const tristate &x) {
                return val == x.val;
            }
            bool operator==(state x) {
                return val == x;
            }
            bool operator==(bool x) {
                if (x) 
                    return val == True;
                return val == False;
            }

            operator std::string() {
                if (val == True)
                    return "{true}";
                if (val == False)
                    return "{false}";
                return "{unknown}";
            }     

        private:
            state val;
    };
}

#endif /* TRISTATE_HH */

