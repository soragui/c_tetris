# 编译器和标志
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Isrc -O2
LDFLAGS = -lncurses

# 目录
SRC_DIR = src
OBJ_DIR = obj

# 源文件和目标文件
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 目标可执行文件
TARGET = tetris

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	@echo "链接 $(TARGET)..."
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# 编译
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "编译 $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 创建 obj 目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 清理
clean:
	@echo "清理..."
	rm -rf $(OBJ_DIR) $(TARGET)

# 重新编译
rebuild: clean all

# 运行
run: $(TARGET)
	./$(TARGET)

# 调试版本
debug: CFLAGS += -g -DDEBUG
debug: clean all

# 帮助
help:
	@echo "可用目标:"
	@echo "  all      - 编译项目（默认）"
	@echo "  clean    - 清理编译产物"
	@echo "  rebuild  - 重新编译"
	@echo "  run      - 编译并运行"
	@echo "  debug    - 编译调试版本"
	@echo "  help     - 显示帮助"

.PHONY: all clean rebuild run debug help
