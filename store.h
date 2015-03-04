#ifndef INCL_STORE_H
#define INCL_STORE_H
/* Pack a stream of bytes into C string.
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

#define LINEWIDTH 80

typedef struct _StoreState {
    int col;
    int last;
    FILE *out;
} StoreState;

int store(int c, StoreState *ss);
int finish(StoreState *ss);

#endif
