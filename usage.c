/* usage() and version() functions moved out of the way.
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

#include <stdio.h>
#include <stdlib.h>
#include "usage.h"

void version(void) {
    printf("1.0\n");
}

void usage(void) {
  printf(
"Usage: ggpm [OPTION]... [INPUT_FILE] [OUTPUT_FILE]\n"
"Process macros in FILEs.  If no FILE or if FILE is `-', standard input\n"
"or standard output is used.\n"
"\n"
"Mandatory or optional arguments to long options are mandatory or optional\n"
"for short options too.\n"
"\n"
"Operation modes:\n"
"      --help                   display this help and exit\n"
"      --version                output version information and exit\n"
"\n"
"Exit status is 0 for success, 1 for failure.\n"
"\n"
"Report bugs to: sphinx.pinastri@gmail.org\n"
"ggpm home page: <https://code.google.com/p/ggpm/>\n"
);
}

