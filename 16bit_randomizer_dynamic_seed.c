#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define LFSR_MASK 0xB400u // 16-bit LFSR mask // 다항식(x^16 + x^14 + x^13 + x^11 + 1)
#define PAGE_SIZE 4096 
#define FIXED_SEED 0x1234 // 16-bit fixed seed

uint16_t generate_page_seed(uint16_t fixed_seed, size_t page_number) {
    return (fixed_seed ^ (page_number * 0x9E37) ^ 0xABCD) & 0xFFFF;
}

char* text_to_binary(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("파일을 열 수 없습니다.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* data = (char*)malloc(length + 1);
    if (!data) {
        perror("메모리 할당 실패");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    char* binary_data = (char*)malloc(length * 8 + 1);
    binary_data[0] = '\0';

    for (long i = 0; i < length; i++) {
        for (int j = 7; j >= 0; j--) {
            char bit = ((data[i] >> j) & 1) + '0';
            strncat(binary_data, &bit, 1);
        }
    }

    free(data);
    return binary_data;
}

void write_binary_to_file(const char* filename, const char* data) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("파일을 열 수 없습니다.");
        return;
    }
    fprintf(file, "%s", data);
    fclose(file);
}

void binary_to_text(const char* binary_data, const char* output_filename) {
    FILE* file = fopen(output_filename, "wb");
    if (!file) {
        perror("파일을 열 수 없습니다.");
        return;
    }

    size_t length = strlen(binary_data);
    for (size_t i = 0; i < length; i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8 && (i + j) < length; j++) {
            byte = (byte << 1) | (binary_data[i + j] - '0');
        }
        fputc(byte, file);
    }

    fclose(file);
}

void lfsr_process_page(const char* input, char* output, uint16_t fixed_seed, size_t page_number, size_t page_size_bits) {
    uint16_t page_seed = generate_page_seed(fixed_seed, page_number);
    uint16_t lfsr = page_seed;
    
    for (size_t i = 0; i < page_size_bits; i++) {
        uint8_t bit = input[i] - '0';
        uint8_t rand_bit = (lfsr & 1);
        uint8_t xor_bit = bit ^ rand_bit;

        output[i] = xor_bit + '0';
        lfsr = (lfsr >> 1) ^ (-(lfsr & 1) & LFSR_MASK);
    }
    output[page_size_bits] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("사용법: %s <입력 텍스트 파일 이름> <모드 (encode/decode)>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "encode") == 0) {
        printf("고정 Seed: 0x%04X\n", FIXED_SEED);

        char* input_binary = text_to_binary(argv[1]);
        if (!input_binary) {
            return EXIT_FAILURE;
        }

        write_binary_to_file("input2.txt", input_binary);

        size_t total_binary_length = strlen(input_binary);
        size_t page_size_bits = PAGE_SIZE * 8;
        size_t total_pages = (total_binary_length + page_size_bits - 1) / page_size_bits;

        FILE* output_file = fopen("output.txt", "w");
        
        if (!output_file) {
            perror("파일을 열 수 없습니다.");
            free(input_binary);
            return EXIT_FAILURE;
        }

        for (size_t page = 0; page < total_pages; page++) {
            size_t start_bit = page * page_size_bits;
            size_t end_bit = (page + 1) * page_size_bits;
            
            if (end_bit > (size_t)total_binary_length) {
                end_bit = total_binary_length;
            }
            
            size_t current_page_size = end_bit - start_bit;
            
            char* page_input = (char*)malloc(current_page_size + 1);
            char* page_output = (char*)malloc(current_page_size + 1);
            
            strncpy(page_input, input_binary + start_bit, current_page_size);
            page_input[current_page_size] = '\0';

            lfsr_process_page(page_input, page_output, FIXED_SEED, page, current_page_size);

            fprintf(output_file, "%s", page_output);

            free(page_input);
            free(page_output);
        }

        fclose(output_file);
        printf("인코딩 완료: output.txt, input2.txt 생성\n");
        free(input_binary);
    } 
    else if (strcmp(argv[2], "decode") == 0) {
        FILE* output_file = fopen("output.txt", "r");
        if (!output_file) {
            perror("output.txt 파일을 열 수 없습니다.");
            return EXIT_FAILURE;
        }

        fseek(output_file, 0, SEEK_END);
        long output_length = ftell(output_file);
        fseek(output_file, 0, SEEK_SET);

        char* encoded_input = (char*)malloc(output_length + 1);
        fread(encoded_input, 1, output_length, output_file);
        fclose(output_file);
        encoded_input[output_length] = '\0';

        char* decoded_binary = (char*)malloc(output_length + 1);
        decoded_binary[0] = '\0';

        size_t page_size_bits = PAGE_SIZE * 8;
        size_t total_pages = (output_length + page_size_bits - 1) / page_size_bits;

        for (size_t page = 0; page < total_pages; page++) {
            size_t start_bit = page * page_size_bits;
            size_t end_bit = (page + 1) * page_size_bits;
            
            if (end_bit > (size_t)output_length) {
                end_bit = output_length;
            }
            
            size_t current_page_size = end_bit - start_bit;
            
            char* page_input = (char*)malloc(current_page_size + 1);
            char* page_output = (char*)malloc(current_page_size + 1);
            
            strncpy(page_input, encoded_input + start_bit, current_page_size);
            page_input[current_page_size] = '\0';

            lfsr_process_page(page_input, page_output, FIXED_SEED, page, current_page_size);

            strcat(decoded_binary, page_output);

            free(page_input);
            free(page_output);
        }

        write_binary_to_file("decode1.txt", decoded_binary);
        binary_to_text(decoded_binary, "decode2.txt");
        printf("디코딩 완료: decode1.txt, decode2.txt 생성\n");

        free(encoded_input);
        free(decoded_binary);
    } 
    else {
        printf("잘못된 모드입니다. 'encode' 또는 'decode'를 사용하십시오.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}