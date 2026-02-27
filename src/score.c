#include "score.h"

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
