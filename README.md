# Ceserver v0.0.01(alpha)

Currently, this project is just a way for me to explore my hobby. I'm trying to build a backend or possibly a full-stack (in the future) web framework using the C programming language.

## Project Structure

- **lib/**: Berisi header file untuk proyek.
- **/main.c**: Place your main function here
- **.gitignore**: File yang menentukan file mana yang diabaikan oleh Git.
- **README.md**: Dokumentasi proyek.
- **Makefile**: Berisi instruksi untuk kompilasi proyek.

## Instalation

1. Use Visual Studio Code text editor (recommended).
2. Install the C/C++ extension on VSCode
3. clone this project
4. just import lib/cesocket.h and use on your main file project

# Development
Nvidia Toolkit (cudart.lib)
WS2 (-lws2_32)

## Compile code

1. code your main function on main.c file
2. compile with command "gcc main.c -lws2_32 -o program_name.exe" (windows)

## Compile With Cuda

1. (compile cu program) "nvcc -c [cuda_code_name.cu] -o [cuda_lib_name.obj] -Xcompiler '/GS-'"
2. (compile c program) "gcc -c main.c -o main.o"
3. (compile both) "gcc main.o [cuda_lib_name.obj] -L. -lcudart -lws2_32 -lmsvcrt -o [program_name.exe]"

-Xcompiler '/GS-' = deactivate buffer overflow protection
-lws2_32 = windows websocket lib
-lcudart = cuda lib