ROOT = $(pth)
BUILD = build
DIR_OBJ = $(BUILD)/obj

# 最终生成的可执行文件
TARGET	= main

# 模块所在目录
SRCS	= $(wildcard pth/lesson03_pth_cleanup.c common/*.c)
OBJS	= $(addprefix $(DIR_OBJ)/, $(patsubst %.c, %.o, $(SRCS)))

INCLUDES = -I include

FLAGS = -Wall -g

CC = gcc

.PHONY: clean all

all:
	@echo "1.检查编译目录是否存在"	
	@if [ ! -d build ]; then \
	mkdir -p build/obj/pth; \
	mkdir -p build/obj/common; \
	echo "编译目录已创建"; \
	fi
	@echo "2.源文件"
	@echo $(SRCS)
	@echo "3.目标文件"
	@echo $(OBJS)
	@echo "4.编译目录"
	@echo $(BUILD)
	@echo "5.开始编译"
	@make $(TARGET)

$(TARGET):$(OBJS)
	@echo "7.生成可执行文件" 
	$(CC) $^ -o $(BUILD)/$@ -pthread
	@echo "8.编译成功" 
$(DIR_OBJ)/%.o:%.c
	@echo "6.生成目标文件"
	$(CC) -c $< -o $@ $(INCLUDES) $(FLAGS)

clean:
	rm -fr $(BUILD)
