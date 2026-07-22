#Makefile setup modified from from: https://makefiletutorial.com/

#Settings
TARGET_EXEC := main
TARGET_DIR := bin
BUILD_DIR := build
SRC_DIRS := src include
DATA_DIR = data

DATA_SRC := $(shell find $(SRC_DIRS) -path '*data*' -name '*.c')
SRC := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRC:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC := clang
CFLAGS := $(INC_FLAGS) -MMD -MP -std=c23 -O3 -Wall -Wextra
LDLIBS := -lncurses
LDFLAGS :=
CDEFS :=

#Set debug with 'make DEBUG=1'
ifdef DEBUG
	CFLAGS += -g3 -DDEBUG -O0
endif

#Colors
RED := \033[38;2;255;0;0m
YELLOW := \033[38;2;255;255;0m
GREEN := \033[38;2;21;204;4m
NC := \033[0m

#Building
$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	@mkdir -p $(dir $@)
	@$(CC) $(OBJS)  -o $(TARGET_DIR)/$(TARGET_EXEC) $(LDFLAGS) $(LDLIBS) && \
		printf '\n  $(GREEN)Compiled binary$(NC): %s\n' $@

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@printf '  $(YELLOW)Compiling$(NC): %s\n' $@
	@$(CC) $(CFLAGS) $(CDEFS) -c $< -o $@

#Commands
.PHONY: clean data debug run
clean: 
	@printf '  $(RED)rm -r$(NC) %s %s %s\n' $(BUILD_DIR)/ $(TARGET_DIR)/ $(DATA_DIR)/
	@rm -r build/ bin/ data/ 2>/dev/null || true

#Generate data/ files and build src/data/data.c.
data:
	@printf '  $(YELLOW)Downloading and Building$(NC): %s\n' "$(DATA_DIR) *.json and *.txt files"
	@./scripts/build-data-source-files.py
	@printf '  $(YELLOW)Building$(NC): %s\n' "data/*.c files"
	@./scripts/generate-data.c.sh

debug:
	@$(MAKE) clean
	@$(MAKE) DEBUG=1
	@gdb bin/main

run: $(TARGET_DIR)/$(TARGET_EXEC)
	@./$(TARGET_DIR)/$(TARGET_EXEC)

-include $(DEPS)
