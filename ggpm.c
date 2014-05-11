/* Re-implementation of a classic macro processor GPM.
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
#include <getopt.h>
#include "usage.h"

static int opt_debug = 0;
int debug_flags = 0;

int main (int argc, char **argv) {
    int c;

    static struct option long_options[] =
      { {"help",    no_argument,       0, 0},
	{"version", no_argument,       0, 1},
	{"out",     required_argument, 0, 'o'},
	{"debug",   required_argument, &debug_flags, 'd'},
	{0, 0, 0, 0}
      };

    int option_index = 0;

    while((c = getopt_long (argc, argv, "abc:d:f:", long_options, &option_index)) != -1) {

        switch (c) {
          case 0: /* help */
            usage();
            exit(0);

          case 1: /* version */
            version();
            exit(0);

          case 'a':
            puts ("option -a\n");
            break;
          case 'b':
            puts ("option -b\n");
            break;

          case 'c':
            printf ("option -c with value `%s'\n", optarg);
            break;

          case 'd':
            printf ("option -d with value `%s'\n", optarg);
            break;

          case 'f':
            printf ("option -f with value `%s'\n", optarg);
            break;

          case '?':
            /* getopt_long() already printed an error message.  */
            break;

          default:
            usage();
            exit(1);
	}
    }
  /* Print any remaining command line arguments (not options).  */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
	printf ("%s ", argv[optind++]);
      putchar ('\n');
    }
  exit (0);
}
