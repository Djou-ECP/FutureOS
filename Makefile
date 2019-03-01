# ---------------------------- #
# -- SortieDeCache Makefile -- #
# ---------------------------- #

# -- File list ----------
FILE = *.cpp TD1/timespec.cpp


# -- Paths ----------
SRC_PATH = TD3
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include

# -- Macros ----------
CC = g++

# -- Flags ----------
# PAS DE VECTORISATION -----
# C_OPTIMISATION_FLAGS = -O2 -std=c++11 -march=native -fno-tree-vectorize 


C_OPTIMISATION_FLAGS = -O0 -std=c++11 -march=native -pthread -lrt

# VECTORISATION FORCEE
#C_OPTIMISATION_FLAGS = -O3 -std=c++11 -march=native -ftree-vectorize -fopt-info

C_INC_FLAGS = -I$(INC_PATH)

CFLAGS = $(C_OPTIMISATION_FLAGS) $(C_INC_FLAGS)
LDFLAGS = $(C_OPTIMISATION_FLAGS) $(C_INC_FLAGS)

# -- Final product ----------
PRODUCT   = simd_somme.exe

# -- src and obj List ----------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(INC) $(LIB_LIB_PATH) -lm

# -- Cleanup ----------
clean:
	rm -f $(OBJ)
	rm -f exe/$(PRODUCT)
