# Programming Assignment

**Note:** The final solutions should contain a Makefile for the multi-stage build. The Makefile should also contain targets `all` and `clean`. It’s recommended to have compiler and compiler flags declared as Makefile variables. Alternatively, CMake could also be used instead of `make`.

---

## ENV-1: Setup GNU/Linux Environment

**Requirements:**

Install any GNU/Linux distribution on a Virtual or Physical Machine. Run the following commands in a shell:

```bash
lsb_release -a >> $USER.txt
```

(if available) and

```bash
cat /etc/os-release >> $USER.txt
```

Submit the `$USER.txt` file to the GitHub repository as a solution for the task.

**Expected Output:**

The expected output is a text file named `$USER.txt` where `$USER` is the current user’s username. The content should look similar to the following:

```
Distributor ID: Distribution Name
Description: Ubuntu 20.04.3 LTS
...
NAME="Ubuntu"
VERSION="20.04.3 LTS (Focal Fossa)"
ID=ubuntu
...
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
VERSION_CODENAME=focal
UBUNTU_CODENAME=focal
```

---

## ENV-2: Complex numbers and sorting

**Requirements:**

Write a multi-file C++ program consisting of the following 3 modules:

1. A class that represents a complex number along with all basic operations: plus, minus, multiply by a constant, absolute value.
2. A function that implements any sorting algorithm on an array/vector of complex numbers.
3. A main function containing a demonstration of having multiple complex numbers in an array and sorting them.

Modules for the complex number class and sorting algorithm should be separate and each of them should have source and header files.

**Expected Result:**

A solution containing all the described modules (source and header files), as well as a Makefile for automated build of the solution.

---

## FS-1: Read from the file

**Description:**

You need to implement a program that does the same thing as the `cat` command in Linux.

**Requirements:**

Write a C++ program `read-file` accepting a file path as an argument. Implement proper error handling to ensure the required file path is passed. Open the given file with the proper mode and ensure errors are handled. Read the content of the file into a buffer until reaching the end of the file. Print every portion of the file read in each iteration.

**Hint:** If you use `printf` or `std::cin`, ensure your strings are terminated with the ‘\0’ symbol.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./read-file input.txt
```

The program should output the content of the `input.txt` file on the screen.

---

## FS-2: Copy a file

**Requirements:**

Write a C++ program `simple-copy` accepting two command line arguments representing the source file path and destination file path. Implement proper error handling to account for missing or invalid arguments. Copy the content of the source file into the destination file. If the destination file already exists, it should be overwritten.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./simple-copy source-file destination-file
```

---

## FS-3: Secure file deletion

**Description:**

Your task is to implement a C++ program that deletes a file from the filesystem along with its content by replacing every byte in the file with the ‘\0’ character.

**Requirements:**

Write a C++ program named `hacker-rm` that accepts a path to a file as a command-line argument. Implement proper error handling to check if the file exists and if the input path is valid. Open the file for writing and replace every byte with the null character (‘\0’). Remove the file from the filesystem after overwriting its content.

**Hint:** You can use `write`, `stat`, `lseek`, and `unlink` functions to solve the problem.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./hacker-rm file-to-erase
```

---

## FS-4: Redirect standard input

**Description:**

By default, a program gets standard input waiting for keyboard input. Your task is to make any program read input from a file instead of waiting for keyboard input without changing the usage of `std::cin` or `scanf`.

**Requirements:**

Write a C++ program named `redirect-cin` that accepts a path to a file as a command-line argument. Implement the function `initialize(int argc, char** argv)` to start reading from the given file with proper error handling. Implement a simple logic for the `main()` function to read a string from the standard input (`std::cin`) and print the reversed string to the console (`std::out`).

**Hint:** You can use one of the `dup()` functions.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./redirect-cin input-file
```

Assuming the input file contains the text “123456789”, the program should output the following:

```
987654321
```

---

## FS-5: Manual append

**Requirements:**

Write a C/C++ program that writes two lines into a file “first line” and “second line”. Writing should happen into two different file descriptors but write into the same file.

**Hint:** You may use `dup`/`dup2`.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./manual-append result.txt
```

The output file should contain the following text:

```
first line
second line
```

---

## FS-10: Copy a file accounting holes

**Requirements:**

Write a C++ program accepting two command line arguments representing the source file path and destination file path. Implement proper error handling to account for missing or invalid arguments. Copy the content of the source file into the destination file. If the destination file already exists, it should be overwritten. If the source file contains holes, the destination file should also contain the hole as is. Print the overall amount of bytes copied to the destination file, including the total amount of physical data and total amount of holes if any.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./copy source-file destination-file
```

After the copy is completed, the program should print the following message to the console:

```
Successfully copied X bytes (data: M, hole: N).
```

---

## FS-11: Copy a certain number of bytes from one file to another

**Example:**

```bash
$ ./a.out
Enter the name of the source file: source.txt
Enter the name of the destination file: destination.txt
Enter the initial offset in the source file (in bytes): 100
Enter the initial offset in the destination file (in bytes): 50
Enter the number of bytes to copy: 200
```

**Expected Result:**

```
Successfully copied 200 bytes from the source file to the destination file.
```

---

## PS-1: Do command

**Requirements:**

Write a C++ program called `do-command` that takes `N` arguments as input. The first argument is the command name and the others are arguments for the given command. The function `do_command(char** argv)` should create a new process, execute the given command with the given arguments, and wait for its completion. After execution, `do_function` should print the exit status of the command and the duration of the command execution.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./do-command ls -al
```

The command is expected to print the program output to the console and a line like:

```
drwxr-xr-x 8 user user 256 Oct 21 15:45 .
drwxr-xr-x 3 user user 96 Oct 16 16:45 ..
...
Command completed with X exit code and took Y seconds.
```

---

## PS-2: Interactive shell

**Requirements:**

Write a C++ program that implements an interactive shell with an infinite loop waiting for a command. After entering a command, the program should create a new process to run the command. The current directory of the shell program should be temporarily added to the `PATH` variable of every new process. If the `exit` command is entered, the shell program should stop and exit with a success code. If the entered command starts with the word `silent`, the output of the executed command should be redirected to a file `PID.log`, where `PID` is the process ID of the child process.

**Expected Result:**

The resulting application should be able to build and execute from the command line as follows:

```bash
make ./interactive-shell
```

The user may enter commands such as:

```
ls
/bin/ls
./other-program
```

Also, the user may enter commands like:

```
silent ls -al
```

The output of `ls` should be redirected into the file `PID.log`, where `PID` is the identifier of the child process.

## TH-1: The Array Summary

### Requirements

Write a C++ program that creates an array of `N` elements (`N > 1,000,000`).  
Initialize the array with random values at the beginning.  
Compute the summary of the array using a regular for loop and calculate the time spent for the operation.  
Create `M` threads to sum the same array with multiple threads, considering that every thread gets an equal portion to sum, except, maybe, the last one.  
Wait for all the threads to complete and measure the time spent for summing the array using `M` threads.  
Print the durations spent to sum the array with and without threads.  

**Note**: `N` and `M` variables should be passed to the program via command line arguments.

### Expected Result

The resulting application should be able to build and execute from command line as follows:

```bash
make
./array-summary 1000000 4
```

The command should print the output to the console as follows:
```bash
Time spent without threads: X
Time spent with M threads: Y
```

## TH-SYNC: Threads Synchronization

### Requirements

Write the code of this program, synchronize it using the three synchronization primitives (`mutex`, `spinlock`, `semaphore`).

Calculate the time for which the program will be executed with the help of each of the above synchronizations.

As a result, send the best performing code for each of the synchronization primitives and a text file with the measurements.

Basic code:
```cpp
#include <iostream>
#include <pthread.h>

class Integer {
private:
  int inner_value = 0;
public:
  Integer(int value) :inner_value(value) {}
  void inc() {
    inner_value++;
  }
  int get_inner_value() {
    return inner_value;
  }
};

void* increment(void* arg) {
  Integer* num = (Integer*)arg;
  for (int i = 0; i < 10000; ++i) {
    num->inc();
  }
  return nullptr;
}

int main(int argc, char** argv) {
  Integer* num = new Integer(0);
  int thread_count = 50;
  pthread_t* tids = new pthread_t[thread_count];

  for (int i = 0; i < thread_count; ++i) {
    pthread_create(&tids[i], NULL, increment, num);  
  }

  for (int i = 0; i < thread_count; ++i) {
    pthread_join(tids[i], NULL);
  }

  std::cout << "Final value is " << num->get_inner_value() << std::endl;
  
  delete[] tids;
  delete num;
  return 0;
}
```
