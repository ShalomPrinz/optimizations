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

typedef struct {
    int vowelCount;
    int letters[26];
    int digits[10];
} Result;

// Single process vowel, letters, digits counting function
// NOTE - shouldn't be called, this is just an edge case when we fails to fork
Result calculateCountsSingleProcess(char* buf, int size) {
    Result r;
    int v0 = 0, v1 = 0, v2 = 0, v3 = 0;

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
        r.letters[i0 >> 3] += IS_LETTER(i0);
        r.letters[i1 >> 3] += IS_LETTER(i1);
        r.letters[i2 >> 3] += IS_LETTER(i2);
        r.letters[i3 >> 3] += IS_LETTER(i3);
        r.letters[i4 >> 3] += IS_LETTER(i4);
        r.letters[i5 >> 3] += IS_LETTER(i5);
        r.letters[i6 >> 3] += IS_LETTER(i6);
        r.letters[i7 >> 3] += IS_LETTER(i7);
        r.digits[i0 >> 3] += IS_DIGIT(i0);
        r.digits[i1 >> 3] += IS_DIGIT(i1);
        r.digits[i2 >> 3] += IS_DIGIT(i2);
        r.digits[i3 >> 3] += IS_DIGIT(i3);
        r.digits[i4 >> 3] += IS_DIGIT(i4);
        r.digits[i5 >> 3] += IS_DIGIT(i5);
        r.digits[i6 >> 3] += IS_DIGIT(i6);
        r.digits[i7 >> 3] += IS_DIGIT(i7);

        buf += 8;
        size -= 8;
    }

    r.vowelCount = v0 + v1 + v2 + v3;

    // Clean up remaining bytes
    while (size--) {
        unsigned char info = char_info[*buf++];
        r.letters[info >> 3] += IS_LETTER(info);
        r.digits[info >> 3] += IS_DIGIT(info);
        r.vowelCount += IS_VOWEL(info);
    }

    return r;
}

int countVowels(char* buf, int size) {
    /*
        Short strategy explanation:
            1. Create 3 forks, one for each heavy task (pi, hamming, sparse)
            2. Run remaining task (counting) in main process
            3. Wait for all forks to complete, print their results in order using pipes
            4. Return requested value - vowel count
    */
    
    // --- FORK 1: PI ---
    // create pi pipe
    int p1[2];
    pipe(p1);
    // create pi fork
    pid_t pi_fork_pid = fork();
    if (pi_fork_pid == 0) {
        // INLINE START - findLongestPiMatch(buf, size);
        int PI_NUM_PROCESSES = 16;
        int pi_chunk_size = size >> 4;
        pid_t pi_pids[PI_NUM_PROCESSES - 1];
        
        // Create child processes
        for (int proc_id = 0; proc_id < PI_NUM_PROCESSES - 1; proc_id++) {
            pid_t pid = fork();
            
            if (pid < 0) {
                // Fork failed, kill any children already created
                for (int j = 0; j < proc_id; j++) {
                    kill(pi_pids[j], SIGKILL);
                    waitpid(pi_pids[j], NULL, 0);
                }
                // for simplicity, just reduce to single process
                PI_NUM_PROCESSES = 1;
                goto parent_only;
            }
            
            if (pid == 0) {
                // handle chunk proc_id
                int start = proc_id * pi_chunk_size;
                int end = start + pi_chunk_size;
                
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
            pi_pids[proc_id] = pid;
        }
        
    parent_only:
        // Parent process: handle last chunk
        int parent_id = PI_NUM_PROCESSES - 1;
        int start = parent_id * pi_chunk_size;
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
        
        int longestPiMatch = longestMatch;
        
        // Wait for all children and collect results
        for (int i = 0; i < PI_NUM_PROCESSES - 1; i++) {
            int status;
            waitpid(pi_pids[i], &status, 0);
            
            if (WIFEXITED(status)) {
                int child_result = WEXITSTATUS(status);
                if (child_result > longestPiMatch) {
                    longestPiMatch = child_result;
                }
            }
        }
        
        // INLINE END - findLongestPiMatch(buf, size);
        char pi_pipe_buf;
        read(p1[0], &pi_pipe_buf, 1); // Wait for parent signal, and then print result
        printf("Longest pi digit match found: %d characters\n", longestPiMatch);
        fflush(stdout);
        _exit(0);
    }
    
    // --- FORK 2: HAMMING ---
    // create hamming pipe
    int p2[2];
    pipe(p2);
    // create hamming fork
    pid_t hamming_fork_pid = fork();
    if (hamming_fork_pid == 0) {
        // INLINE START - findBestHammingMatch(buf, size);
        // Find the position with highest Hamming match to 100 digits of pi
        // Hamming match = count of positions where characters match (ignores mismatches in between)

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
                    waitpid(pids[j], NULL, 0);
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
            pid_t finished_pid = waitpid(pids[i], &status, 0);
            
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
        
        // INLINE END - findBestHammingMatch(buf, size);
        char hamming_pipe_buf;
        read(p2[0], &hamming_pipe_buf, 1); // Wait for parent signal, and then print result
        printf("=== Best Hamming Match to Pi (100 digits) ===\nBest index: %d\nHamming score: %d/100 matches\n", bestIndex, finalBestScore);   
        if (bestIndex >= 0) {
            // Print character-by-character comparison
            fwrite("Character-by-character comparison:\nPi:  3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067\nBuf: ",
                1, 146, stdout);
            
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
        fflush(stdout);
        _exit(0);
    }

    // --- FORK 3: SPARSE ---
    // create sparse pipe
    int p3[2];
    pipe(p3);
    // create sparse fork
    pid_t sparse_fork_pid = fork();
    if (sparse_fork_pid == 0) {
        // INLINE START - analyzeAtSparseAddresses(buf, size);
        // Counts characters at addresses divisible by 1000 (huge stride = cache miss every access)
        int d0 = 0, d1 = 0, s_v0 = 0, s_v1 = 0, c3_1 = 0, c3_2 = 0;
        
        // calc steps once instead of checking ptr < end each iteration
        int steps = (size + 999) / 1000;
        int current = steps;
        char *ptr = buf;
        while (current >= 8) {
            // load eight sparse addresses
            unsigned char i0 = char_info[ptr[0]];
            unsigned char i1 = char_info[ptr[1000]];
            unsigned char i2 = char_info[ptr[2000]];
            unsigned char i3 = char_info[ptr[3000]];
            unsigned char i4 = char_info[ptr[4000]];
            unsigned char i5 = char_info[ptr[5000]];
            unsigned char i6 = char_info[ptr[6000]];
            unsigned char i7 = char_info[ptr[7000]];

            // update counts
            d0 += IS_DIGIT(i0) + IS_DIGIT(i1) + IS_DIGIT(i2) + IS_DIGIT(i3);
            d1 += IS_DIGIT(i4) + IS_DIGIT(i5) + IS_DIGIT(i6) + IS_DIGIT(i7);
            s_v0 += IS_VOWEL(i0) + IS_VOWEL(i1) + IS_VOWEL(i2) + IS_VOWEL(i3);
            s_v1 += IS_VOWEL(i4) + IS_VOWEL(i5) + IS_VOWEL(i6) + IS_VOWEL(i7);
            c3_1 += IS_THREE(i0) + IS_THREE(i1) + IS_THREE(i2) + IS_THREE(i3);
            c3_2 += IS_THREE(i4) + IS_THREE(i5) + IS_THREE(i6) + IS_THREE(i7);

            // move to next block
            ptr += 8000;
            current -= 8;
        }

        int digitCount = d0 + d1;    // Digits at those positions
        int sparseVowelCount = s_v0 + s_v1;    // Vowels at those positions
        int count3 = c3_1 + c3_2;    // How many times '3' appears at index % 1000 == 0

        // Handle remainder
        while (current--) {
            unsigned char info = char_info[*ptr];
            digitCount += IS_DIGIT(info);
            sparseVowelCount += IS_VOWEL(info);
            count3 += IS_THREE(info);
            ptr += 1000;
        }
        
        // INLINE END - analyzeAtSparseAddresses(buf, size);
        char sparse_pipe_buf;
        read(p3[0], &sparse_pipe_buf, 1); // Wait for parent signal, and then print result
        printf("Positions checked: %d\nCount of '3' at addresses divisible by 1000: %d\nVowels at sparse addresses: %d\nDigits at sparse addresses: %d\n", steps, count3, sparseVowelCount, digitCount);
        fflush(stdout);
        _exit(0);
    }

    // --- FORK 4: COUNT ---
    int WORKERS = 8;
    size_t chunk = size >> 3;
    int count_pipes[WORKERS][2];
    pid_t count_pids[WORKERS];
    // open pipes for all count_pipes in main process (bubbles to forks)
    for (int i = 0; i < WORKERS; i++) {
        pipe(count_pipes[i]);
    }

    // fork count_fork_pid to do the most heavy task
    pid_t count_fork_pid = fork();
    if (count_fork_pid == 0) {
        for (int i = 0; i < WORKERS; i++) {
            count_pids[i] = fork();

            if (count_pids[i] < 0) {
                // Fork failed - kill all previous forks
                for (int j = 0; j < i; j++) {
                    kill(count_pids[j], SIGKILL);
                    waitpid(count_pids[j], NULL, 0);
                }
                // Here I'm not in a created fork - but I'm in count_fork_pid
                // so I exit with error code and expect main process to calculate manually
                _exit(1);
            }

            if (count_pids[i] == 0) {
                // current fork shouldn't read from its pipe
                close(count_pipes[i][0]);

                Result r = {0};
                size_t off = i * chunk;
                size_t current_chunk_size = (i == WORKERS-1) ? size - off : chunk;
                
                int v0 = 0, v1 = 0, v2 = 0, v3 = 0;
                char *count_ptr = buf + off;
                while (current_chunk_size >= 8) {
                    // load eight bytes and than separate them by bitwise operations
                    uint64_t eight = *(uint64_t*)(count_ptr);
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
                    r.letters[i0 >> 3] += IS_LETTER(i0);
                    r.letters[i1 >> 3] += IS_LETTER(i1);
                    r.letters[i2 >> 3] += IS_LETTER(i2);
                    r.letters[i3 >> 3] += IS_LETTER(i3);
                    r.letters[i4 >> 3] += IS_LETTER(i4);
                    r.letters[i5 >> 3] += IS_LETTER(i5);
                    r.letters[i6 >> 3] += IS_LETTER(i6);
                    r.letters[i7 >> 3] += IS_LETTER(i7);
                    r.digits[i0 >> 3] += IS_DIGIT(i0);
                    r.digits[i1 >> 3] += IS_DIGIT(i1);
                    r.digits[i2 >> 3] += IS_DIGIT(i2);
                    r.digits[i3 >> 3] += IS_DIGIT(i3);
                    r.digits[i4 >> 3] += IS_DIGIT(i4);
                    r.digits[i5 >> 3] += IS_DIGIT(i5);
                    r.digits[i6 >> 3] += IS_DIGIT(i6);
                    r.digits[i7 >> 3] += IS_DIGIT(i7);

                    count_ptr += 8;
                    current_chunk_size -= 8;
                }

                r.vowelCount = v0 + v1 + v2 + v3;

                // Clean up remaining bytes
                while (current_chunk_size--) {
                    unsigned char info = char_info[*count_ptr++];
                    r.letters[info >> 3] += IS_LETTER(info);
                    r.digits[info >> 3] += IS_DIGIT(info);
                    r.vowelCount += IS_VOWEL(info);
                }            

                // write result to pipe
                write(count_pipes[i][1], &r, sizeof(r));
                close(count_pipes[i][1]);
                _exit(0);
            }
        }
        // exit count_fork_pid after all work is done
        // exit status 0 - signal everything worked as expected
        _exit(0);
    } else if (count_fork_pid < 0) {
        // I'm in main process and count_fork failed to start: set mode to parent only
        WORKERS = 1;
    }

    // Run on main process only - aggregate results from count_fork_pid workers
    // wait for count_fork_pid to end
    int count_fork_status;
    waitpid(count_fork_pid, &count_fork_status, 0);
    int count_exit_status = 0;
    if (WIFEXITED(count_fork_status)) {
        count_exit_status = WEXITSTATUS(count_fork_status);
    }
    int vowelCount = 0;
    // aggregate results from count_fork_pid workers only on success
    if (count_exit_status == 0 && WORKERS > 1) {
        for (int i = 0; i < WORKERS; i++) {
            Result r;
            read(count_pipes[i][0], &r, sizeof(r));
            close(count_pipes[i][0]);
    
            vowelCount += r.vowelCount;
            for (int j = 0; j < 26; j++) letterCounts[j] += r.letters[j];
            for (int j = 0; j < 10; j++) digitCounts[j] += r.digits[j];
        }
    } else {
        Result r = calculateCountsSingleProcess(buf, size);
        vowelCount += r.vowelCount;
        for (int j = 0; j < 26; j++) letterCounts[j] += r.letters[j];
        for (int j = 0; j < 10; j++) digitCounts[j] += r.digits[j];
    }

    // --- FORKS RESULT CONTROLLER ---
    // Print pi result
    write(p1[1], "G", 1);
    close(p1[1]);
    waitpid(pi_fork_pid, NULL, 0);

    // Print hamming result
    write(p2[1], "G", 1);
    close(p2[1]);
    waitpid(hamming_fork_pid, NULL, 0);
    
    // Print sparse result
    write(p3[1], "G", 1);
    close(p3[1]);
    waitpid(sparse_fork_pid, NULL, 0);

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
    char buffer[1024];
    char* ptr = buffer;
    
    // vowels
    ptr += sprintf(ptr, "Vowel count: %d, Letters: [", vowelCount);

    // letters
    int first = 1;
    for (int i = 0; i < 26; i++) {
        if (letterCounts[i] > 0) {
            if (!first) {
                *ptr++ = ',';
                *ptr++ = ' ';
            }
            *ptr++ = '(';
            *ptr++ = 'a' + i;
            *ptr++ = ',';
            ptr += sprintf(ptr, "%d", letterCounts[i]);
            *ptr++ = ')';
            first = 0;
        }
    }
    
    *ptr++ = ']';
    *ptr++ = ',';
    *ptr++ = ' ';
    *ptr++ = 'D';
    *ptr++ = 'i';
    *ptr++ = 'g';
    *ptr++ = 'i';
    *ptr++ = 't';
    *ptr++ = 's';
    *ptr++ = ':';
    *ptr++ = ' ';
    *ptr++ = '[';
    
    // digits
    first = 1;
    for (int i = 0; i < 10; i++) {
        if (digitCounts[i] > 0) {
            if (!first) {
                *ptr++ = ',';
                *ptr++ = ' ';
            }
            *ptr++ = '(';
            *ptr++ = '0' + i;
            *ptr++ = ',';
            ptr += sprintf(ptr, "%d", digitCounts[i]);
            *ptr++ = ')';
            first = 0;
        }
    }
    
    *ptr++ = ']';
    *ptr++ = '\n';
    *ptr = '\0';    
    fwrite(buffer, 1, ptr - buffer, stdout);
}
