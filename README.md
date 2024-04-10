Link do site com a documentação feita pelo doxygen: https://jvictorferreira3301.github.io/MIMO/



# Engineering Project II - Implementation of MIMO Digital Communication System

This project was developed for the Engineering Project II course taught in the second semester at the Faculty of Computing and Telecommunications of the Federal University of Pará. The objective of this project is to develop a C library containing functions that implement matrix operations. The resulting library will be used to implement a Multiple-in, Multiple-out (MIMO) Digital Communication System in C language. The system to be implemented is described in the image below:

<img src="./figures/mimosystem.png" alt="mimo-implementação">

### 💻 Prerequisites

- GSL Library (`gsl/gsl_linalg.h`): The GNU Scientific Library (GSL) is a numerical library for C and C++. In this case, the linear algebra part is being used.

- Makefile: a utility that automates the process of building programs, managing dependencies between source files.

## Makefile Guide
The Makefile in this project is used to automate the building process of the C project. Here's a guide on how to use it:

### Variables

The Makefile sets some variables for directories and flags used in the compilation process:

- `src`: The directory containing the source files.
- `matrices`: The directory containing matrix files.
- `obj`: The directory where object files and the executable will be stored.
- `out`: The name of the executable file.
- `w`: Warning flags for the gcc compiler.
- `gsl`: Flags for linking the GSL (GNU Scientific Library) library.
- `math`: Flag for linking the math library.
- `font`: The main source file of the project.
- `test_file`: Filename pattern for test files.

### Commands

- `all`: This is the default command. It creates the `obj` directory (if it doesn't exist) and compiles the project.
- `$(out)`: This command compiles the main source file and links with the GSL library and the math library.
- `$(obj)/matrices.o`: This command compiles the matrix file and generates an object file.
- `$(obj)`: This command creates the `obj` directory if it doesn't exist.
- `test`: This command runs the executable file.
- `clean`: This command cleans the `obj` directory and removes all test files.

To use the Makefile, you can type `make <command>` in the terminal, replacing `<command>` with the command you want to execute. For example, `make all` to compile the project, or `make clean` to clean.

