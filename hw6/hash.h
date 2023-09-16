#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {   
        unsigned long long w[5] = {0, 0, 0, 0, 0};
        int digits[k.length()]; // holds the int versions of every letter/int

        // convert all the letters to digits
        int index = 0;
        while(index < (int)k.length())
        {
            digits[index] = letterDigitToNumber(k[index]);
            index++;
        }

        int rounds = k.length() / 6; // how many full rounds of 6 we have
        int remainder = k.length() % 6; // remainder, if we have one
    
        unsigned long long total = 0; // what will be put into w[#]
        if(remainder == 0) // there are round # of FULL substrings
        {
            for(int i = 0; i < rounds; i++) // loop round # of times
            {
                for(int j = 0; j < 6; j++)
                {
                    if(j == 0) total = digits[6 * i]; // first index of substring of 6
                    else
                    {
                        total *= 36;
                        total += digits[(6 * i) + j];
                    }
                }
                w[4 - rounds + i + 1] = total;
                total = 0;
            }
        }
        else // there is a round without 6 full characters
        {
            for(int i = 0; i < rounds + 1; i++) // loop round + 1 # of times (bc of remainder letters)
            {
                if(i == 0) // you're in the round with < 6 characters
                {
                    for(int j = 0; j < remainder; j++)
                    {
                        if(j == 0) total = digits[j]; // first index of substring of 6
                        else
                        {
                            total *= 36;
                            total += digits[j];
                        }
                    }
                    w[4 - rounds] = total;
                    total = 0;
                }
                else
                {
                    for(int j = 0; j < 6; j++)
                    {
                        if(j == 0) total = digits[(6 * i) - (6 - remainder)]; // first index of substring of 6
                        else
                        {
                            total *= 36;
                            total += digits[(6 * i) - (6 - remainder) + j];
                        }
                    }
                    w[4 - rounds + i] = total;
                    total = 0;
                }
            }
        }

        HASH_INDEX_T result = 0;
        for(int k = 0; k < 5; k++) // actually calculate h(k)
        {
            result += rValues[k] * w[k];
        }

        return result;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    int letterDigitToNumber(char letter) const
    {
        size_t result = 0;

        if(isupper(letter)) letter = tolower(letter); // turn to lowercase if necessary
        
        if(isdigit(letter))
        {
            result = letter - 22;
        }
        else // letter is a char
        {
            result = letter - 97;
        }
        return result;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
