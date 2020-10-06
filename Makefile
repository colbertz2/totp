CC=gcc -Wall -g
CLOG=log

COMP=script -q -c "$(CC) -c -lssl -lcrypto -lgd -lqrencode -o $@ $<" $(CLOG)/$(@F)
LINK=script -q -c "$(CC) -lssl -lcrypto -lgd -lqrencode -o $@ $^" $(CLOG)/$(@F)

all: log submission

submission: submission.c otp.o qr.o qrgen/qrcodegen.o
	$(LINK)

qr.o: qr.c qr.h
	$(COMP)

qrgen/qrcodegen.o:
	$(MAKE) -C qrgen all

otp.o: otp.c otp.h
	$(COMP)

tests: test_qr test_otp

test_qr: test_qr.c qr.o qrgen/qrcodegen.o
	$(LINK)

test_otp: test_otp.c otp.o
	$(LINK)

log:
	mkdir log

clean:
	rm -rf *.dSYM 
	rm -f log/*
	rm -f submission *.o test_qr test_otp
	$(MAKE) -C qrgen/ clean
