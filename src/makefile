EXE = calc_efar.x
LIB = libfileIO.a libutils.a

SRCCC = main.cc
SRCCXX = farfield.cpp
OBJCC = $(SRCCC:%.cc=%.o)
OBJCXX = $(SRCCXX:%.cpp=%.o)
OBJ = $(OBJCC) $(OBJCXX)

OMPFLG = -fopenmp

CC = g++
CXX = g++
LD = g++



default: $(EXE)

$(OBJCC): %.o: %.cc
	$(CC) $(OPT) -c $< -o $@

$(OBJCXX): %.o: %.cpp
	$(CXX) $(OPT) $(OMPFLG) -c $< -o $@

$(LIB):
	@cd fileIO && make OPT=$(OPT)
	@cd utils  && make OPT=$(OPT)

$(EXE): $(OBJCC) $(OBJCXX) $(LIB)
	$(LD) $(OBJ) $(LIB) $(OMPFLG) -o $(EXE)
	cp $(EXE) $(BIN)

rm:
	rm -f $(OBJCXX)
# 	rm libfileIO.a
# 	@cd fileIO && make clean

clean:
	rm -f $(EXE) $(LIB) $(OBJ)
	@cd fileIO && make clean
	@cd utils  && make clean