CC=gcc -Wall -g
CLOG=log

COMP=script -q -c "$(CC) -c -lssl -lcrypto -L ./qrgen -lqrcodegen -o $@ $<" $(CLOG)/$(@F)
LINK=script -q -c "$(CC) -lssl -lcrypto -L ./qrgen -lqrcodegen -o $@ $^" $(CLOG)/$(@F)

all: log submission

submission: submission.c qr.o
	$(LINK)

qr.o: qr.c qr.h qrgen/qrcodegen.o
	$(COMP)

qrgen/qrcodegen.o:
	$(MAKE) -C qrgen all

tests: test_qr

test_qr: test_qr.c qr.o qrgen/qrcodegen.o
	$(LINK)

log:
	mkdir log

clean:
	rm -rf *.dSYM 
	rm -f log/*
	rm -f submission *.o test_qr
	$(MAKE) -C qrgen/ clean
