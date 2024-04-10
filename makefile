src = ./src
matrix = ./src/matrix
obj = ./build
out = aplication
w = -W -Wall -pedantic
gsl = -lgslcblas -lgsl
math = -lm
font = ./src/MIMO/pds_telecom.c
test_arq = Test*

all: $(obj) $(out)

$(out): $(obj)/matrix.o $(font)
	@echo -e "\n=== Generanting the file $@... ==="
	gcc $< -o $(obj)/$@ $(font) $(gsl) $(math) $(w)
	@echo -e "\n=== To run the code from 'pds_telecom.c': run the file $(obj)/$@.exe or the rule command 'make test'!! ==="

$(obj)/matrix.o: $(matrix)/matrix.c  
	@echo -e "\n=== Generating the file $@... ==="
	gcc -c $< -J $(obj) -o $@ $(gsl) $(w)

$(obj):
	mkdir $(obj)
	
test: $(obj)/$(out)
	@ $(obj)/$(out)

clean:
	@echo -e "\n=== Starting the repository cleaning ==="
	rm -rf $(obj)/*.exe
	rm -rf $(obj)/
	rm -rf $(test_arq)

