# labyrinth-generator

This project is a little labyrinth generator written in C. It is comprised of 2 main files :
- lgen.c, that generates a labyrinth and writes the walls to a text file ;
- bmpgen.c, that takes this text file as input and creates a BMP file out of it.

## Purpose

The goal is to experiment image handling, without an API, therefore in a low level way, and I always find it pleasant to make things in C (oldie but goldie, as they say !).

Moreover, writing to a raw BMP file requires handling data byte by byte, and forced me to get some documentation about how BMP files are structured in the file system (header and pixel array), and it is always an interesting thing to do.

## Dependencies & build

**Packages needed :**

I used a Makefile and GCC for the compilation, so the `make` command and the compiler are the only dependencies in this project.

**Build instructions :**

*Compile into a native executable using the provided Makefile.*

## Contributors

- Enzo CRANCE (ecrance)
