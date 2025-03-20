##########################
# PARAMETRES MODIFIABLES #
##########################

# Compilateurs C et C++
SHELL=/usr/bin/fish
CC = gcc-14.2.0
CPPC = g++-14.2.0

# Différents répertoires utiles. Tous sont relatifs au répertoire du Makefile
LIB_DIR = lib 
SRC_DIR = src
INC_DIR = include
TEMPLATE_DIR = templates
OBJ_DIR = build/obj
BIN_DIR = build/bin

GLOBAL_INCLUDES = # fsge/componentslist.hpp

# Fichiers générés dans BIN_DIR
EXE_NAME = main
LIB_NAME = libfsge

# Extensions
EXT_TO_COMPILE = cpp c
EXT_OTHERS = h hpp tpp json frag vert geom tesc tese comp glsl

# Librairies à ajouter au projet
LDLIBS = xcb xcb-sync xcb-xkb vulkan

# Drapeaux des compilateurs C et C++
CFLAGS = -masm=intel -mfpmath=sse -mmmx -msse -msse2 -mavx2 -mfma -m64 # Y a les extensions MMX, SSE1, SSE2 et AVX
CPPFLAGS = -std=c++23 -masm=intel -mfpmath=sse -mmmx -msse -msse2 -mavx2 -mfma -m64 # Y a les extensions MMX, SSE1, SSE2 et AVX

###############################################
# LE CODE CI-DESSOUS NE DOIT PAS ETRE MODIFIE #
###############################################

DEFAULT_FLAGS = -Wall -pedantic -I$(INC_DIR) -I$(TEMPLATE_DIR) -MMD -MP -I/usr/local/gcc-14.2.0/include/c++/14.2.0 -I/usr/local/gcc-14.2.0/lib/gcc/x86_64-linux-gnu/14.2.0/include $(foreach i, $(GLOBAL_INCLUDES), -include $(i))
DEFAULT_LDLIBS = -lm  -L/usr/local/gcc-14.2.0/lib/gcc/x86_64-linux-gnu/14.2.0


ifeq ($(BUILD), release)
DEFAULT_FLAGS += -DRELEASE -O3
else
DEFAULT_FLAGS += -DDEBUG -O0 -g
endif

EXE = $(BIN_DIR)/$(EXE_NAME)
SRC = $(foreach e,$(EXT_TO_COMPILE),$(shell find . -name "*.$(e)"))
SRC := $(SRC:./%=%)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ := $(OBJ:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIB = $(wildcard $(LIB_DIR)/*.a)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CPPC) -o $@ $^ $(DEFAULT_LDLIBS) $(foreach l,$(LDLIBS),-l$(l)) # $(LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(DEFAULT_FLAGS) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	@mkdir -p $(dir $@)
	$(CPPC) $(DEFAULT_FLAGS) -c $< -o $@ $(CPPFLAGS)

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

.phony: all
all: $(EXE)	

.phony: release
release:
	@echo $(TEST)
	make "BUILD=release"

.phony: debug
debug:
	make "BUILD=debug"

.phony: clean
clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

.phony: count
count: # Pour le fun
	@find . -name "" $(foreach e,$(EXT_TO_COMPILE) $(EXT_OTHERS), -or -name "*.$(e)") | xargs wc -l | awk 'BEGIN {total = 0;}{if (NF == 2 && $$2 == "total") total += $$1;} END {print "Total lines count : " total}'

.phony: genlib
genlib: 	$(OBJ) | $(BIN_DIR)
	@ar rcs $(BIN_DIR)/$(LIB_NAME).a $^
	@echo "Generating library $(LIB_NAME).a"

-include $(OBJ:.o=.d)
