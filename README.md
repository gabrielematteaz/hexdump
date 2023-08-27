# catbin
Simple C utility to print files' content like in binary format

# Flags
-o <offset>: Sets the starting offset in the file (default: 0)<br>
-c <count>: Sets the maximum number of bytes to read from the file (default: 256)<br>
-s <section size>: Sets the size of the hex section (default: 16)<br>

# Example
```shell
./catbin -l 0 data.bin
```
### Result (Example)
```
0   4C 00 00 00 01 14 02 00 00 00 00 00 C0 00 00 00   L...............
1   00 00 00 46                                        ...F
```
