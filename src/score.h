#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <time.h>

#define SCORE_SINGLE  40
#define SCORE_DOUBLE  100
#define SCORE_TRIPLE  300
#define SCORE_TETRIS  1200
#define SCORE_SOFT_DROP  1
#define SCORE_HARD_DROP 2
#define MAX_SCORES    10

typedef struct {
    char name[32];
    int score;
    int lines;
    int level;
    time_t date;
} HighScore;

int score_calculate_lines(int lines, int level);
int score_calculate_drop(int distance, bool is_hard);
void score_add(int* score, int points);

// 最高分系统
void load_high_scores(void);
void save_high_score(const char* name, int score, int lines, int level);
bool is_new_high_score(int score);
void show_high_scores(void);

#endif
