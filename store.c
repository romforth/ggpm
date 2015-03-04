/* Pack a stream of data into a C string.
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
#include "store.h"

int store(int c, StoreState *ss) {
    if (ss->col <= 0) {
        if (putc('"', ss->out) < 0)
            return -1;
        ss->col += 1;
    }
    if (c < ' ') {
        ss->col += 2;
        if (putc('\\', ss->out) < 0)
            return -1;
        if (c == '\r') {
            if (putc('r', ss->out) < 0)
                return -1;
        } else if (c == '\n') {
            if (putc('n', ss->out) < 0)
                return -1;
        } else if (c == '\t') {
            if (putc('t', ss->out) < 0)
                return -1;
        } else {
            if (putc('0' + ((c >> 6) & 3), ss->out) < 0 ||
                putc('0' + ((c >> 3) & 7), ss->out) < 0 ||
                putc('0' + (c & 7), ss->out) < 0)
                  return -1;
            ss->col += 2;
        }
    } else {
        if (c == '"' || c == '\\') {
           if (putc('\\', ss->out) < 0)
               return -1;
           ss->col += 1;
        }
        if (putc(c, ss->out) < 0)
            return -1;
        ss->col += 1;
    }
    if (ss->col > LINEWIDTH - 2) {
        if (putc('"', ss->out) < 0 ||
            putc('\n', ss->out) < 0)
              return -1;
        ss->col = 0;
    }
    ss->last = c;
    return 0;
}

int finish(StoreState *ss) {
    if (ss->col)
        if (putc('"', ss->out) < 0)
            return -1;
    return 0;
}
