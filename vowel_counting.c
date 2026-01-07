/* vowel_counting_original.c */
/* 123456789 Israel Israeli */

#include <stdbool.h>
#include <stdio.h>

// Count array for each character
int letterCounts[26] = {0};  // a-z counts
int digitCounts[10] = {0};   // 0-9 counts

bool isLowerVowel(char c) {
    return c=='a' || c=='e' || c=='i' || c=='o' || c=='u';
}

bool isUpperVowel(char c) {
    return c=='A' || c=='E' || c=='I' || c=='O' || c=='U';
}

bool isVowel(char c) {
    return isLowerVowel(c) || isUpperVowel(c);
}

bool isLowerLetter(char c) {
    return c >= 'a' && c <= 'z';
}

bool isUpperLetter(char c) {
    return c >= 'A' && c <= 'Z';
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

char toLower(char c) {
    if (isUpperLetter(c)) {
        return c + ('a' - 'A');
    }
    return c;
}

void countCharacter(char c) {
    if (isLowerLetter(c)) {
        letterCounts[c - 'a']++;
    } else if (isUpperLetter(c)) {
        letterCounts[c - 'A']++;
    } else if (isDigit(c)) {
        digitCounts[c - '0']++;
    }
}

// function to find longest string that matches pi digits
int findLongestPiMatch(char* buf, int size) {
    // First 100 digits of pi: 3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067
    char piDigits[] = "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067";
    int piLength = 100;
    int longestMatch = 0;
    
    // search - check every position for longest match
    for (int i = 0; i < size; i++) {
        int currentMatch = 0;
        bool isInRow = true;
        
        // Check how many consecutive pi digits match starting at position i
        for (int j = 0; j < piLength && (i + j) < size; j++) {
            if (buf[i + j] == piDigits[j] && isInRow) {
                currentMatch++;
            }
            else {
                isInRow = false;
            }
        }
        
        // Update longest match found so far
        if (currentMatch > longestMatch) {
            longestMatch = currentMatch;
        }        
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
        positionsChecked++;
        // Count '3' at these sparse positions
        if (c == '3') count3++;
        // Also count vowels and digits for comparison
        if (isVowel(c)) vowelCount++;
        if (isDigit(c)) digitCount++;

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
    char *len = buf + size;
    while (ptr < len) {
        char c = *ptr;
        // Count this character
        countCharacter(c);
        // Check if vowel
        if (isVowel(c)) {
            vowelCount++;
        }
        ptr++;
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
