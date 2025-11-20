default: all

all: 
	@cd src; make OPT="-O3" BIN="../bin/"

gdb: 
	@cd src; make OPT="-O0" BIN="../bin/"

rm:
	@cd src; make rm
	rm -rf bin/*

clean:
	@cd src; make clean
	rm -rf bin/*
