/* vowel_counting_original.c */
/* 123456789 Israel Israeli */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Count array for each character
int letterCounts[26] = {0};  // a-z counts
int digitCounts[10] = {0};   // 0-9 counts

// lookup table for ASCII digits and letters
static const unsigned char char_info[256] = {
    // Digits: (index << 3) | IS_DIGIT
    ['0']=0x02, ['1']=0x0a, ['2']=0x12, ['3']=0x1a, ['4']=0x22, 
    ['5']=0x2a, ['6']=0x32, ['7']=0x3a, ['8']=0x42, ['9']=0x4a,

    // Letters: (index << 3) | IS_LETTER | (IS_VOWEL if vowel)
    ['A']=0x05, ['B']=0x09, ['C']=0x11, ['D']=0x19, ['E']=0x25, 
    ['F']=0x29, ['G']=0x31, ['H']=0x39, ['I']=0x45, ['J']=0x49, 
    ['K']=0x51, ['L']=0x59, ['M']=0x61, ['N']=0x69, ['O']=0x75, 
    ['P']=0x79, ['Q']=0x81, ['R']=0x89, ['S']=0x91, ['T']=0x99, 
    ['U']=0xa5, ['V']=0xa9, ['W']=0xb1, ['X']=0xb9, ['Y']=0xc1, ['Z']=0xc9,
    ['a']=0x05, ['b']=0x09, ['c']=0x11, ['d']=0x19, ['e']=0x25, 
    ['f']=0x29, ['g']=0x31, ['h']=0x39, ['i']=0x45, ['j']=0x49, 
    ['k']=0x51, ['l']=0x59, ['m']=0x61, ['n']=0x69, ['o']=0x75, 
    ['p']=0x79, ['q']=0x81, ['r']=0x89, ['s']=0x91, ['t']=0x99, 
    ['u']=0xa5, ['v']=0xa9, ['w']=0xb1, ['x']=0xb9, ['y']=0xc1, ['z']=0xc9
};

#define IS_LETTER(info)   ((info) & 0x01)
#define IS_DIGIT(info)    ((info) & 0x02)
#define IS_VOWEL(info)    ((info) & 0x04)

static const char piDigits[] = "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067";

int findLongestPiMatch(char* buf, int size) {
    int longestMatch = 0;
    
    char* current_pos = buf;
    int remaining_size = size;
    while (remaining_size--) {
        // early exit - if remaining buffer is smaller than the best we've found
        if (remaining_size + 1 <= longestMatch) break;

        int currentMatch = 0;
        const char* p_buf = current_pos;
        const char* p_pi  = piDigits;
        
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; 
        if (*p_buf++ == *p_pi++) { currentMatch++;
        if (*p_buf++ == *p_pi++) { currentMatch++; }}}}}}}}}}}}}}}}}}}}}}}
        }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}

        // update longest if needed
        if (currentMatch > longestMatch) {
            longestMatch = currentMatch;
        }
        
        // move to next buf char
        current_pos++;
    }

    return longestMatch;
}

// Find the position with highest Hamming match to 100 digits of pi
// Hamming match = count of positions where characters match (ignores mismatches in between)
void findBestHammingMatch(char* buf, int size) {
    int bestIndex = -1;
    int bestHammingScore = 0;
    
    // Check every possible starting position in buffer
    char *bufptr = buf;
    char *end = buf + size - 100;
    while (bufptr++ < end) {
        int hammingScore = 0;
        char *ptr = bufptr;
        char *piPtr = (char*)piDigits;
        
        // Process 8 bytes at a time using 64 bit int
        uint64_t buf_word = *(uint64_t*)(ptr);
        uint64_t pi_word = *(uint64_t*)(piPtr);
        
        // XOR: matching bytes = 0, otherwise non zero
        uint64_t t = buf_word ^ pi_word;
        // inisde each byte - propagate any set bits to MSB in byte
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        // save only LSB of each byte 
        t &= 0x0101010101010101;
        // invert - now matching bytes have 0x01, non-matching have 0x00
        t = 0x0101010101010101 - t;
        // sum up all bytes
        hammingScore += (t * 0x0101010101010101) >> 56;
        
        // Unroll a constant loop (piDigits is always 100 bytes)
        // bytes 8-15
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 16-23
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 24-31
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 32-39
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 40-47
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 48-55
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 56-63
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 64-71
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 72-79
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 80-87
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;
        // bytes 88-95
        ptr += 8;
        piPtr += 8;
        buf_word = *(uint64_t*)(ptr);
        pi_word = *(uint64_t*)(piPtr);
        t = buf_word ^ pi_word;
        t |= t >> 1;
        t |= t >> 2;
        t |= t >> 4;
        t &= 0x0101010101010101;
        t = 0x0101010101010101 - t;
        hammingScore += (t * 0x0101010101010101) >> 56;

        // bytes 96-99
        // Process remaining 4 bytes with 32 bit int
        uint32_t buf_word32 = *(uint32_t*)(ptr + 8);
        uint32_t pi_word32 = *(uint32_t*)(piPtr + 8);

        // Same xor logic as above just in 32 bits
        uint32_t t32 = buf_word32 ^ pi_word32;
        t32 |= t32 >> 1;
        t32 |= t32 >> 2;
        t32 |= t32 >> 4;
        t32 &= 0x01010101;
        t32 = 0x01010101 - t32;
        hammingScore += (t32 * 0x01010101) >> 24;

        if (hammingScore > bestHammingScore) {
            bestHammingScore = hammingScore;
            bestIndex = bufptr - buf;
        }
    }
    
    // Print results
    printf("=== Best Hamming Match to Pi (100 digits) ===\nBest index: %d\nHamming score: %d/100 matches\n", bestIndex, bestHammingScore);    
    if (bestIndex >= 0) {
        // Print character-by-character comparison
        printf("Character-by-character comparison:\nPi:  ");
        fwrite(piDigits, 1, 100, stdout);
        
        printf("\nBuf: ");
        char *bestbufptr = buf + bestIndex;
        fwrite(bestbufptr, 1, 100, stdout);
        printf("\n     ");
        
        for (int j = 0; j < 100; j++) {
            if (bestbufptr[j] == piDigits[j]) {
                printf("^");  // Match
            } else {
                printf(" ");  // Mismatch
            }
        }
        printf("\n");
    }
}

// Counts characters at addresses divisible by 1000 (huge stride = cache miss every access)
void analyzeAtSparseAddresses(char* buf, int size) {
    int count3 = 0;        // How many times '3' appears at index % 1000 == 0
    int vowelCount = 0;    // Vowels at those positions
    int digitCount = 0;    // Digits at those positions
    
    // calc steps once instead of checking ptr < end each iteration
    int steps = (size + 999) / 1000;
    int current = steps;
    char *ptr = buf;
    while (current--) {
        char c = *ptr;
        // Count '3' at these sparse positions
        if (c == '3') count3++;
        // Use lookup table to count vowels and digits
        unsigned char info = char_info[(unsigned char)c];
        vowelCount += IS_VOWEL(info) >> 2; // count third bit (vowel)
        digitCount += IS_DIGIT(info) >> 1; // count second bit (digit)
        
        ptr += 1000; // Move to next sparse address
        if (current--) {
            c = *ptr;
            // Count '3' at these sparse positions
            if (c == '3') count3++;
            // Use lookup table to count vowels and digits
            info = char_info[(unsigned char)c];
            vowelCount += IS_VOWEL(info) >> 2; // count third bit (vowel)
            digitCount += IS_DIGIT(info) >> 1; // count second bit (digit)
            
            ptr += 1000; // Move to next sparse address
        } else {
            break;
        }
    }
    
    printf("Positions checked: %d\nCount of '3' at addresses divisible by 1000: %d\nVowels at sparse addresses: %d\nDigits at sparse addresses: %d\n", steps, count3, vowelCount, digitCount);
}

int countVowels(char* buf, int size) {
    int vowelCount = 0;
    
    // pi digits check - find longest matching substring
    int longestPiMatch = findLongestPiMatch(buf, size);
    printf("Longest pi digit match found: %d characters\n", longestPiMatch);
    
    // Find best Hamming match to pi digits
    findBestHammingMatch(buf, size);
    
    // analysis - counts '3' at sparse addresses
    analyzeAtSparseAddresses(buf, size);

    char *ptr = buf;
    while (size--) {
        // Count character
        unsigned char info = char_info[(unsigned char)*ptr++];
        if (IS_LETTER(info)) {
            // Top 5 bits contain (0..25)
            letterCounts[info >> 3]++;
            vowelCount += IS_VOWEL(info) >> 2; // count third bit (vowel)
        } 
        else if (IS_DIGIT(info)) {
            // Top 5 bits contain (0..9)
            digitCounts[info >> 3]++;
        }
    }
    return vowelCount;
}

// Getter functions for statistics

int* getLetterCounts() {
    return letterCounts;
}

int* getDigitCounts() {
    return digitCounts;
}

void printAllStats(int vowelCount) {
    printf("Vowel count: %d, Letters: [", vowelCount);
    
    bool first = true;
    for (int i = 0; i < 26; i++) {
        if (letterCounts[i] > 0) {
            if (!first) printf(", ");
            printf("(%c,%d)", 'a' + i, letterCounts[i]);
            first = false;
        }
    }
    
    printf("], Digits: [");
    first = true;
    for (int i = 0; i < 10; i++) {
        if (digitCounts[i] > 0) {
            if (!first) printf(", ");
            printf("(%d,%d)", i, digitCounts[i]);
            first = false;
        }
    }
    printf("]\n");
}
