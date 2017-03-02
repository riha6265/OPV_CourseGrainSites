CC=g++
CFLAGS = -Wall -Wextra -std=c++11 -pedantic -g 

PROG = testCluster.cpp
SRC = $(PROG).c libCluster.cpp
HDR = cluster.h
OBJ = $(SRC:.c=.o)

$(PROG): $(OBJ)
$(OBJ): $(HDR)
TAGS: $(SRC) $(HDR)
	etags &^

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJ) 