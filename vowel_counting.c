/* vowel_counting_original.c */
/* 123456789 Israel Israeli */

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

int findLongestPiMatch(char* buf, int size) {
    static const char piDigits[] = "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067";
    int longestMatch = 0;
    
    char* current_pos = buf;
    int remaining_size = size;
    while (remaining_size--) {
        // early exit - if remaining buffer is smaller than the best we've found
        if (remaining_size + 1 <= longestMatch) break;

        int currentMatch = 0;
        const char* p_buf = current_pos;
        const char* p_pi  = piDigits;
        
        // 12 chunks x 8 bytes = 96
        int chunks = 12; 
        while (chunks--) {
            // nested unrolling, stops at first mismatch
            if (*p_buf++ == *p_pi++) { currentMatch++;
            if (*p_buf++ == *p_pi++) { currentMatch++;
            if (*p_buf++ == *p_pi++) { currentMatch++;
            if (*p_buf++ == *p_pi++) { currentMatch++;
            if (*p_buf++ == *p_pi++) { currentMatch++;
            if (*p_buf++ == *p_pi++) { currentMatch++;
            if (*p_buf++ == *p_pi++) { currentMatch++;
            if (*p_buf++ == *p_pi++) { currentMatch++; }}}}}}}}
            goto update_record;
        }

        // 100 - 96 = 4 remaining chars
        if (*p_buf++ == *p_pi++) {
            currentMatch++;
            if (*p_buf++ == *p_pi++) {
                currentMatch++;
                if (*p_buf++ == *p_pi++) {
                    currentMatch++;
                    if (*p_buf++ == *p_pi++) {
                        currentMatch++; 
                    }
                }
            }
        }

        // update longest if needed
    update_record:
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
    char piDigits[] = "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067";
    
    int bestIndex = -1;
    int bestHammingScore = 0;
    
    // Check every possible starting position in buffer
    int piLength = 100;
    int end = size - piLength;
    for (int i = 0; i <= end; i++) {
        int hammingScore = 0;
        
        // Count matching characters at each position (Hamming similarity)
        char *bufptr = buf + i;
        int j = 0;
        for (; j < piLength - 4; j += 4) {
            if (bufptr[j] == piDigits[j]) hammingScore++;
            if (bufptr[j + 1] == piDigits[j + 1]) hammingScore++;
            if (bufptr[j + 2] == piDigits[j + 2]) hammingScore++;
            if (bufptr[j + 3] == piDigits[j + 3]) hammingScore++;
        }

        // Handle remaining chars
        for (; j < piLength; j++) {
            if (bufptr[j] == piDigits[j]) hammingScore++;
        }
        
        // Track the best match
        if (hammingScore > bestHammingScore) {
            bestHammingScore = hammingScore;
            bestIndex = i;
        }
    }
    
    // Print results
    printf("=== Best Hamming Match to Pi (100 digits) ===\n");
    printf("Best index: %d\n", bestIndex);
    printf("Hamming score: %d/100 matches\n", bestHammingScore);
    
    if (bestIndex >= 0) {
        // Print character-by-character comparison
        printf("Character-by-character comparison:\n");
        printf("Pi:  ");
        for (int j = 0; j < piLength; j++) {
            printf("%c", piDigits[j]);
        }
        printf("\n");
        
        char *bestbufptr = buf + bestIndex;
        printf("Buf: ");
        for (int j = 0; j < piLength; j++) {
            printf("%c", bestbufptr[j]);
        }
        printf("\n");
        
        printf("     ");
        for (int j = 0; j < piLength; j++) {
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
    int positionsChecked = 0;
    
    char *ptr = buf;
    char *end = buf + size;
    while (ptr < end) {
        // First
        char c = *ptr;
        if (c == '3') count3++;
        unsigned char info = char_info[(unsigned char)c];
        positionsChecked++;
        // Count '3' at these sparse positions
        // Also count vowels and digits for comparison
        if (IS_VOWEL(info)) vowelCount++;
        if (IS_DIGIT(info)) digitCount++;

        ptr += 1000;  // Move to next sparse address
    }
    
    printf("Positions checked: %d\n", positionsChecked);
    printf("Count of '3' at addresses divisible by 1000: %d\n", count3);
    printf("Vowels at sparse addresses: %d\n", vowelCount);
    printf("Digits at sparse addresses: %d\n", digitCount);
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
            // Check if vowel
            if (IS_VOWEL(info)) {
                vowelCount++;
            }
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
