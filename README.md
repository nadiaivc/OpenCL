# OpenCL
Vasya sends data to Anna via a special channel. The data is represented by the characters “0” and “1”. Sometimes there is noise in the channel represented by the symbol “2”.  Anya can read messages only in phased continuous areas. If one line contains different characters, then what she has already read, she skips. If Anya reads down the aisle for a long time, then she gets tired and reads incorrectly.

Thus, reading the line 000001111100000 requires 3 steps. To read subsequent messages, 7 steps are required:

* 1111111111111111111

* 00000000000000000

* 00000000000000000

* 000000111111000111

(the first 3 lines are read in 1 pass each, the last in 4).

Your program should determine the smallest number of cells that turn out to be incorrectly read, taking into account Ani's tiredness.

For example, add that after 2 passes, that is, the minimum number of incorrectly read cells is 35.

The program should receive data in the format indicated above. The response should be output to standard output.
