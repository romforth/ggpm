/* Remove comments and spaces from C code and pack it into a C string.
   Copyright (C) 2014-2015 P. Cherepanov and E. Batyrova.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 3
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, if not, see <http://www.gnu.org/licenses/>.
*/

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "store.h"

#define cases(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z) \
    case a:case b:case c:case d:case e:case f:case g:case h:\
    case i:case j:case k:case l:case m:case n:case o:case p:\
    case q:case r:case s:case t:case u:case v:case w:case x:\
    case y:case z:

static void error(int rc, const char *msg) {
    fprintf(stderr, "%s\n", msg);
        exit(rc);
}

static int is_namechar(int c) {
    return isalnum(c) || c == '_';
}

// String continuation is ignored because it's not used in GPM source.
int main (int argc, char **argv) {
    int c, prev = 0, prev_prev=0;
    int column = 0, last_stored = 0;
    char buf[300];
    enum {kComment, kCPPcomment, kString, kChar, kSpace, kProgram} state = kProgram;
    StoreState ss = {0, 0, 0};
    FILE *input;

    if (argc != 3)
        error(1, "compact INPUT OUTPUT\n");
    input = fopen(argv[1],"rb");
    if (input == 0)
        error(1, "Cannot open input file");

    ss.out = fopen(argv[2],"wb");
    if (ss.out == 0)
        error(1, "Cannot open output file");
    while(fscanf(input,"%[^\n]\n", buf) == 1) {
        if(!strncmp(buf,"// Start GPM code", 17))
            goto has_gpm;
    }
    error(1,"Cannot find \"// Start GPM code\"");
  has_gpm:
    fputs("char ggpm_source[]=\n", ss.out);
    goto Program;
  Comment:
     prev = 0;
     while((c=getc(input)) != EOF) {
         if (c == '/' && prev == '*')
             goto Space;
         prev = c;
     }
     goto End;
 CPPcomment:
     while((c=getc(input)) != EOF) {
         if (c == '\n')
             goto Program;
     }
     goto End;
 String:
     prev = prev_prev = 0;
     while(!((c=getc(input)) == EOF || (c == '"' && (prev != '\\' ||  prev_prev == '\\')))) {
         prev_prev = prev;
         prev = c;
         store(c, &ss);
     }
     store('"', &ss);
     if (c == EOF)
         goto End;
     goto Program;
 Char:
     prev = prev_prev = 0;
     while(!((c=getc(input)) == EOF || (c == '\'' && (prev != '\\' ||  prev_prev == '\\')))) {
         prev_prev = prev;
         prev = c;
         store(c, &ss);
     }
     store('\'', &ss);
     if (c == EOF)
         goto End;
     goto Program;
 Space:
     while((c=getc(input)) != EOF && isspace(c))
         ;
     ungetc(c, input);
     if (is_namechar(ss.last) && is_namechar(c))
         store(' ', &ss);
     /* fall through */
 Program:
     while((c=getc(input)) != EOF) {
         switch(c) {
         cases('a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z')
         cases('A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z')
         case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '_':
             store(c, &ss);
             break;
         case ' ':case '\t':case '\r':case '\n':
             goto Space;
         case '/': {
             int c2 = getc(input);
             if (c2 == '/') {
                 goto CPPcomment;
             } else if (c2 == '*') {
                 goto Comment;
             } else {
                 ungetc(c2, input);
                 store(c, &ss);
             }
             break;
         }
         case '\'':
             store(c, &ss);
             goto Char;
         case '"':
             store(c, &ss);
             goto String;
         case '#':
             store('#', &ss);
             while((c=getc(input)) != EOF && c != '\n')
                 store(c, &ss);
             store('\n', &ss);
             goto Space;
         default:
             store(c, &ss);
         }
     }
     /* fall through */
 End:
    finish(&ss);
    fputs(";\nint ggpm_source_len = sizeof(ggpm_source) - 1;\n", ss.out);

    if (input != stdin)
          fclose(input);
    if (ss.out != stdout)
          fclose(ss.out);
    return 0;
}
