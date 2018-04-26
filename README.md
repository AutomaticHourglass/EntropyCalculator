# EntropyCalculator
Simple entropy calculator for plaintext and binary files in c++.

Started as a class project on of of my PhD classes.

Program works as follows:

File Entropy Calculator v1.0
Author:   Unsal Gokdag
Counts occuring of each letter and calculates the entropy via:
    H = -P(A)i(A)
where:

P(A) is probablity of symbol
i(A) is information held by symbol = log2(A)

Update: Calculation of 1-step Markov Proecess is added.

Usage:
./entropycalc <filename> (binary)
binary makes program to calculate entropy of all ascii values.

Example:
./entropycalc Feeding.txt
./entropycalc 'head.bmp.zip' binary
Enjoy!
