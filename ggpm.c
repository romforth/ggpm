/* Re-implementation of a classic macro processor GPM.
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
#include <memory.h>
#include <getopt.h>

#include "usage.h"

void GPM(int);

int debug_flags = 0;
int stack_size = 1000000;
char *output_fname = 0;
FILE *input, *output;

int main (int argc, char **argv) {
    int c;

static struct option long_options[] = {
    {"help",    no_argument,       0, 0},
    {"version", no_argument,       0, 1},
    {"out",     required_argument, 0, 'o'},
    {"stack",   required_argument, 0, 's'},
    {0, 0, 0, 0}
};

    int option_index = 0;

    while((c = getopt_long (argc, argv, "s:", long_options, &option_index)) != -1) {

        switch (c) {
          case 0: /* help */
            usage();
            exit(0);

          case 1: /* version */
            version();
            exit(0);

          case 's':
            if(sscanf(optarg, "%d", &stack_size) != 1 || stack_size < 1000) {
            	fprintf(stderr, "Incorrect -s (--stack) value = %s\n", optarg);
            	exit(1);
            }
            break;

          case 'o':
        	output_fname = optarg;
            break;

          case '?':
            /* getopt_long() already printed an error message.  */
            break;

          default:
            usage();
            exit(1);
	}
    }

  if (optind + 2 < argc) {
	  fprintf(stderr, "Too many arguments\n");
      exit(1);
  }
  input = stdin;
  output = stdout;
  if (optind < argc)
    {
      input = fopen(argv[optind],"rb");
      if (input == 0)
    	  error(1, "Cannot open input file");
      optind++;
    }
  if (optind < argc)
    {
      output = fopen(argv[optind],"rb");
      if (output == 0)
    	  error(1, "Cannot open output file");
      optind++;
    }
  GPM(stack_size);
  if (input != stdin)
	  fclose(input);
  if (output != stdout)
	  fclose(output);
  return 0;
}

#define MARKER 0x80000000

int A, W;
int H=0, P=0, F=0, C=0;
int  S=39, E=33, q=1;
int *ST;
const int MST[] = { -1, 4, 'D','E','F',-1,
              0,4,'V','A','L',-2,
              6, 7, 'U','P','D','A','T','E',-3,
              12,4,'B','I','N',-4,
              21,4,'D','E','C',-5,
              27,4,'B','A','R',-6};


static void NextCh(int *ST, int *A, int *C) {
	if (*C == 0)
          *A = getc(input);
    else {
        *A = ST[*C];
        *C = *C+1;
    }
}

static void Load(int *ST, int *S, int A, int H) {
	if (H == 0) {
        putc(A, output);
	} else {
      ST[*S] = A;
      *S = *S + 1;
    }
}

static int Find(int *ST, int *A, int *W, int E, int x) {
	int r;
	*A = E;
    *W = x;
     do {
  	     for (r = 0; r < ST[*W]; r++) {
              if (ST[*W+r] != ST[*A+r+1])
                   goto Next;
  	     }
         *W = *A + 1 + ST[*W];
         return 1;
       Next:
         *A = ST[*A];
     } while(*A >= 0);
     return 0;
}

// This routine outputs the item on the stack starting at ST[x]
// if the item is not complete, printing stops at ST[S-1] and
// is follows by '...(Incomplete)'.
static void Item(int *ST, int *S, int x) {
    int k, n = ST[x]==0 ? *S-x-1 : ST[x]-1;
    for (k = 1; k <= n; k++)
        putc(ST[x+k], stderr);

    if (ST[x] == 0)
        fprintf(stderr, "...\t(Incomplete)");
}


// CPL program for GPM
void GPM(int n) {

// n is the stack size allowed. This should be as large as possible --
// say 10000.
    int r;
    ST = (int *)malloc((n+1)*sizeof(*ST));
    memcpy(ST, MST, sizeof(MST));

// The name-value pairs for the six machine code macros are first assembled
// in the vector MST and then copied into the base of the stack.

// This routine depends on the method of marking machine code macros.
// The method adopted here (which is different from that described in
// the paper or used in the actual Titan program) is to make the value
// a negative index integer which is used to index the label vector
// MachineMacro, whose entries

/// and Number[x] = x-16
/// and Char[x] = x+16

// These are implementation-dependent functions. They convert the index
// equivalent of a decimal digit read in with ReadSymbol to the corresponding
// number (also of type index) and vice versa.

// Main cycle.

Start:
		  NextCh(ST, &A, &C);
		  switch(A) {
		  case '<': q = q+1; goto Q2;
		  case '$': goto Fn;
		  case ',': goto NextItem;
		  case ';': goto Apply;
		  case '~': goto LoadArg;
		  case MARKER: goto EndFn;
		  case '>': goto Exit;
		  case EOF:
	         return;
		  }

Copy:     Load(ST, &S, A, H);
Scan:     if (q == 1)
	          goto Start;
  Q2:     NextCh(ST, &A, &C);
          if (A == EOF) {
              fprintf(stderr, "Unexpected EOF in quoted string.\n");
              return;
          } else if (A == '<') {
              q++;
              goto Copy;
          } else if(A == '>') {
              q--;
              if (q == 1)
                  goto Start;
              else
                  goto Copy;
          } else
        	  goto Copy;
// Warning Character Actions
Fn:
		  ST[S] = H;
		  ST[S+1] = F;
		  ST[S+2] = 0;
		  ST[S+3] = 0;
		  H = S+3;
		  F = S+1;
		  S = S+4;
		  goto Start;
NextItem: if (H == 0)
	          goto Copy;
          ST[H] = S - H - ST[H];
          ST[S] = 0;
          H = S;
          S = S+1;
          goto Start;
Apply:    if (P > F)
	          goto Monitor1;
          if (H==0)
        	  goto Copy;
          {
            int f0 = ST[F];
            int h0 = ST[F-1];
        	ST[H] = S - H;
            ST[S] = MARKER;
            ST[F-1] = S - F + 2;
            ST[F] = P;
            ST[F+1] = C;
            P = F;
            H = h0;
            S = S + 1;
            F = f0;
          }
          if (H != 0)
             ST[H] = ST[H] + ST[P-1];
          if (!Find(ST, &A, &W, E, P+2))
        	  goto Monitor7;

          if (ST[W] < 0) {
        	  switch(-ST[W]) {
        	  case 1: goto DEF;
        	  case 2: goto VAL;
        	  case 3: goto UPDATE;
        	  case 4: goto BIN;
        	  case 5: goto DEC;
        	  case 6: goto BAR;
        	  }
          }
          C = W + 1;
          goto Start;

LoadArg:  if (P==0) {
	        if (H==0)
	        	goto Copy;
	        else
	        	goto Monitor2;
          }
          NextCh(ST, &A, &C);
          W = P + 2;
          if (A - '0' < 0)
        	  goto Monitor3;
          for (r = 0; r < A - '0'; r++) {
              W = W + ST[W];
              if (ST[W] == MARKER)
            	  goto Monitor4;
          }
          for (r = 1; r < ST[W]; r++) {
              A = ST[W+r];
              Load(ST, &S, A, H);
          }
          goto Start;

EndFn:    if (F > P)
	          goto Monitor5;
          ST[S]= E;
          A = S;
          while (ST[A] >= P - 1 + ST[P-1]) {
              int a = A;
              A = ST[a];
        	  ST[a] = ST[a] - ST[P-1];
          }
          W = ST[A];
          while (W > P-1)
              W = ST[W];
          ST[A] = W;
          E = ST[S];
          if (H!=0) {
              if (H > P)
                  H = H - ST[P-1];
              else
                  ST[H] = ST[H] - ST[P-1];
          }
          C = ST[P+1];
          S = S - ST[P-1];
          A = P - 1;
          W = P - 1 + ST[P-1];
          P = ST[P];

          while (A != S)
              ST[A++] = ST[W++];
          goto Start;

  Exit:   if (C!=0 || H!=0)
              goto Monitor8;
          return;

// Machine code macros
DEF:      if (H != 0) {
	          ST[H] = ST[H] - ST[P-1] + 6;
          }
	      ST[P-1] = 6;
	      ST[P+5] = E;
	      E =  P + 5;
          goto EndFn;

// This version of DEF is shorter than that given in
// Section 7 as it leaves EndFn to copy back the definition.
VAL:      if(!Find(ST, &A, &W, E, P+6))
	          goto Monitor7;
          while (ST[W+1] != MARKER) {
              A = ST[W+1];
              W = W+1;
              Load(ST, &S, A, H);
          }
          goto EndFn;
UPDATE:   if (!Find(ST, &A, &W, E, P+6))
	          goto Monitor7;
          A = P + 9 + ST[P+9];
          if (ST[A] > ST[W])
        	  goto Monitor9;
          for (r=1; r <= ST[A]; r++)
              ST[W+r] = ST[A+r];
          goto EndFn;
BIN:      W = 0;
          if (ST[P+7] == '+' || ST[P+7] == '-')
        	  A = P+8;
          else
        	  A = P+7;
          while (ST[A] != MARKER) {
        	  int x = ST[A]-'0';
        	  if (x < 0 || x > 9)
        		  goto Monitor9;
              W = 10*W + x;
              A += 1;
          }
          ST[S] = ST[P+7] == '-' ? -W : W;
          S += 1;
          goto EndFn;
DEC:      W = ST[P+7];
          if (W < 0) {
              W = -W;
              A = '-';
              Load(ST, &S, A, H);
          }
          { int W1 = 1;
              while (10*W1 <= W)
            	  W1 = 10*W1;
              do {
              	  A = W/W1 + '0';
                  W = W % W1;
                  W1 = W1/10;
                  Load(ST, &S, A, H);
              } while(W1 >= 1);
          }
          goto EndFn;

BAR:      W = ST[P+9];
          A = ST[P+11];
          switch(ST[P+7]) {
          case '+':
        	  A = W + A; break;
          case '-':
        	  A = W - A; break;
          case '*':
        	  A = W * A; break;
          case '/':
        	  A = W / A; break;
          default:
        	  A = W % A; break;
          }
          Load(ST, &S, A, H);
          goto EndFn;

// Monitor for errors

// Monitor Entries and Effects

Monitor1: // Unmatched ; in definition string. Treated as <;>
          fprintf(stderr, "\nMONITOR: Unmatched semicolon in definition of ");
          Item(ST, &S, P+2);
          printf("\nIf this has been quoted the result would be \n");
          goto Copy;
Monitor2: // Unquoted ~ in argument list in input stream. Treat as <~>
          fprintf(stderr, "\nMONITOR: Unquoted tilde in argument list of ");
          Item(ST, &S, F+2);
          printf("\nIf this had been quoted the result would be \n");
          goto Copy;
Monitor3: // Impossible character (negative) as argument number. Terminate.
          fprintf(stderr, "\nMONITOR: impossible argument number in definition of ");
          Item(ST, &S, P+2);
          goto Monitor11;
Monitor4: // Not enough argument supplied in call. Terminate.
          fprintf(stderr, "\nMONITOR: No argument ");
          H = 0;
          putc(A, stderr);
          printf("in call for ");
          Item(ST, &S, P+2);
          goto Monitor11;
Monitor5: // Terminator in impossible place; if C=0, this is the input stream.
          // Probably machine error: Terminate. If C != 0, this is an argument
          // list. Probably due to a missing semicolon: Final semicolon inserted.
          fprintf(stderr, "\nMONITOR: Terminator in ");
          if (C==0) {
              fprintf(stderr, "input stream. Probably machine error.");
              goto Monitor11;
          }
          fprintf(stderr, "argument list for ");
          Item(ST, &S, F+2);
          fprintf(stderr, "\nProbably due to a semicolon missing from the definition of ");
          Item(ST, &S, P+2);
          fprintf(stderr, "If a final semicolon is added the result is\n");
          C = C - 1;
          goto Apply;
Monitor7: // Undefined macro name. Terminate
          fprintf(stderr, "\nMONITOR: Undefined name ");
          Item(ST, &S, W);
          goto Monitor11;
Monitor8: // Wrong exit (not C=H=0). Machine error: Terminate.
          fprintf(stderr, "\nMONITOR: Unmatched >. Probably machine error.");
          goto Monitor11;
Monitor9: // Update string too long. Terminate.
          fprintf(stderr, "\nMONITOR: Update argument too long for ");
          Item(ST, &S, P+9);
          goto Monitor11;
Monitor10: // Non-digit in argument for BIN. Terminate
          fprintf(stderr, "\nMONITOR: Non-digit in number");
          goto Monitor11;
Monitor11: // General monitor after irremediable errors.
          W = 20;
          fprintf(stderr, "\nCurrent macros are ");
          while (P != 0 && F != 0) {
              int r, W1;
              if (P > F) {
            	  W1 = P + 2;
            	  P = ST[P];
                  fprintf(stderr, "\nAlready entered ");
              } else {
                   W1 = F + 2;
                   F = ST[F];
                   fprintf(stderr, "\nNot yet entered ");
              }
              for (r=1; r <= W; r++) {
                  Item(ST, &S, W1);
                  if (ST[W1] == 0)
                      break;
                  W1 = W1 + ST[W1];
                  if (ST[W1] == MARKER)
                	  break;
                  if (W!=1)
                      fprintf(stderr, "\nArg %d\t",r);
              }
              W = 1;
          }
          fprintf(stderr, "\nEnd of monitor printing\n");
          A = 'Q';
          Load(ST, &S, A, H);
          if (P > F)
        	  goto EndFn;
          else
              goto Start;
// End CPL program for GPM.
}
