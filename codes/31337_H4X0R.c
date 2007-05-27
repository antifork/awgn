/*
 * 31337 H4X0R SHELLCODE. 
 *
 * Linux.
 * OpenBSD.
 * FreeBSD. 
 * NetBSD ( maybe )
 *
 *  Copyright (c) 2001 Nicola Bonelli <bonelli@antifork.org>
 *
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 *
 *
__asm__("
        xor %eax , %eax
        xor %ebx , %ebx
        xor %ecx , %ecx
        xor %edx , %edx

        movb $0x4 , %al
        movb $0x1 , %bl
        movb $0xd6, %dl

        jmp callb

next:   popl %ecx
        push %edx
        push %ecx
        push %ebx
        push %ebx * garbage * 

        int $0x80

        xor %eax, %eax
        xor %ebx, %ebx
        movb $0x1, %al
        movb $0x1, %bl

        pushl %ebx
        pushl %ebx * garbage *
        int $0x80

callb:  call next
.string \"  _ __ ___   ___  __| (_) ___  _ __ ___   __ _ _ __ \n | '_ ` _ \\ / _ \\/ _` | |/ _ \\| '_ ` _ \\ / 
_` | '_ \\\n | | | | | |  __/ (_| | | (_) | | | | | | (_| | | | |\n |_| |_| |_|\\___|\\__,_|_|\\___/|_| |_| |_|
\\__,_|_| |_|\n\"");

 *
 *
 *
 *
 *
 *
 */

char sc[]= "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x04\xb3\x01\xb2\xd6\xeb\x13"
	   "\x59\x52\x51\x53\x53\xcd\x80"
	   "\x31\xc0\x31\xdb\xb0\x01\xb3\x01\x53\x53\xcd\x80"
           "\xe8\xe8\xff\xff\xff"
	   "\x20\x20\x5f\x20\x5f\x5f\x20\x5f\x5f\x5f\x20\x20\x20\x5f\x5f\x5f\x20\x20\x5f\x5f\x7c\x20\x28\x5f\x29\x20\x5f\x5f\x5f\x20\x20\x5f\x20\x5f\x5f\x20\x5f\x5f\x5f\x20\x20\x20\x5f\x5f\x20\x5f\x20\x5f\x20\x5f\x5f\x20\x0a\x20\x7c\x20\x27\x5f\x20\x60\x20\x5f\x20\x5c\x20\x2f\x20\x5f\x20\x5c\x2f\x20\x5f\x60\x20\x7c\x20\x7c\x2f\x20\x5f\x20\x5c\x7c\x20\x27\x5f\x20\x60\x20\x5f\x20\x5c\x20\x2f\x20\x5f\x60\x20\x7c\x20\x27\x5f\x20\x5c\x0a\x20\x7c\x20\x7c\x20\x7c\x20\x7c\x20\x7c\x20\x7c\x20\x20\x5f\x5f\x2f\x20\x28\x5f\x7c\x20\x7c\x20\x7c\x20\x28\x5f\x29\x20\x7c\x20\x7c\x20\x7c\x20\x7c\x20\x7c\x20\x7c\x20\x28\x5f\x7c\x20\x7c\x20\x7c\x20\x7c\x20\x7c\x0a\x20\x7c\x5f\x7c\x20\x7c\x5f\x7c\x20\x7c\x5f\x7c\x5c\x5f\x5f\x5f\x7c\x5c\x5f\x5f\x2c\x5f\x7c\x5f\x7c\x5c\x5f\x5f\x5f\x2f\x7c\x5f\x7c\x20\x7c\x5f\x7c\x20\x7c\x5f\x7c\x5c\x5f\x5f\x2c\x5f\x7c\x5f\x7c\x20\x7c\x5f\x7c\x0a";


main() {
   long *ret;

   ret = (long *)&ret + 2;
   (*ret) = (long)sc;

}

