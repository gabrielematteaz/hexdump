# catbin
This C utility, named "catbin", is a program that reads a file and displays its contents in a formatted manner. It utilizes the "mttstr" library for string manipulation and conversion operations.

# Compilation
To compile the utility, make sure you have a C compiler installed on your system and the "mttstr" library available. Then, follow these steps:
- Save the code into a file with a .c extension (e.g., catbin.c).
- Open a terminal or command prompt and navigate to the directory where the file is located.
- Run the following command (If you are using GCC) to compile the utility, linking against the "mttstr" library:

```shell
gcc -o catbin catbin.c -lmttstr
```
This assumes that the "mttstr" library is installed and available in the default library path. If it's installed in a non-standard location, you may need to specify the library path using the -L option, and include the library file using the -l option.

If there are no errors, the compilation process will generate an executable file named catbin in the same directory.

# Usage
Once you have compiled the utility, you can use it with the following command:

```shell
./catbin [options] file
```
Replace [options] with any combination of the following command-line options:
-o <offset>: Sets the starting offset in the file (default: 0).
-l <limit>: Sets the maximum number of bytes to read from the file (default: 0, which means read until the end of the file).
-s <sections>: Sets the number of sections per line (default: 2).
-c <columns>: Sets the number of columns per section (default: 8).
Replace file with the path to the file you want to read.

# Example
Here's an example usage of the utility:

```shell
./catbin -o 0 -l 100 -s 3 -c 4 myfile.txt
```
In this example, the utility will read the file myfile.txt, starting from the beginning (offset 0) and reading up to 100 bytes. The output will display the file contents with 3 sections per line, and each section will have 4 columns.
