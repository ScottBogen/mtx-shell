/*  		cat.c file 

    Implementation of UNIX cmd `cat`

    EX:
	Read from file:   $ cat <filename>
	Read from STDIN:  $ cat 		
	Read from STDOUT: $ <filename> > cat

*/

#include "ucode.c"

char buf[24];
char string[128];
char tty[32];

int main(int argc, char *argv[ ]) {

  int n;
  int in, out;
  int i;

  gettty(tty);
  int ttyfd = open(tty, O_WRONLY);

  // STDIN
  if (argc == 1) {
    in = 0;
    out = 1;
  }

  // FILE
  else {
    in = open(argv[1], O_RDONLY);
    out = 1;
  }

  i = 0;

  
  while(1) {
    n = read(in, buf, 1);      	  // read 1 byte from input
    if (n < 1) { break; }         // if no data, break
    string[i] = buf[0];

    if (!in) { write(ttyfd, buf, 1); }    // print char, meant for "cat" with no args


    // if FILE has a newline
    if (string[i] == '\n') { 
      i++;
      string[i] = '\r';          // force line to end in \n\r

      write(out, string, i);
      write(ttyfd, "\r", 1);     // to bring the cursor back without filling up string

      memset(string, 0, 128);
      i = 0;
    }

    // STDIN user presses "enter"
    else if (string[i] == '\r' && !in) {
      string[i++] = '\n';
      string[i++] = '\r';
      write(ttyfd, "\n\r", 2);     // to bring the cursor back without filling up string
      write(out, string, i);
      memset(string, 0, 128);
      i = 0;
    }

    else {
      i++;
    }
  }

  close(in); close(out); close(ttyfd);
}
