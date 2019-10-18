# Debug mode:
DEBUG   = yes

CC        = g++
LIB       = libtexo.a

CPPFLAGS  = -Wall -ansi
ifeq '$(DEBUG)' 'yes'
CPPFLAGS += -g -O0
else
CPPFLAGS += -O3
endif

SRC_DIR   = src
SRC       = $(wildcard $(SRC_DIR)/*.cpp)
OBJ       = $(SRC:.cpp=.o)

INCLUDE   = -I../lib/scriptpp


default: $(LIB)

ifneq "clean" "$(MAKECMDGOALS)"
-include deps.mk
endif

deps.mk: $(SRC)
	$(CC) -MM $^ > $@

%.o: %.cpp %.hpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(LIB): $(OBJ)
	ar rcs $@ $^

clean:
	rm -f $(OBJ) $(LIB) deps.mk