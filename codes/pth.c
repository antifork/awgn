/*
 * $Id: pth.c,v 1.5 2003/01/30 10:46:49 awgn Exp $
 *
 * pthread_alarm(), pthread_setjmp(), pthread_longjmp().
 *  
 * Copyright (c) 2002 Nicola Bonelli <bonelli@antifork.org>
 *
 * All rights reserved.
 * 
 * Linux 2.4	   :  gcc pthread.c -lpthread
 * FreeBSD 4.5|4.6 :  gcc pthread.c -pthread 
 * OpenBSD ?? bugged reentrant library. 
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <setjmp.h>
#include <signal.h>
#include <pthread.h>

static pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t jmp_mutex   = PTHREAD_MUTEX_INITIALIZER;

static pthread_t jmp_owner;

#define pthread_alarm(sec) ({				\
    int ret = 0;					\
    sigset_t set;					\
    sigemptyset (&set);					\
    sigaddset (&set, SIGALRM);				\
							\
    if (sec != 0) {					\
        pthread_mutex_lock (&alarm_mutex);		\
        pthread_mutex_unlock (&jmp_mutex);		\
	pthread_mutex_lock   (&jmp_mutex);		\
        pthread_sigmask (SIG_UNBLOCK, &set, NULL);      \
	ret = alarm (sec);				\
    }							\
    else {						\
    	pthread_sigmask (SIG_BLOCK, &set, NULL);	\
    	pthread_mutex_unlock (&alarm_mutex);		\
    	pthread_mutex_unlock (&jmp_mutex);		\
    }							\
    ret;						\
})


#define pthread_setjmp(env) ({				\
    pthread_mutex_lock(&jmp_mutex);			\
    jmp_owner = pthread_self();				\
    setjmp (env);					\
})


#define pthread_longjmp(env,val) ({			\
    if ( pthread_equal( pthread_self(), jmp_owner )) 	\
    longjmp (env, val);			  		\
})


#define pthread_ignore_alarm() do {			\
    sigset_t set;					\
    sigemptyset (&set);					\
    sigaddset (&set, SIGALRM);				\
    pthread_sigmask (SIG_BLOCK, &set, NULL);		\
} while (0)


/* example code */

jmp_buf local;

void
handler (int c)
{
    printf ("->SIGALRM\n");
    pthread_longjmp (local, 1);

}

int
pthread_c ()
{
    pthread_ignore_alarm();

    printf ("pth_c::start\n");
    sleep  (6);
    printf ("pth_c::exit\n");
   
    return 0;  
}  

int
pthread_b ()
{
    pthread_ignore_alarm(); // <- threads ignore SIGALRM 

    printf ("pth_b::start\n");

    if (!pthread_setjmp (local)) {
	printf("pth_b::pthread_alarm(4)\n");	

	pthread_alarm (4);    // <- set the alarm for this thread 
	sleep (69);
	pthread_alarm (0);    // <- remove alarm  

    }
    else {
	pthread_alarm (0);    // <- alarm needs to be removed even when expired 
	printf ("pth_b:: longjmp(TIMEOUT)! (ok)\n");
    }

    printf ("pth_b::exit\n");

    return 0;
}

int
pthread_a ()
{
    pthread_ignore_alarm();

    printf ("pth_a::start\n");
    printf ("pth_a::pthread_alarm(2)\n");

    pthread_alarm (2);
    sleep (69);
    pthread_alarm (0);

    printf ("pth_a::exit\n");
    
    return 0;
}

pthread_t pth_a;
pthread_t pth_b;
pthread_t pth_c;

int
main (int argc, char **argv)
{

    signal (SIGALRM, handler);

    pthread_mutex_init (&alarm_mutex, NULL);
    pthread_mutex_init (&jmp_mutex, NULL);

    pthread_create (&pth_a, NULL, (void *) pthread_a, NULL);
    pthread_create (&pth_b, NULL, (void *) pthread_b, NULL);
    pthread_create (&pth_c, NULL, (void *) pthread_c, NULL);


    pthread_join (pth_a, NULL);
    pthread_join (pth_b, NULL);
    pthread_join (pth_c, NULL);

    printf ("done.\n");

    return 0;
}
