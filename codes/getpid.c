/*
 * Copyright (c) 2007 Nicola Bonelli <bonelli@antifork.org>
 *
 * All rights reserved.
 *
 * Tested on some linux flavors
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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

static int
pid_filter(const struct dirent *d)
{
        if (atoi(d->d_name))
                return 1;
        return 0;
}


pid_t
get_pid(const char *cmd, pid_t start) 
{
        char status[80];
        FILE *f;

        pid_t pid = -1;
        struct dirent **filelist;
        int i, n;
        n = scandir("/proc", &filelist,pid_filter , alphasort);
        for(i=0; i<n; i++) {
                pid_t p = atoi(filelist[i]->d_name);
                if ( p <= start )
                        continue;
                if ( sprintf(status,"/proc/%d/status", p) < 0 )
                        continue;
                f = fopen(status,"r");
                if ( f == NULL )
                        continue;
                        
                char *line = NULL ; size_t size;
                if ( getline(&line, &size, f) < 0 )
                        goto next;

                if ( sscanf(line,"Name: %s", status) != 1 )
                        goto next;

                if (!strcmp(status,cmd)) {
                        fclose(f);
                        pid = p;
                        break;
                }
        next:
                fclose(f);
        }

        // free filelist...
        for(i=0; i<n; i++) {
                free(filelist[i]);
        }
        return pid; 
}
        

int
main (int argc, char *argv[])
{
        pid_t bp = 0;

        while( (bp=get_pid("bash",bp)) != -1 )
                printf("%d\n",bp);

        return 0;
}
