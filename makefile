all:
	@cd Equal; make
	@cd Plive; make
	@cd Mkbkp; make

clean:
	@cd Equal; make clean
	@cd Plive; make clean
	@cd Mkbkp; make clean
