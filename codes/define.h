/*
 *
 * Copyright (c) 2002 Nicola Bonelli <bonelli@antifork.org>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer. 2.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 */

#define CVSID   "$Id: define.h,v 1.10 2002/12/11 21:52:32 awgn Exp $"

typedef unsigned char REGISTER;


#define FATAL(f,arg...) do {						\
	fprintf(stderr,"%s:%d: ",__FILE__,__LINE__);			\
	fprintf(stderr,f,## arg);					\
	exit(1);							\
} while (0)

#define PUTS(f,arg...)  do {      					\
                        fprintf(stderr,f,## arg);                       \
                        fflush (stderr);                                \
} while (0)

#ifdef  EADBUG
#define DEBUG(s,f,arg...) do {  					\
                          fprintf(stderr,"%s::",s);  			\
                          fprintf(stderr,f,## arg); 			\
} while (0)
#else
#define DEBUG(s,f,arg...) do {} while(0)
#endif

#define bit         >>3
#define kbit        <<7
#define mbit        <<17
#define gbit        <<27
#define kbyte       <<10
#define mbyte       <<20
#define gbyte       <<30
#define REG_BS(r,b)      ( r[b>>3] |=   1<<(b&7) )
#define REG_BR(r,b)      ( r[b>>3] &= ~ 1<<(b&7) )
#define REG_BT(r,b)      ( r[b>>3]  &   1<<(b&7) )
#define REG_BN(r,b)      ( r[b>>3] ^=   1<<(b&7) )


#define B_SET_(opt,n,f)      ((opt & f)       ? 			\
(FATAL("%s and %s! Set either of these options", #n, #f)) : (opt|= n))
#define B_SET__(opt,n,f1,f2) ((opt & (f1|f2)) ? 			\
(FATAL("%s and %s! Set either of these options", #n, ( opt & f1 ? #f1 : #f2 ))) : (opt|= n))

#define STR(a)	  # a
#define CAT(a,b)  a ## b
#define XCAT(a,b) CAT(a,b)
#define MIN(a,b) ( (a) < (b) ? (a) : (b) )
#define MAX(a,b) ( (a) > (b) ? (a) : (b) )
#define SIGN(x)  ( (x) > 0 ? 1  : ( (x) == 0 ?   0  :  -1  ) )
#define U(x)     ( (x) > 0 ? x  : 0 )
#define ABS(x)   ( (x) < 0 ? -(x) : (x) )
#define RE(x) (__real__ (x))
#define IM(x) (__imag__ (x))


#define ATOMIC(a,b)     ( (!(a)&&(b)) || ((a)&&!(b)) || (!(a)&&!(b)) )

#define ATOMIC_TEST(a,b)  do {				\
if ( !ATOMIC(options.##a,options.##b) )                 \
FATAL("set either of these options: %s | %s",#a,#b);    \
} while (0)

#define DEPEND_TEST(a,b)  do { 				\
if ( options.##a && !options.##b )                      \
FATAL("%s option require %s",#a,#b);                    \
} while (0)

/* integer math */

#define ISQRT(v)  do { 					\
long g; unsigned long bit; g = 0; 			\
bit = 1<<15; do { g ^= bit; if (g * g > v ) g ^= bit; } \
while (bit >>=1); g; 					\
} while (0)

#define LOG2(x)  do { 					\
int _i=(int)x; int _j=0;				\
while (_i>>=1) _j++; _j; 				\
} while (0)

/* termcap util */

static char esc_up[] = {0x1b, 0x5b, 0x41, 0x0};
static char esc_dw[] = {0x1b, 0x5b, 0x42, 0x0};
static char esc_rg[] = {0x1b, 0x5b, 0x43, 0x0};
static char esc_lf[] = {0x1b, 0x5b, 0x44, 0x0};

#define SLEEP_LOOP      2

#define LOOP(x) do { 					\
while ( (x) == -1 ) sleep(SLEEP_LOOP); 			\
} while (0)

#define NCUT(x) do { 					\
int _i=0; 						\
while(x[_i]!=0) 					\
{ if (x[_i]=='\n') x[_i]=0; _i++; } 			\
x; 							\
} while (0)

#define GET_WSCOL() do { 				\
struct winsize ws = { 0, 0, 0, 0 }; 			\
ioctl (0, TIOCGWINSZ, &ws); 				\
ws.ws_col; 						\
} while (0)

#define LINE_CL()  do { 				\
int _i; 						\
for(_i=0; _i < GET_WSCOL() ; _i++) 			\
putchar(' '); putchar('\r'); 				\
} while (0)

#define LINE_UP()	fputs(esc_up,stdout)
#define LINE_DW()	fputs(esc_dw,stdout)
