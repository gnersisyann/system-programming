Note: The final solutions should contain a Makefile for the multi-stage build. 
The Makefile should also contain targets all and clean. It’s recommended to have compiler and compiler flags to be declared as a Makefile variables. 
Alternatively, cmake could also be used instead of make.

ENV-1: Setup GNU/Linux Environment
Requirements
Install any GNU/Linux distribution on a Virtual or Physical Machine.
Run lsb_release -a >> $USER.txt command (if available) in a shell and get the result.
Run cat /etc/os-release >> $USER.txt command (if available) in a shell and get the result.
Submit $USER.txt file to the Github as a solution for the task.
Expected output
The expected output is a text file named $USER.txt where $USER is the current user’s username. The content is expected to be similar to the following:

Distributor ID: Distribution Name
Description:    Ubuntu 20.04.3 LTS
..... omitted 
NAME="Ubuntu"
VERSION="20.04.3 LTS (Focal Fossa)"
ID=ubuntu
..... omitted
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
VERSION_CODENAME=focal
UBUNTU_CODENAME=focal

ENV-2: Complex numbers and sorting
Requirements
Write multi-file C++ program consisting of following 3 modules:

A class that represents a complex number along with all the basic operations: plus, minus, multiply by a constant, absolute value.
A function that implements any sorting algorithm on an array/vector of complex numbers.
A main function containing a demonstration of having multiple complex numbers in array and sorting them.
Modules for complex number class and sorting algorithm should be separate and each of them should have source and header files.

Expected result
A solution containing all the described modules (source and header files), as well as, a Makefile for automated build of the solution.

FS-1: Read from the file
Description
You need to implement a program that does the same thing as the cat command in the Linux.

Requirements
Write a C++ program read-file accepting a file path as an argument.
Implement proper error handling to make sure required file path is passed.
Open the given file with proper mode and make sure errors are handled.
Read the content of the file into a buffer until reaching the end of the file.
Print every portion of the file read in the iteration.
Hint: if you use printf or std::cin, make sure your strings are terminated with ‘\0’ symbol.

Expected result
The resulting application should be able to build and execute from command line as follows:

make
./read-file input.txt
The program should output the content of the input.txt file on the screen.

FS-2: Copy a file
Requirements
Write a C++ program simple-copy accepting two command line arguments representing source file path and destination file path.
Implement proper error handling to account if not all arguments are passed or if one of arguments is not a valid one.
Copy the content of the source file into a destination file. If destination file already exists, it should be overwritten.
Expected result
The resulting application should be able to build and execute from command line as follows:

make
./simple-copy source-file destination-file

FS-3: Secure file deletion
Description
Your task is to implement a C++ program that deletes the file from the filesystem along with its content by replacing every byte in the files with ‘\0’ character.

Requirements
Write a C++ program named “hacker-rm” that accepts a path to a file as a command-line argument.
Implement proper error handling to check if the file exists and if the input path is valid.
Open the file for writing and replace every byte with the null character (‘\0’).
Remove the file from the file system after overwriting its content.
Ensure that your program works for various file sizes and types.
Hint: you can use write, stat, lseek, unlink function to solve the problem.

Expected result
The resulting application should be able to build and execute from command line as follows:

make
./hacker-rm file-to-erase

FS-4: Redirect standard input
Description
By default, program gets standard input waiting for the keyboard input. Your task is to make any program to read input from the file instead of waiting for the keyboard input without changing usages of std::cin or scanf.

Requirements
Write a C++ program named “redirect-cin” that accepts a path to a file as a command-line argument.
Implement the function initialize(int argc, char** argv) to start reading from the given file with a proper error handling.
Implement a simple logic for main() function to read string from the standard input (std::cin) and print the reversed string to the cosnsole (std::out).
If there is an error with given file, the program should terminate with the proper error code.
The program’s main source file should look like:

void initialize(int argc, char** argv)
{
  // put your implementation here 
  // ............................
}
 
int main(int argc, char** argv)
{
  // initialize the program based on the requirements
  initialize(argc, argv);
 
  // read the string 
  std::string input;
  std::cin >> input;
  
  // logic to get the reversed string
  std::string reversed;
  
  // put your reversing logic here
  // .............................
 
  // print to the console
  std::cout << s;
 
  return 0
}
Hint: you can use one of dup() functions.

Expected result
The resulting application should be able to build and execute from command line as follows:

make
./redirect-cin input-file
And, assuming input file contains the text “123456789”, the program should output the following:

987654321

FS-5: Manaul append
Description
Your task is to write a program on C/C++ that writes two lines in the files “first line” and “second line”. Writing should happen into two different file descriptors but write into the same file.

Requirements
Write a C/C++ program that gets the filepath with the first argument and opens it.
File should not be opened with the O_APPEND file.
You are not allowed to use seek/lseek to move the cursor to the end.
The program writes two lines: “first line” and “second line”. Each line is written in a different descriptor.
After write, the file should contain both lines in the same file.
Hint: you may use dup/dup2.

Expected result
The resulting application should be able to build and execute from command line as follows:

make
./manual-append result.txt
The output file should contain the following text:

first line
second line

FS-10: Copy a file accounting holes
Requirements
Write a C++ program accepting two command line arguments representing source file path and destination file path.
Implement proper error handling to account if not all arguments are passed or if one of arguments is not a valid one.
Copy the content of the source file into a destination file. If destination file already exists, it should be overwritten.
If source file contains holes, the destination file should also contain hole as is.
Print overall amount of bytes copied to the destination file including total amount of physical data and total amount of holes if any.
Expected result
The resulting application should be able to build and execute from command line as follows:

make
./copy source-file destination-file
After copy is completed, program should print following message to the console:

Successfully copied X bytes (data: M, hole: N).
The logical and physical sizes of source and destination files should match exactly.

FS-11: Write a program to copy a certain number of 
bytes from one file to another, while allowing the user to specify the
 the starting offset in the source file and the starting offset in the destination file.  
destination. Use the system calls open, read, write, and lseek.

Example of how the program works:
$ ./a.out. 
Enter the name of the source file: source.txt
Enter the name of the destination file: destination.txt
Enter the initial offset in the source file (in bytes): 100
Enter the initial offset in the destination file (in bytes): 50
Enter the number of bytes to copy: 200

Successfully copied 200 bytes from the source file to the destination file.

PS-1: Do command
Description
Your task is to implement a C++ program that accepts a command name and command arguments with command-line arguments and executes it in a new process.

Requirements
Write a C++ program called “do-command” that takes N arguments as an input.
Assume that first argument is command name and others are arguments for the given command.
A program should contain a special function do_command(char** argv), where argv is null-terminated vector of arguments.
The function “do_command” should create a new process, execute the given command with the given arguments and wait for its completion.
After program execution is done, do_function should print exit status of the command and the duration of the command execution.
The “main” function of the program is responsible for building proper argument vector and using do_command function to execute the command.
Expected result
The resulting application should be able to build and execute from command line as follows:

make
./do-command ls -al
The command is expected to print program output to the console and a line as follows:

drwxr-xr-x   8 user  user    256 Oct 21 15:45 .
drwxr-xr-x   3 user  user     96 Oct 16 16:45 ..
drwxr-xr-x  15 user  user    480 Nov  9 23:53 .git
-rw-r--r--   1 user  user    591 Oct 16 16:45 .gitignore
-rw-r--r--@  1 user  user  11357 Oct 21 15:45 LICENSE
-rw-r--r--   1 user  user     18 Oct 16 16:45 README.md
drwxr-xr-x  17 user  user    544 Oct 21 15:45 docs
drwxr-xr-x  16 user  user    512 Oct 16 16:45 samples
 
Command completed with X exit code and took Y seconds.

PS-2: Interactive shell
Requirements
Write a C++ program that implements an interactive shell with an infinite loop waiting for a command.
On every iteration of the loop a command string is passed to the shell program.
A command can be either a name of command or a path (relative or absolute).
After command is entered, program should create a new process to run the command.
The current directory of the shell program should be temporarily added to the PATH variable of every new process.
If “exit” command is entered, shell program should stop and exit with a success code.
If entered command starts with word “silent”, the executing command should redirect the standard output and error streams into the PID.log file where PID is the process id of the child process.
Expected result
The resulting application should be able to build and execute from command line as follows:

make
./interactive-shell
User may enter commands as follows:

ls
or

/bin/ls
or

./other-program
and the command should print the output to the console.

Also user may want to enter commands as follows

silent ls -al
and the output of program ls should be redirected into the file PID.log where PID is the identifier of the child process.
