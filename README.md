# catbin
Simple C++ utility to print files' content like in binary format

# Flags
-o <offset>: Sets the starting offset in the file (default: 0)<br>
-l <limit>: Sets the maximum number of bytes to read from the file (default: 256)<br>
-s <sections>: Sets the number of sections per line (default: 2)<br>
-c <columns>: Sets the number of columns per section (default: 8)<br>

# Example
```shell
./catbin -l 0 -s 4 -c 4 text.txt
```
### Result (Example)
```
0   4C 00 00 00 01 14 02 00  00 00 00 00 C0 00 00 00   L....... ........
1   00 00 00 46                                        ...F
```
