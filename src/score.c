#include "score.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static HighScore high_scores[MAX_SCORES];
static int high_scores_loaded = 0;

int score_calculate_lines(int lines, int level) {
    int base_scores[] = {0, SCORE_SINGLE, SCORE_DOUBLE, SCORE_TRIPLE, SCORE_TETRIS};
    if (lines >= 1 && lines <= 4) {
        return base_scores[lines] * (level + 1);
    }
    return 0;
}

int score_calculate_drop(int distance, bool is_hard) {
    if (is_hard) {
        return distance * SCORE_HARD_DROP;
    }
    return distance * SCORE_SOFT_DROP;
}

void score_add(int* score, int points) {
    if (score && points > 0) {
        *score += points;
    }
}

void load_high_scores(void) {
    if (high_scores_loaded) return;
    
    FILE* f = fopen("data/highscores.txt", "r");
    if (!f) {
        // 初始化空记录
        memset(high_scores, 0, sizeof(high_scores));
        high_scores_loaded = 1;
        return;
    }
    
    for (int i = 0; i < MAX_SCORES; i++) {
        if (fscanf(f, "%31s %d %d %d %ld",
                   high_scores[i].name,
                   &high_scores[i].score,
                   &high_scores[i].lines,
                   &high_scores[i].level,
                   &high_scores[i].date) != 5) {
            // 填充剩余为空记录
            for (int j = i; j < MAX_SCORES; j++) {
                memset(&high_scores[j], 0, sizeof(HighScore));
            }
            break;
        }
    }
    fclose(f);
    high_scores_loaded = 1;
}

void save_high_score(const char* name, int score, int lines, int level) {
    HighScore new_score = {
        .score = score,
        .lines = lines,
        .level = level,
        .date = time(NULL)
    };
    strncpy(new_score.name, name, 31);
    new_score.name[31] = '\0';
    
    // 插入排序（按分数降序）
    for (int i = 0; i < MAX_SCORES; i++) {
        if (score > high_scores[i].score) {
            // 后移
            for (int j = MAX_SCORES - 1; j > i; j--) {
                high_scores[j] = high_scores[j-1];
            }
            high_scores[i] = new_score;
            break;
        }
    }
    
    // 保存到文件
    FILE* f = fopen("data/highscores.txt", "w");
    if (!f) return;
    
    for (int i = 0; i < MAX_SCORES; i++) {
        if (high_scores[i].score > 0) {
            fprintf(f, "%s %d %d %d %ld\n",
                    high_scores[i].name,
                    high_scores[i].score,
                    high_scores[i].lines,
                    high_scores[i].level,
                    high_scores[i].date);
        }
    }
    fclose(f);
}

bool is_new_high_score(int score) {
    // 如果还没填满，或者分数超过最后一个记录
    for (int i = 0; i < MAX_SCORES; i++) {
        if (high_scores[i].score == 0) return true;
        if (score > high_scores[i].score) return true;
    }
    return false;
}

void show_high_scores(void) {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║         TETRIS HIGH SCORES           ║\n");
    printf("╠══════╤═══════════╤═══════╤═══════════╣\n");
    printf("║ Rank │   Name    │ Score │   Level   ║\n");
    printf("╠══════╪═══════════╪═══════╪═══════════╣\n");
    
    for (int i = 0; i < MAX_SCORES; i++) {
        if (high_scores[i].score > 0) {
            printf("║  %2d  │ %-9s │ %5d │    %2d     ║\n",
                   i + 1,
                   high_scores[i].name,
                   high_scores[i].score,
                   high_scores[i].level);
        } else {
            printf("║  %2d  │           │       │           ║\n", i + 1);
        }
    }
    
    printf("╚══════╧═══════════╧═══════╧═══════════╝\n\n");
}
