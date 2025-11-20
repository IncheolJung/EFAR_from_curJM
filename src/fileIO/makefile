LIB = libfileIO.a
SRC = readfiles.cpp writefiles.cpp
OBJ = $(SRC:.cpp=.o)
UTILS = ../utils
CXX = g++
LD = ar rcs


default: $(LIB)

$(OBJ): %.o: %.cpp
	$(CXX) -c $(OPT) $< -I$(UTILS) -o $@

$(LIB): $(OBJ)
	$(LD) $(LIB) $(OBJ)
	cp $(LIB) ..

clean:
	rm -f $(LIB) $(OBJ)