# MTX Shell

MTX Shell provides a command line interface for the MTX operating system, originally written by KC Wang of Washington State University. 


## Installation

Install [qemu](https://www.qemu.org/download/) on your system to load and run the image file.


```bash
     sudo apt-get install bcc
     sudo apt-get install qemu-system-i386
```

## Usage

Execute the image file in qemu with the provided executable.

```bash
foo@bar:~$ ./run
```
Alternatively, you can configure qemu directly. 

```bash
foo@bar:~$ qemu-system-arm -M versatilepb -m 256M -sd src/sdimage -kernel src/wanix \
-serial mon:stdio -serial /dev/pts/0
```

If you wish to use a serial port instead of qemu, change `-serial /dev/pts/[PID]` to the PID number of your preferred port.

### Initialization

Upon loading the kernel, process 0 (P0) will handcraft a new process P1, which will run INIT (init.c) indefinitely. This will fork processes to handle login on multiple ports.

### Login

Login (login.c) will request a username and password. Multiple users can be logged in across different serial ports.

A working sample login:

```bash
username: root
pass:     12345
```

A list of logins is found within `/etc/passwd`.

Upon success, it will change directory to the user's preferred home directory.

### Shell

The shell (sh.c) controls the shell terminal on each port. The goal of shell is to provide easy access to the filesystem by abstracting syscalls to the kernel. 

Available functionality within shell:

* UNIX-like commands: `ls`, `grep`, `cat`, `more`, `mkdir`, `rmdir`, `logout`, `l2u`

* Piping: `cat | grep`

* IO Redirections: `>`, `>>`, `<`

### Example calls

Some example calls may include:

The `ls` command:

``` bash
scsh $ ls
drwxr-xr-x  10   0   0   1024       .
drwxr-xr-x  10   0   0   1024       ..
-rwx------  2   0   0   12288       lost+found
drwxr-xr-x  2   0   0   1024       bin
drwxr-xr-x  2   0   0   1024       boot
drwxr-xr-x  2   0   0   1024       dev
drwxr-xr-x  2   0   0   1024       etc
.....
```
The `cat` command:

``` bash
scsh $ cat f2
This is a small file 
with a few lines for testing
cp, mv, lpr, < .....
Happy testing!

scsh $ cat f2 | more
```

File redirection:

```bash
scsh $ ls >> outputfile
scsh $ cat outputfile
drwxr-xr-x  10   0   0   1024       .
drwxr-xr-x  10   0   0   1024       ..
-rwx------  2   0   0   12288       lost+found
drwxr-xr-x  2   0   0   1024       bin
drwxr-xr-x  2   0   0   1024       boot
drwxr-xr-x  2   0   0   1024       dev
drwxr-xr-x  2   0   0   1024       etc
.....
```

## License

<http://www.gnu.org/licenses/>
