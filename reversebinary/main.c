//
//  main.c
//  reversebinary
//
//  Created by Michael Puterbaugh on 2/5/13.
//  Copyright (c) 2013 Michael Puterbaugh. All rights reserved.
//
//  From http://www.spotify.com/us/jobs/tech/reversed-binary/
//
//      "Yi has moved to Sweden and now goes to school here. The first years of schooling she got in China,
//      and the curricula do not match completely in the two countries. Yi likes mathematics, but now...
//      The teacher explains the algorithm for subtraction on the board, and Yi is bored. Maybe it is possible to
//      perform the same calculations on the numbers corresponding to the reversed binary representations of the
//      numbers on the board? Yi dreams away and starts constructing a program that reverses the binary
//      representation, in her mind. As soon as the lecture ends, she will go home and write it on her computer.
//
//      Task
//      Your task will be to write a program for reversing numbers in binary. For instance, the binary
//      representation of 13 is 1101, and reversing it gives 1011, which corresponds to number 11.
//
//      Input
//      The input contains a single line with an integer N, 1 ≤ N ≤ 1,000,000,000.
//
//      Output
//      Output one line with one integer, the number we get by reversing the binary representation of N."


#include <stdio.h>
#include <inttypes.h>

/*
 *  The program uses the most compact binary representation of any input number.
 *  I.e., no leading zeros.  Decimal 13 = %1101, not %00001101 or %00000000000000000000000000001101.
 *
 *  We normalize before reversal by shifting until the highest bit is set (if any).
 *  The new trailing zeros we might introduce are irrelevant: they'll become ignored leading zeros after reversal.
 */
uint32_t normalize_highest_bit_set(uint32_t v)
{
    // spec says 0 input isn't possible, but if we don't handle it sensibly we get an infinite loop.
    if (v == 0)
        return v;
    
    // Shift left until the highest bit is set.
    //      On platforms with a "count leading zeros" machine instruction, this could be optimized to
    //      v << clz(v)
    //      Maybe the compiler is already smart enough to do this optimization.
    while (!(v & 0x80000000))
    {
        v = v << 1;
    }
    return v;
}

/*
 *  Return the number we get by reversing the binary representation of v
 *
 *  The reversal algorithm requires an exactly 32-bit unsigned int;
 *  all possible input values can be represented in this range, as per the spec.
 */
uint32_t reverse_binary(uint32_t v)
{
    v = normalize_highest_bit_set(v);
    
    // Taken from http://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel
    //
    // Since I didn't write this code from scratch, I might as well explain it to prove I understand how it works.
    //      Swaps neighboring positions, in increasingly large chunks, until entirely reversed.  For an 8-bit int:
    //          76543210 => 6:7 4:5 2:3 0:1 => 45:67 01:23 => 0123:4567
    //      Masking after right shifts, but before left shifts, allows the same mask constants to be used twice.
    //      A tiny optimization, but bit-twiddling code is unreadable anyway so why not go all out, right?
    v =     ((v >>  1) & 0x55555555) | ((v & 0x55555555) <<  1);
    v =     ((v >>  2) & 0x33333333) | ((v & 0x33333333) <<  2);
    v =     ((v >>  4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) <<  4);
    v =     ((v >>  8) & 0x00FF00FF) | ((v & 0x00FF00FF) <<  8);
    return  ( v >> 16              ) | ( v               << 16);
}

int main(int argc, const char * argv[] /* ignored */)
{
    uint32_t v;
    
    while (scanf("%u", &v) != EOF)
    {
        printf("%u\n", reverse_binary(v));
    }
   
    return 0;
}


