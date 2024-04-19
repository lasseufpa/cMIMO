src = ./src
matrix = ./src/matrix
obj = ./build
out = aplication
w = -W -Wall -pedantic
gsl = -lgslcblas -lgsl
math = -lm
font = ./src/MIMO/pds_telecom.c
test_arq = Test*

all: $(obj) $(obj)/$(out)

$(obj)/$(out): $(obj)/matrix.o $(font)
	@echo -e "\n=== Generanting the file $@... ==="
	gcc $^ -o $@ $(gsl) $(math) $(w)
	@echo -e "\n=== To run the code from 'pds_telecom.c': run the file $@ or the rule command 'make test'!! ==="

$(obj)/matrix.o: $(matrix)/matrix.c $(obj)
	@echo -e "\n=== Generating the file $@... ==="
	gcc -c $< -o $@ $(gsl) $(w)

$(obj):
	mkdir -p $(obj)
	
test: $(obj)/$(out)
	@./$(obj)/$(out)

clean:
	@echo -e "\n=== Starting the repository cleaning ==="
	rm -rf $(obj)/*
	rm -rf $(test_arq)