#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_DIGITS 64  
void div_convert(uint32_t n, int base, char* out, size_t out_size) {
    char temp[65];
    int pos = 0;

    if (n == 0) {
        strcpy_s(out, out_size, "0"); // safe copy
        return;
    }

    while (n > 0) {
        int remainder = n % base;
        n /= base;
        temp[pos++] = (remainder < 10) ? '0' + remainder : 'A' + (remainder - 10);
    }

    // reverse safely
    int j;
    for (j = 0; j < pos && j < (int)out_size - 1; j++) {
        out[j] = temp[pos - j - 1];
    }
    out[j] = '\0';
}
 void sub_convert(uint32_t n, int base, char* out) {
    char temp[65]; // Temporary buffer
    int pos = 0;

    // Handle zero case
    if (n == 0) {
        strcpy_s(out, 65, "0");
        return;
    }

    // Find highest power of base <= n
    uint32_t power = 1;
    while (power <= n / base) { // prevent overflow
        power *= base;
    }

    // Subtraction algorithm
    while (power > 0) {
        int digit = n / power;     // how many times power fits
        n -= digit * power;        // subtract that portion

        // Convert digit to character
        if (digit < 10)
            temp[pos++] = '0' + digit;
        else
            temp[pos++] = 'A' + (digit - 10);

        power /= base;
    }

    // Null-terminate the result
    temp[pos] = '\0';
    strcpy_s(out, 65, temp);
}
void print_tables(uint32_t n) {
    char bin[65], oct[23], hex_str[17];

    // Original number
    div_convert(n, 2, bin, sizeof(bin));
    div_convert(n, 8, oct, sizeof(oct));
    div_convert(n, 16, hex_str, sizeof(hex_str));
    printf("Original     : Binary = %s Octal = %s Decimal = %u Hex = %s\n",
        bin, oct, n, hex_str);

    // Left shift by 3
    uint32_t shifted = n << 3;
    div_convert(n, 2, bin, sizeof(bin));
    div_convert(n, 8, oct, sizeof(oct));
    div_convert(n, 16, hex_str, sizeof(hex_str));
    printf("Left Shift 3 : Binary = %s Octal = %s Decimal = %u Hex = %s\n",
        bin, oct, shifted, hex_str);

    // AND with 0xFF
    uint32_t masked = n & 0xFF;
    div_convert(n, 2, bin, sizeof(bin));
    div_convert(n, 8, oct, sizeof(oct));
    div_convert(n, 16, hex_str, sizeof(hex_str));
    printf("AND 0xFF    : Binary = %s Octal = %s Decimal = %u Hex = %s\n",
        bin, oct, masked, hex_str);
}

        

int main(void) {
    FILE* file = fopen("a1_test_file.txt", "r");
    if (!file) {
        printf("Error: Could not open file\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip empty lines or lines starting with #
        char* ptr = line;
        while (*ptr != '\0' && isspace((unsigned char)*ptr)) ptr++;
        if (*ptr == '\0' || *ptr == '#') continue;

        char func_name[50];
        uint32_t num;
        int base;
        char expected[65];
        char result[65];

        // Handle div_convert / sub_convert
        if (strncmp(ptr, "div_convert", 11) == 0 || strncmp(ptr, "sub_convert", 11) == 0) {
            int fields = sscanf_s(ptr, "%49s %u %d %64s",
                func_name, (unsigned int)_countof(func_name),
                &num,
                &base,
                expected, (unsigned int)_countof(expected));
            if (fields != 4) {
                printf("Skipping invalid line: %s", line);
                continue;
            }

            if (strcmp(func_name, "div_convert") == 0) {
                div_convert(num, base, result, sizeof(result));
            }
            else {
                sub_convert(num, base, result);
            }

            printf("Test: %s %u %d => Expected: %s, Got: %s - %s\n",
                func_name, num, base, expected, result,
                strcmp(result, expected) == 0 ? "PASS" : "FAIL");

        }
        // Handle print_tables
        else if (strncmp(ptr, "print_tables", 12) == 0) {
            char extra[50]; // placeholder for FORMATTED_OUTPUT
            int fields = sscanf_s(ptr, "%49s %u %49s",
                func_name, (unsigned int)_countof(func_name),
                &num,
                extra, (unsigned int)_countof(extra));
            if (fields < 2) {
                printf("Skipping invalid line: %s", line);
                continue;
            }

            print_tables(num);
        }
        // Unknown function
        else {
            printf("Unknown function: %s\n", ptr);
        }
    }

    fclose(file);
    return 0;
}