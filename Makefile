SRC_DIR = Src
OBJ_DIR = Obj
INC_DIR = Inc

CXX=g++
FLAGS= -g -Wall -Wextra -I$(INC_DIR)
THREADS = -pthread
LIBS= -lm
RM= rm -f
EXN= -o a.out	#name of the executable

_SRCS = main.cpp MVC.cpp
SRCS = $(patsubst %,$(SRC_DIR)/%,$(_SRCS))

_OBJS = main.o MVC.o
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

_DEPS = main.h ParGreedy.h util.h ctpl_stl.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX)  -c -o $@ $< $(FLAGS)

program: $(OBJS)
	$(CXX)  $(THREADS) -o $@ $^ $(FLAGS) $(LIBS)

.PHONY: clean

clean:
	${RM} $(OBJ_DIR)/*.o *.out
