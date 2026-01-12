/* vowel_counting_original.c */
/* 123456789 Israel Israeli */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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

#define IS_LETTER(info)   ((info) & 1)
#define IS_DIGIT(info)    ((info >> 1) & 1)
#define IS_VOWEL(info)    ((info >> 2) & 1)
#define IS_THREE(info)    ((info) == 0x1a)

static const char piDigits[] = "3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067";

int findLongestPiMatch(char* buf, int size) {
    int NUM_PROCESSES = 16;
    int chunk_size = size >> 4;
    pid_t pids[NUM_PROCESSES - 1];
    
    // Create child processes
    for (int proc_id = 0; proc_id < NUM_PROCESSES - 1; proc_id++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            // Fork failed, kill any children already created
            for (int j = 0; j < proc_id; j++) {
                kill(pids[j], SIGKILL);
                wait(NULL);
            }
            // for simplicity, just reduce to single process
            NUM_PROCESSES = 1;
            goto parent_only;
        }
        
        if (pid == 0) {
            // handle chunk proc_id
            int start = proc_id * chunk_size;
            int end = start + chunk_size;
            
            int longestMatch = 0;
            char* current_pos = buf + start;
            int remaining_size = end - start;
            
            while (remaining_size--) {
                // early exit
                if (remaining_size + 1 <= longestMatch) break;
                
                int currentMatch = 0;
                const char* p_buf = current_pos;
                const char* p_pi = piDigits;
                
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
                
                if (currentMatch > longestMatch) {
                    longestMatch = currentMatch;
                }
                
                current_pos++;
            }
            
            // Exit with result (limited to 0-255 but assume piDigits is 100 chars)
            _exit(longestMatch);
        }
        
        // Parent stores child PID
        pids[proc_id] = pid;
    }
    
parent_only:
    // Parent process: handle last chunk
    int parent_id = NUM_PROCESSES - 1;
    int start = parent_id * chunk_size;
    int end = size;
    
    int longestMatch = 0;
    char* current_pos = buf + start;
    int remaining_size = end - start;
    
    while (remaining_size--) {
        // early exit
        if (remaining_size + 1 <= longestMatch) break;
        
        int currentMatch = 0;
        const char* p_buf = current_pos;
        const char* p_pi = piDigits;
        
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
        
        if (currentMatch > longestMatch) {
            longestMatch = currentMatch;
        }
        
        current_pos++;
    }
    
    int finalMax = longestMatch;
    
    // Wait for all children and collect results
    for (int i = 0; i < NUM_PROCESSES - 1; i++) {
        int status;
        wait(&status);
        
        if (WIFEXITED(status)) {
            int child_result = WEXITSTATUS(status);
            if (child_result > finalMax) {
                finalMax = child_result;
            }
        }
    }
    
    return finalMax;
}

// Find the position with highest Hamming match to 100 digits of pi
// Hamming match = count of positions where characters match (ignores mismatches in between)
void findBestHammingMatch(char* buf, int size) {
    // num of parallel forks
    int NUM_PROCESSES = 32;

    // init
    int searchSpace = size - 100;
    int chunk_size = searchSpace >> 5;
    pid_t pids[NUM_PROCESSES];
    int finalBestScore = 0;
    int winningForkId = -1;
    
    // Create NUM_PROCESSES child processes, each calculates its own chunk
    for (int proc_id = 0; proc_id < NUM_PROCESSES; proc_id++) {
        pid_t pid = fork();

        // means failed to start new fork
        if (pid < 0) {
            for (int j = 0; j < proc_id; j++) {
                kill(pids[j], SIGKILL);
                wait(NULL);
            }
            // for simplicity, just reduce to single process
            NUM_PROCESSES = 1;
            finalBestScore = 0;
            goto hamming_parent_only;
        }
        
        // fork started successfully
        if (pid == 0) {
            // Child process: handle chunk proc_id
            int start = proc_id * chunk_size;
            int end = (proc_id == NUM_PROCESSES - 1) ? searchSpace : (start + chunk_size);
            
            int bestHammingScore = 0;
            
            char *bufptr = buf + start - 1;
            char *chunk_end = buf + end;

            while (bufptr++ < chunk_end) {
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
                
                // save best score
                if (hammingScore > bestHammingScore) {
                    bestHammingScore = hammingScore;
                }
            }
            
            // Exit with best hamming score in current chunk
            _exit(bestHammingScore);
        }
        
        pids[proc_id] = pid;
    }
    
    // Parent: wait for all forks and collect results    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int status;
        pid_t finished_pid = wait(&status);
        
        if (WIFEXITED(status)) {
            int child_score = WEXITSTATUS(status);
            
            // Find which fork this was (needed for exact search later)
            int fork_id = -1;
            for (int j = 0; j < NUM_PROCESSES; j++) {
                if (pids[j] == finished_pid) {
                    fork_id = j;
                    break;
                }
            }
            
            if (child_score > finalBestScore) {
                finalBestScore = child_score;
                // save winning id to use as chunk start for exact search
                winningForkId = fork_id;
            }
        }
    }
    
hamming_parent_only:
    // Now recalculate winning fork's chunk to find exact index
    // if NUM_PROCESSES == 1, just search entire space here because forks failed
    const bool ONLY_PARENT = (NUM_PROCESSES == 1);
    int winning_start = ONLY_PARENT ? 0 : winningForkId * chunk_size;
    int winning_end = (ONLY_PARENT || winningForkId == NUM_PROCESSES - 1) ? searchSpace : (winning_start + chunk_size);
    
    int bestIndex = -1;
    char *bufptr = buf + winning_start - 1;
    char *winning_chunk_end = buf + winning_end;
    
    while (bufptr++ < winning_chunk_end) {
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

        if (!ONLY_PARENT) {
            if (hammingScore == finalBestScore) {
                bestIndex = bufptr - buf;
                // Exit while loop if got to best score
                break;
            }
        } else if (hammingScore > finalBestScore) { // must calc score from scratch in ONLY_PARENT mode
            finalBestScore = hammingScore;
            bestIndex = bufptr - buf;
        }
    }
    
    // Print results
    printf("=== Best Hamming Match to Pi (100 digits) ===\nBest index: %d\nHamming score: %d/100 matches\n", bestIndex, finalBestScore);   
    if (bestIndex >= 0) {
        // Print character-by-character comparison
        fwrite("Character-by-character comparison:\nPi:  3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067\nBuf: ",
            1, 147, stdout);
        
        char *bestbufptr = buf + bestIndex;
        fwrite(bestbufptr, 1, 100, stdout);
        
        static const char match_map[2] = {' ', '^'};
        char marker_buf[107] = {0};
        marker_buf[0] = '\n';
        marker_buf[1] = ' ';
        marker_buf[2] = ' ';
        marker_buf[3] = ' ';
        marker_buf[4] = ' ';
        marker_buf[5] = ' ';
        for (int j = 0; j < 100; j++) {
            marker_buf[j + 6] = match_map[bestbufptr[j] == piDigits[j]];
        }
        marker_buf[106] = '\n';
        fwrite(marker_buf, 1, 107, stdout);
    }
}

// Counts characters at addresses divisible by 1000 (huge stride = cache miss every access)
void analyzeAtSparseAddresses(char* buf, int size) {
    int d0 = 0, d1 = 0, v0 = 0, v1 = 0, c3_1 = 0, c3_2 = 0;
    
    // calc steps once instead of checking ptr < end each iteration
    int steps = (size + 999) / 1000;
    int current = steps;
    while (current >= 8) {
        // load eight sparse addresses
        unsigned char i0 = char_info[buf[0]];
        unsigned char i1 = char_info[buf[1000]];
        unsigned char i2 = char_info[buf[2000]];
        unsigned char i3 = char_info[buf[3000]];
        unsigned char i4 = char_info[buf[4000]];
        unsigned char i5 = char_info[buf[5000]];
        unsigned char i6 = char_info[buf[6000]];
        unsigned char i7 = char_info[buf[7000]];

        // update counts
        d0 += IS_DIGIT(i0) + IS_DIGIT(i1) + IS_DIGIT(i2) + IS_DIGIT(i3);
        d1 += IS_DIGIT(i4) + IS_DIGIT(i5) + IS_DIGIT(i6) + IS_DIGIT(i7);
        v0 += IS_VOWEL(i0) + IS_VOWEL(i1) + IS_VOWEL(i2) + IS_VOWEL(i3);
        v1 += IS_VOWEL(i4) + IS_VOWEL(i5) + IS_VOWEL(i6) + IS_VOWEL(i7);
        c3_1 += IS_THREE(i0) + IS_THREE(i1) + IS_THREE(i2) + IS_THREE(i3);
        c3_2 += IS_THREE(i4) + IS_THREE(i5) + IS_THREE(i6) + IS_THREE(i7);

        // move to next block
        buf += 8000;
        current -= 8;
    }

    int digitCount = d0 + d1;    // Digits at those positions
    int vowelCount = v0 + v1;    // Vowels at those positions
    int count3 = c3_1 + c3_2;    // How many times '3' appears at index % 1000 == 0

    // Handle remainder
    while (current--) {
        unsigned char info = char_info[*buf];
        digitCount += IS_DIGIT(info);
        vowelCount += IS_VOWEL(info);
        count3 += IS_THREE(info);
        buf += 1000;
    }
    
    printf("Positions checked: %d\nCount of '3' at addresses divisible by 1000: %d\nVowels at sparse addresses: %d\nDigits at sparse addresses: %d\n", steps, count3, vowelCount, digitCount);
}

int countVowels(char* buf, int size) {
    // pi digits check - find longest matching substring
    int longestPiMatch = findLongestPiMatch(buf, size);
    printf("Longest pi digit match found: %d characters\n", longestPiMatch);
    
    // Find best Hamming match to pi digits
    findBestHammingMatch(buf, size);
    
    // analysis - counts '3' at sparse addresses
    analyzeAtSparseAddresses(buf, size);

    int v0 = 0, v1 = 0, v2 = 0, v3 = 0;
    int local_letters[26] = {0};
    int local_digits[10] = {0};

    while (size >= 8) {
        // load eight bytes and than separate them by bitwise operations
        uint64_t eight = *(uint64_t*)(buf);
        unsigned char i0 = char_info[eight & 0xFF];
        unsigned char i1 = char_info[(eight >> 8) & 0xFF];
        unsigned char i2 = char_info[(eight >> 16) & 0xFF];
        unsigned char i3 = char_info[(eight >> 24) & 0xFF];
        unsigned char i4 = char_info[(eight >> 32) & 0xFF];
        unsigned char i5 = char_info[(eight >> 40) & 0xFF];
        unsigned char i6 = char_info[(eight >> 48) & 0xFF];
        unsigned char i7 = char_info[(eight >> 56) & 0xFF];

        // calculate vowel count for all bytes
        v0 += IS_VOWEL(i0) + IS_VOWEL(i1);
        v1 += IS_VOWEL(i2) + IS_VOWEL(i3);
        v2 += IS_VOWEL(i4) + IS_VOWEL(i5);
        v3 += IS_VOWEL(i6) + IS_VOWEL(i7);

        // update letter and digit counts
        local_letters[i0 >> 3] += IS_LETTER(i0);
        local_letters[i1 >> 3] += IS_LETTER(i1);
        local_letters[i2 >> 3] += IS_LETTER(i2);
        local_letters[i3 >> 3] += IS_LETTER(i3);
        local_letters[i4 >> 3] += IS_LETTER(i4);
        local_letters[i5 >> 3] += IS_LETTER(i5);
        local_letters[i6 >> 3] += IS_LETTER(i6);
        local_letters[i7 >> 3] += IS_LETTER(i7);
        local_digits[i0 >> 3] += IS_DIGIT(i0);
        local_digits[i1 >> 3] += IS_DIGIT(i1);
        local_digits[i2 >> 3] += IS_DIGIT(i2);
        local_digits[i3 >> 3] += IS_DIGIT(i3);
        local_digits[i4 >> 3] += IS_DIGIT(i4);
        local_digits[i5 >> 3] += IS_DIGIT(i5);
        local_digits[i6 >> 3] += IS_DIGIT(i6);
        local_digits[i7 >> 3] += IS_DIGIT(i7);

        buf += 8;
        size -= 8;
    }

    int vowelCount = v0 + v1 + v2 + v3;

    // Clean up remaining bytes
    while (size--) {
        unsigned char info = char_info[*buf++];
        local_letters[info >> 3] += IS_LETTER(info);
        local_digits[info >> 3] += IS_DIGIT(info);
        vowelCount += IS_VOWEL(info);
    }

    // Unrolled copy to global arrays
    letterCounts[0] = local_letters[0]; letterCounts[1] = local_letters[1];
    letterCounts[2] = local_letters[2]; letterCounts[3] = local_letters[3];
    letterCounts[4] = local_letters[4]; letterCounts[5] = local_letters[5];
    letterCounts[6] = local_letters[6]; letterCounts[7] = local_letters[7];
    letterCounts[8] = local_letters[8]; letterCounts[9] = local_letters[9];
    letterCounts[10] = local_letters[10]; letterCounts[11] = local_letters[11];
    letterCounts[12] = local_letters[12]; letterCounts[13] = local_letters[13];
    letterCounts[14] = local_letters[14]; letterCounts[15] = local_letters[15];
    letterCounts[16] = local_letters[16]; letterCounts[17] = local_letters[17];
    letterCounts[18] = local_letters[18]; letterCounts[19] = local_letters[19];
    letterCounts[20] = local_letters[20]; letterCounts[21] = local_letters[21];
    letterCounts[22] = local_letters[22]; letterCounts[23] = local_letters[23];
    letterCounts[24] = local_letters[24]; letterCounts[25] = local_letters[25];

    digitCounts[0] = local_digits[0]; digitCounts[1] = local_digits[1];
    digitCounts[2] = local_digits[2]; digitCounts[3] = local_digits[3];
    digitCounts[4] = local_digits[4]; digitCounts[5] = local_digits[5];
    digitCounts[6] = local_digits[6]; digitCounts[7] = local_digits[7];
    digitCounts[8] = local_digits[8]; digitCounts[9] = local_digits[9];
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
