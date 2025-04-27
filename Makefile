##########################
# PARAMETRES MODIFIABLES #
##########################

# Compilateurs C et C++
SHELL=/usr/bin/fish
CPREPROC = gcc-14.2.0 -E
CC = gcc-14.2.0
CPPC = g++-14.2.0
LINKER = ld

# Différents répertoires utiles. Tous sont relatifs au répertoire du Makefile
BUILD_ROOT = build
LIB_DIR = lib 
SRC_DIR = src
INC_DIR = include
LD_DIR = ldscripts
TEMPLATE_DIR = templates
OBJ_DIR := $(BUILD_ROOT)/obj
BIN_DIR := $(BUILD_ROOT)/bin

GLOBAL_INCLUDES =

LINKER_SCRIPT_TEMPLATE=link.template.ld

# Fichiers générés dans BIN_DIR
ELF_NAME = main
LIB_NAME = libmephisto
LINKER_SCRIPT = $(BUILD_ROOT)/link.ld

# Extensions
EXT_TO_COMPILE = cpp c
EXT_OTHERS = h hpp tpp json frag vert geom tesc tese comp glsl

# Librairies à ajouter au projet
LDLIBS = xcb xcb-sync xcb-xkb vulkan

# Drapeaux des compilateurs C et C++
CFLAGS = -masm=intel -mfpmath=sse  -mfma -m64 # Y a les extensions MMX, SSE1, SSE2 et AVX
CFLAGS += -mmmx -msse -msse2 -mavx2 # extensions d'instructions SIMD sur les flottants
CFLAGS += -mrdrnd -mrdseed # extensions pour les générations aléatoires physiques

CPPFLAGS = -march=native -std=c++23 -masm=intel -mfpmath=sse  -mfma -m64 # Y a les extensions MMX, SSE1, SSE2 et AVX
CPPFLAGS += -mmmx -msse -msse2 -mavx2 # extensions d'instructions SIMD sur les flottants
CPPFLAGS += -mrdrnd -mrdseed # extensions pour les générations aléatoires physiques

###############################################
# LE CODE CI-DESSOUS NE DOIT PAS ETRE MODIFIE #
###############################################

DEFAULT_FLAGS = -pipe -Wall -pedantic -I$(INC_DIR) -I$(TEMPLATE_DIR) -MMD -MP -I/usr/local/gcc-14.2.0/include/c++/14.2.0 -I/usr/local/gcc-14.2.0/lib/gcc/x86_64-linux-gnu/14.2.0/include $(foreach i, $(GLOBAL_INCLUDES), -include $(i))
DEFAULT_LDLIBS = -lm  -L/usr/local/gcc-14.2.0/lib/gcc/x86_64-linux-gnu/14.2.0

ifeq ($(BUILD), release)
DEFAULT_FLAGS += -DNDEBUG -Ofast # fast permet les optis mathématiques même si moins bien dans le cas général
else
DEFAULT_FLAGS += -O0 -g
endif

ELF = $(BIN_DIR)/$(ELF_NAME)
SRC = $(foreach e,$(EXT_TO_COMPILE),$(shell find . -name "*.$(e)"))
SRC := $(SRC:./%=%)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ := $(OBJ:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIB = $(wildcard $(LIB_DIR)/*.a)

$(ELF): $(OBJ) $(LINKER_SCRIPT) | $(BIN_DIR)
	$(CPPC) $(LINKER_SCRIPT) -o $@ $(OBJ) $(DEFAULT_LDLIBS) $(foreach l,$(LDLIBS),-l$(l)) -fno-pie -no-pie # $(LIB)

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

$(LINKER_SCRIPT): $(LD_DIR)/$(LINKER_SCRIPT_TEMPLATE)
	$(CPREPROC) -xc -I$(INC_DIR) $(LD_DIR)/$(LINKER_SCRIPT_TEMPLATE) -o $(LINKER_SCRIPT)

.phony: all
all: $(ELF)	

.phony: release
release:
	@echo $(TEST)
	make "BUILD=release"

.phony: debug
debug:
	make "BUILD=debug"

.phony: clean
clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) $(LINKER_SCRIPT)

.phony: count
count: # Pour le fun
	@find . -name "" $(foreach e,$(EXT_TO_COMPILE) $(EXT_OTHERS), -or -name "*.$(e)") | xargs wc -l | awk 'BEGIN {total = 0;}{if (NF == 2 && $$2 == "total") total += $$1;} END {print "Total lines count : " total}'

.phony: genlib
genlib: 	$(OBJ) | $(BIN_DIR)
	@ar rcs $(BIN_DIR)/$(LIB_NAME).a $^
	@echo "Generating library $(LIB_NAME).a"

-include $(OBJ:.o=.d)
