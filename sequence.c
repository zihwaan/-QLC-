#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int max_sequence;
    double avg_sequence;
    int total_sequences;
    int zero_sequences;
    int one_sequences;
    long total_length;
} SequenceStats;

SequenceStats analyze_binary_sequence(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        exit(1);
    }

    SequenceStats stats = {0, 0.0, 0, 0, 0, 0};
    int current_sequence = 1;
    char prev_bit = fgetc(file);
    int total_sequence_length = 0;
    
    if (prev_bit == EOF) {
        fclose(file);
        return stats;
    }
    
    stats.total_length = 1;
    char current_bit;
    
    while ((current_bit = fgetc(file)) != EOF) {
        stats.total_length++;
        
        if (current_bit == prev_bit) {
            current_sequence++;
        } else {
            // 시퀀스가 끝날 때마다 통계 업데이트
            if (prev_bit == '0') {
                stats.zero_sequences++;
            } else {
                stats.one_sequences++;
            }
            
            total_sequence_length += current_sequence;
            stats.max_sequence = (current_sequence > stats.max_sequence) ? 
                                current_sequence : stats.max_sequence;
            
            stats.total_sequences++;
            current_sequence = 1;
        }
        prev_bit = current_bit;
    }
    
    // 마지막 시퀀스 처리
    if (prev_bit == '0') {
        stats.zero_sequences++;
    } else {
        stats.one_sequences++;
    }
    
    total_sequence_length += current_sequence;
    stats.max_sequence = (current_sequence > stats.max_sequence) ? 
                        current_sequence : stats.max_sequence;
    stats.total_sequences++;
    
    stats.avg_sequence = (double)total_sequence_length / stats.total_sequences;
    
    fclose(file);
    return stats;
}

int main() {
    printf("=== 인코딩 전 (input2.txt) 분석 ===\n");
    SequenceStats input_stats = analyze_binary_sequence("input2.txt");
    printf("총 비트 수: %ld\n", input_stats.total_length);
    printf("최대 연속 길이: %d\n", input_stats.max_sequence);
    printf("평균 연속 길이: %.2f\n", input_stats.avg_sequence);
    printf("총 시퀀스 수: %d\n", input_stats.total_sequences);
    printf("0 시퀀스 수: %d\n", input_stats.zero_sequences);
    printf("1 시퀀스 수: %d\n", input_stats.one_sequences);
    printf("\n");
    
    printf("=== 인코딩 후 (output.txt) 분석 ===\n");
    SequenceStats output_stats = analyze_binary_sequence("output.txt");
    printf("총 비트 수: %ld\n", output_stats.total_length);
    printf("최대 연속 길이: %d\n", output_stats.max_sequence);
    printf("평균 연속 길이: %.2f\n", output_stats.avg_sequence);
    printf("총 시퀀스 수: %d\n", output_stats.total_sequences);
    printf("0 시퀀스 수: %d\n", output_stats.zero_sequences);
    printf("1 시퀀스 수: %d\n", output_stats.one_sequences);
    
    return 0;
}
