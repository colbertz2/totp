CC=gcc -Wall -g
CLOG=log

COMP=script -q -c "$(CC) -c -o $@ $<" $(CLOG)/$(@F)
LINK=script -q -c "$(CC) -o $@ $^" $(CLOG)/$(@F)

all: log submission

submission: submission.c
	$(LINK)

log:
	mkdir log

clean:
	rm -rf *.dSYM 
	rm -f log/*
	rm -f submission
