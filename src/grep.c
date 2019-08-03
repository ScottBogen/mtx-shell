/**************** grep.c file ******************

	This file implements UNIX `grep`
	

***********************************************/
#include "ucode.c"

char buf[1024];
char tty[32];

int main(int argc, char *argv[ ]) {

  prints("ENTERED SCOTT'S GREP\n\r");

  int i, j, n, length;
  int in, out;
  char* pattern;
  char line[128];
  char c;

  gettty(tty);

  int outtty = open(tty, O_WRONLY);

  if (argc < 2) {
    prints("Not enough args\n\r");
    return -1;
  }

  // STDIN
  if (argc == 2) {
    pattern = argv[1];
    in = 0;
    out = 1;
  }

  // FILE
  else {
    pattern = argv[1];
    in = open(argv[2], O_RDONLY);
    if (in < 1) {
      printf("Bad file: %s\n\r", argv[2]);
      return -1;
    }
    out = 1;
  }

  // make sure pattern is acceptable
  if (!pattern) { return -1; }

  // find pattern length
  length = 0;
  i = 0;
  while (pattern[length] != '\0') {
    length++;
  }


  // main loop
  i = j = 0;
  int to_print = 0;
  memset(line, 0, 128);		// force-empty line
  while (1) {
    n = read(in, buf, 1);      	  // read in from buf
    if (n < 1) { break; }         // leave if no more bytes

    line[i] = buf[0];

    if (buf[0] == '\n') {
      line[++i] = '\r';       	    // Force '\n\r' to bring back cursor
      if (to_print) {
        write(out, line, i);
        write(outtty, "\r", 1);     // prevents filling string

      }
      to_print = 0;
      i = j = 0;
      memset(line, 0, 128);
    }

    else {
      if (line[i] == pattern[j]) {
        j++;
        if (j == length && !to_print) {
          to_print = 1;
        }
      }
      else {
        j = 0;
      }
      i++;
    }
  }
  close(in); close(out);
}
