/* usage() and version() functions moved out of the way.
   Copyright (C) 2014 P. Cherepanov and E. Batyrova.

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

#include <stdio.h>
#include <stdlib.h>
#include "usage.h"

void version(void) {
    printf("1.0\n");
}

void usage(void) {
  printf(
"Usage: ggpm [OPTION]... [FILE]...\n"
"Process macros in FILEs.  If no FILE or if FILE is `-', standard input\n"
"is read.\n"
"\n"
"Mandatory or optional arguments to long options are mandatory or optional\n"
"for short options too.\n"
"\n"
"Operation modes:\n"
"      --help                   display this help and exit\n"
"      --version                output version information and exit\n"
"  -E, --fatal-warnings         once: warnings become errors, twice: stop\n"
"                                 execution at first error\n"
"  -i, --interactive            unbuffer output, ignore interrupts\n"
"  -P, --prefix-builtins        force a `m4_' prefix to all builtins\n"
"  -Q, --quiet, --silent        suppress some warnings for builtins\n"
"\n"
"Preprocessor features:\n"
"  -D, --define=NAME[=VALUE]    define NAME as having VALUE, or empty\n"
"  -I, --include=DIRECTORY      append DIRECTORY to include path\n"
"  -s, --synclines              generate `#line NUM \"FILE\"' lines\n"
"  -U, --undefine=NAME          undefine NAME\n"
"\n"
"Limits control:\n"
"  -g, --gnu                    override -G to re-enable GNU extensions\n"
"  -G, --traditional            suppress all GNU extensions\n"
"  -H, --hashsize=PRIME         set symbol lookup hash table size [509]\n"
"  -L, --nesting-limit=NUMBER   change nesting limit, 0 for unlimited [0]\n"
"\n"
"Debugging:\n"
"  -d, --debug[=FLAGS]          set debug level (no FLAGS implies `aeq')\n"
"      --debugfile[=FILE]       redirect debug and trace output to FILE\n"
"                                 (default stderr, discard if empty string)\n"
"  -l, --arglength=NUM          restrict macro tracing size\n"
"  -t, --trace=NAME             trace NAME when it is defined\n"
"\n"
"FLAGS is any of:\n"
"  a   show actual arguments\n"
"  c   show before collect, after collect and after call\n"
"  e   show expansion\n"
"  f   say current input file name\n"
"  i   show changes in input files\n"
"  l   say current input line number\n"
"  p   show results of path searches\n"
"  q   quote values as necessary, with a or e flag\n"
"  t   trace for all macro calls, not only traceon'ed\n"
"  x   add a unique macro call id, useful with c flag\n"
"  V   shorthand for all of the above flags\n"
"\n"
"Exit status is 0 for success, 1 for failure.\n"
"\n"
"Report bugs to: sphinx.pinastri@gmail.org\n"
"ggpm home page: <http://www.gnu.org/software/m4/>\n"
);
}

