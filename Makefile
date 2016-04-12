#
# EECS 678
#

CC = gcc
INC = -I.
FLAGS = -Wall -Wextra -Werror -Wno-unused -g

LIBPRIQUEUE_DIR          = libpriqueue/
LIBPRIQUEUE_TEST_DIR     = $(LIBPRIQUEUE_DIR)tests/
LIBPRIQUEUE_TEST_SRCS    = $(wildcard $(LIBPRIQUEUE_TEST_DIR)*.c)
LIBPRIQUEUE_TEST_OBJECTS = $(patsubst %.c, %.o, $(LIBPRIQUEUE_TEST_SRCS))
SCHEMES                  = fcfs sjf psjf pri ppri rr1 rr2 rr4

all: simulator queuetest doc/html

doc/html: doc/Doxyfile libpriqueue/libpriqueue.c libscheduler/libscheduler.c
	doxygen doc/Doxyfile

simulator: simulator.o libscheduler/libscheduler.o libpriqueue/libpriqueue.o
	$(CC) $^ -o $@

sim_runs: simulator
	for scheme in $(SCHEMES) ; do \
		echo $$scheme ; \
		runghc test_results.hs 1 $$scheme ; \
	done

queuetest: queuetest.o libpriqueue/libpriqueue.o $(LIBPRIQUEUE_TEST_OBJECTS) test_utils.o 
	$(CC) $^ -o $@

queuetest.o: queuetest.c
	$(CC) -c $(FLAGS) $(INC) $< -o $@

libscheduler/libscheduler.o: libscheduler/libscheduler.c libscheduler/libscheduler.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@

libpriqueue/libpriqueue.o: libpriqueue/libpriqueue.c libpriqueue/libpriqueue.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@

simulator.o: simulator.c libscheduler/libscheduler.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@

$(LIBPRIQUEUE_TEST_DIR)%.o: $(LIBPRIQUEUE_TEST_DIR)%.c $(LIBPRIQUEUE_TEST_DIR)%.h 
	$(CC) -c $(FLAGS) $(INC) $< -o $@

test_utils.o: test_utils.c test_utils.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@


.PHONY : clean
clean:
	rm -rf simulator queuetest *.o libscheduler/*.o libpriqueue/*.o doc/html $(LIBPRIQUEUE_TEST_DIR)*.o *~ libscheduler/*~ libpriqueue/*~ $(LIBPRIQUEUE_TEST_DIR)*~
