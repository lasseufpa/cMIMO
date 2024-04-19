Link do site com a documentação feita pelo doxygen: https://jvictorferreira3301.github.io/MIMO/



# Engineering Project II - Implementation of MIMO Digital Communication System

This project was developed for the Engineering Project II course taught in the second semester at the Faculty of Computing and Telecommunications of the Federal University of Pará. The objective of this project is to develop a C library containing functions that implement matrix operations. The resulting library will be used to implement a Multiple-in, Multiple-out (MIMO) Digital Communication System in C language. The system to be implemented is described in the image below:

<img src="Doxyfiles/figures/mimosystem.png" alt="mimo-implementação">

### 💻 Requisites

- GSL Library (`gsl/gsl_linalg.h`): The GNU Scientific Library (GSL) is a numerical library for C and C++. In this case, the linear algebra part is being used.

- Makefile: a utility that automates the process of building programs, managing dependencies between source files.

To install the dependencies above run `install_dependencies.sh` with the following command in the terminal:

```bash
❯ sh install_dependencies.sh
```

# Makefile Guide

This Makefile is used to compile and clean a C project that includes the `pds_telecom.c` file and a matrix library. Here's a step-by-step guide on how to use this Makefile.

## Variables

- `src`: The directory where the project's source code is located.
- `matrix`: The directory where the source code for the matrix library is located.
- `obj`: The directory where the object files and the executable will be placed.
- `out`: The name of the executable.
- `w`: Warning flags for the gcc compiler.
- `gsl`: Flags to link the GSL library.
- `math`: Flag to link the math library.
- `font`: The path to the `pds_telecom.c` file.
- `test_arq`: A pattern that matches the test files.

## Rules

- `all`: This is the default rule. It creates the object directory (if needed) and compiles the executable.
- `$(obj)/$(out)`: This rule compiles the executable. It depends on the object file of the matrix library and the `pds_telecom.c` file.
- `$(obj)/matrix.o`: This rule compiles the object file of the matrix library. It depends on the source code file of the matrix library.
- `$(obj)`: This rule creates the object directory, if it doesn't already exist.
- `test`: This rule runs the executable.
- `clean`: This rule removes the object directory and all test files.

## How to Use

To compile the project, run the following command in the terminal:

```bash
❯ make
```

To run the project, run the following command in the terminal:

```bash
❯ make test
```

To clean the project (remove the object directory and all test files), run the following command in the terminal:

```bash
❯ make clean
```

