#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_DIGITS 64  
// Binary String Builder
void to_32bit_binary(uint32_t value, char* out) {
    for (int i = 31; i >= 0; i--) {
        out[31 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    out[32] = '\0';
}

void oct_to_bin(const char* oct, char* out) {
    int position = 0;
    for (int i = 0; oct[i] != '\0'; i++) {
        int digit = oct[i] - '0';
        //binary needs up to 2^2
        for (int j = 2; j >= 0; j--) {
            out[position++] = ((digit >> j) & 1) ? '1' : '0';
        }
    }
    out[position] = '\0';
}

void oct_to_hex(const char* oct, char* out) {
    char bin[100];
    oct_to_bin(oct, bin);
    // convert to binary first for easier math
    int length = (int)strlen(bin);
    // add padding zeros to acount for grouping
    int pad = (4 - (length % 4)) % 4;
    char padded[100];
    for (int i = 0; i < pad; i++) {
        padded[i] = '0';
    }

    // copy binary string into padded buffer
    strcpy_s(padded + pad, sizeof(padded) - pad, bin);

    int out_position = 0;
    for (int i = 0; i < length + pad; i += 4) {
        int value = 0;
        for (int j = 0; j < 4; j++) {
            value = (value << 1) | (padded[i + j] - '0');
        }
        if (value < 10) out[out_position++] = '0' + value;
        else out[out_position++] = 'A' + (value - 10);
    }
    out[out_position] = '\0';
}

void hex_to_bin(const char* hex, char* out) {
    int position = 0;
    for (int i = 0; hex[i] != '\0'; i++) {
        char c = hex[i];
        int value;
        // get the decimal values from hex
        if (c >= '0' && c <= '9') value = c - '0';
        else if (c >= 'A' && c <= 'F') value = 10 + (c - 'A');
        else if (c >= 'a' && c <= 'f') value = 10 + (c - 'a');
        else continue; 
        // convert to binary
        for (int j = 3; j >= 0; j--) {
            out[position++] = ((value >> j) & 1) ? '1' : '0';
        }
    }
    out[position] = '\0';
}



void to_sign_magnitude(int32_t n, char* out) {
    if (n >= 0) {
        to_32bit_binary((uint32_t)n, out);
    }
    else {
        uint32_t magnitude = (uint32_t)(-n);
        to_32bit_binary(magnitude, out);
        // set sign bit
        out[0] = '1'; 
    }
}

void to_ones_complement(int32_t n, char* out) {
    if (n >= 0) {
        to_32bit_binary((uint32_t)n, out);
    }
    else {
        uint32_t magnitude = (uint32_t)(-n);
        to_32bit_binary(magnitude, out);
        //flip all bits
        for (int i = 0; i < 32; i++) {
            out[i] = (out[i] == '0') ? '1' : '0';
        }
    }
}

//C already stores in two's compliment, so nice and easy. Otherwise I'd copy 1's compliment negative case and add one
void to_twos_complement(int32_t n, char* out) {
    uint32_t bit_pattern = (uint32_t)n;
    to_32bit_binary(bit_pattern, out);
}

//carry overs from assignment 1-----------------------------------------------------------------------------------------------------------------------------
void div_convert(uint32_t n, int base, char* out, size_t out_size) {
    char buffer[100];
    int pos = 0;

    if (n == 0) {
        strcpy_s(out, out_size, "0"); 
        return;
    }

    while (n > 0) {
        int remainder = n % base;
        n /= base;
        buffer[pos++] = (remainder < 10) ? '0' + remainder : 'A' + (remainder - 10);
    }

    // Division gives backwards result, so flip
    int j;
    for (j = 0; j < pos && j < (int)out_size - 1; j++) {
        out[j] = buffer[pos - j - 1];
    }
    out[j] = '\0';
}
 void sub_convert(uint32_t n, int base, char* out) {
    char buffer[65]; // Temporary buffer
    int pos = 0;

    // Pesky zeros
    if (n == 0) {
        strcpy_s(out, 65, "0");
        return;
    }

    // Find the highest power
    uint32_t power = 1;
    while (power <= n / base) 
    { 
        power *= base;
    }

    // Subtraction algorithm
    while (power > 0) {
        int digit = n / power;     
        n -= digit * power;        

        // Convert 
        if (digit < 10)
            buffer[pos++] = '0' + digit;
        else
            buffer[pos++] = 'A' + (digit - 10);

        power /= base;
    }

    // Null-terminate
    buffer[pos] = '\0';
    strcpy_s(out, 65, buffer);
}
void print_tables(uint32_t n) {
    char bin[65], oct[23], hex_str[17];

    // Original number
    div_convert(n, 2, bin, sizeof(bin));
    div_convert(n, 8, oct, sizeof(oct));
    div_convert(n, 16, hex_str, sizeof(hex_str));
    printf("Original: Binary = %s Octal = %s Decimal = %u Hex = %s\n",
        bin, oct, n, hex_str);

    // Left shift by 3
    uint32_t shifted = n << 3;
    div_convert(n, 2, bin, sizeof(bin));
    div_convert(n, 8, oct, sizeof(oct));
    div_convert(n, 16, hex_str, sizeof(hex_str));
    printf("Left Shift 3: Binary = %s Octal = %s Decimal = %u Hex = %s\n",
        bin, oct, shifted, hex_str);

    // AND with 0xFF
    uint32_t masked = n & 0xFF;
    div_convert(n, 2, bin, sizeof(bin));
    div_convert(n, 8, oct, sizeof(oct));
    div_convert(n, 16, hex_str, sizeof(hex_str));
    printf("AND 0xFF: Binary = %s Octal = %s Decimal = %u Hex = %s\n",
        bin, oct, masked, hex_str);
}

        

int main(void) {
    FILE* fp = fopen("a2_test.txt", "r");
    if (!fp) {
        perror("Error opening a2_test.txt");
        return 1;
    }

    char line[100];
    int test_num = 1;
    int passed = 0, total = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Skip comments and blank lines
        if (line[0] == '#' || strlen(line) <= 1) continue;

        char func[64], input[128], expected[512];

        //splitting lines into correct variables
        if (sscanf_s(line, "%63s %127s %511s",
            func, (unsigned)_countof(func),
            input, (unsigned)_countof(input),
            expected, (unsigned)_countof(expected)) < 3) {
            continue; 
        }

        char actual[100];

        if (strcmp(func, "oct_to_bin") == 0) {
            oct_to_bin(input, actual);
        }
        else if (strcmp(func, "oct_to_hex") == 0) {
            oct_to_hex(input, actual);
        }
        else if (strcmp(func, "hex_to_bin") == 0) {
            hex_to_bin(input, actual);
        }
        else if (strcmp(func, "to_sign_magnitude") == 0) {
            //convert ascII to an int
            int32_t n = atoi(input);
            to_sign_magnitude(n, actual);
        }
        else if (strcmp(func, "to_ones_complement") == 0) {
            int32_t n = atoi(input);
            to_ones_complement(n, actual);
        }
        else if (strcmp(func, "to_twos_complement") == 0) {
            int32_t n = atoi(input);
            to_twos_complement(n, actual);
        }

        total++;
        int ok = strcmp(expected, actual) == 0;

        printf("Test %d: %s(\"%s\") -> Expected: \"%s\", Got: \"%s\" [%s]\n",
            test_num++, func, input, expected, actual, ok ? "PASS" : "FAIL");

        if (ok) passed++;
    }

    fclose(fp);

    printf("Summary: %d/%d tests passed\n", passed, total);
    return 0;
}}

    fclose(file);
    return 0;

}
