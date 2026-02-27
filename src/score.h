#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>

// 计分规则 (任天堂标准)
#define SCORE_SINGLE  40
#define SCORE_DOUBLE  100
#define SCORE_TRIPLE  300
#define SCORE_TETRIS  1200
#define SCORE_SOFT_DROP  1
#define SCORE_HARD_DROP 2

// 函数声明
int score_calculate_lines(int lines, int level);
int score_calculate_drop(int distance, bool is_hard);
void score_add(int* score, int points);

#endif
