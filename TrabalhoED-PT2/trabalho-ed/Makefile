#  Configuracao

APP        := GerenciadorPartidas
CC         := gcc
OUT_DIR    := arquivosGerados

INC_DIRS   := includes
SRC_DIR    := src

ifeq ($(OS),Windows_NT)
  EXE     := .exe
  MKDIR_P = if not exist "$(1)" mkdir "$(1)"
  RM_DIR  = rmdir /s /q
  OBJEXT  := .o
else
  EXE     :=
  MKDIR_P = mkdir -p "$(1)"
  RM_DIR  = rm -rf
  OBJEXT  := .o
endif

C_OPTS := -Wall -Wextra $(addprefix -I,$(INC_DIRS))

# Fontes agora todos em src/ (SEM partidas.c)
SRC_MAIN       := $(SRC_DIR)/main.c
SRC_BD_PART    := $(SRC_DIR)/bd_partidas.c
SRC_BD_TIMES   := $(SRC_DIR)/bd_times.c
SRC_TIMES      := $(SRC_DIR)/times.c

OBJ_MAIN       := $(OUT_DIR)/gerado_main$(OBJEXT)
OBJ_BD_PART    := $(OUT_DIR)/gerado_bd_partidas$(OBJEXT)
OBJ_BD_TIMES   := $(OUT_DIR)/gerado_bd_times$(OBJEXT)
OBJ_TIMES      := $(OUT_DIR)/gerado_times$(OBJEXT)

OBJ := $(OBJ_MAIN) $(OBJ_BD_PART) $(OBJ_BD_TIMES) $(OBJ_TIMES)

BIN := $(OUT_DIR)/$(APP)$(EXE)

.PHONY: all run clean fclean re

all: $(BIN)

$(BIN): $(OBJ)
	@echo "[link] unindo objetos -> $@"
	$(CC) $(OBJ) -o $@
	@echo "[ok] build concluido"

$(OBJ_MAIN): $(SRC_MAIN)
	@echo "[cc] $< -> $@"
	@$(call MKDIR_P,$(OUT_DIR))
	$(CC) $(C_OPTS) -c $< -o $@

$(OBJ_BD_PART): $(SRC_BD_PART)
	@echo "[cc] $< -> $@"
	@$(call MKDIR_P,$(OUT_DIR))
	$(CC) $(C_OPTS) -c $< -o $@

$(OBJ_BD_TIMES): $(SRC_BD_TIMES)
	@echo "[cc] $< -> $@"
	@$(call MKDIR_P,$(OUT_DIR))
	$(CC) $(C_OPTS) -c $< -o $@

$(OBJ_TIMES): $(SRC_TIMES)
	@echo "[cc] $< -> $@"
	@$(call MKDIR_P,$(OUT_DIR))
	$(CC) $(C_OPTS) -c $< -o $@

run: $(BIN)
	@echo "[run] executando..."
	"$(BIN)"

clean:
	@echo "[clean] removendo objetos..."
	-@del /q "$(OUT_DIR)\gerado_*$(OBJEXT)" 2>NUL || true
	-@rm -f "$(OUT_DIR)"/gerado_*$(OBJEXT) 2>/dev/null || true

fclean:
	@echo "[fclean] removendo pasta de gerados..."
	-@$(RM_DIR) "$(OUT_DIR)" 2>NUL || rm -rf "$(OUT_DIR)"

re: fclean all
