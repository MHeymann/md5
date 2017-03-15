### DIRS ##################################################################

SRC = src
OBJ = obj

### OBJS ##################################################################

OBJS 		= $(OBJ)/md5.o
TEST_EXES 	= testmd5
EXES		= md5sum

### FLAGS #################################################################

FLAGS = -Wall -Wextra -pedantic -O3
DBGFLAGS = #-DDEBUG

### COMMANDS ##############################################################

CC		= gcc
RM 		= rm -v -f
COMPILE = $(CC) $(FLAGS) $(DBGFLAGS)

### RULES #################################################################

all: $(EXES) $(TEST_EXES)

clean_all: clean all

tests: $(TEST_EXES)

md5sum: $(OBJS) $(SRC)/md5sum.c
	$(COMPILE) -o $@ $^ $(LFLAGS)

testmd5: $(OBJS) $(SRC)/testmd5.c
	$(COMPILE) -o $@ $^ $(LFLAGS)


$(OBJ)/%.o: $(SRC)/%.c
	$(COMPILE) -c -o $@ $^

.PHONY:	clean
clean:
	$(RM) $(OBJS) $(EXES) $(TEST_EXES)
