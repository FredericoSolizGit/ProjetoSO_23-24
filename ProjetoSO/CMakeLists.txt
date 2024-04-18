cmake_minimum_required(VERSION 3.27)
project(ProjetoSO C)

set(CMAKE_C_STANDARD 17)

add_executable(ProjetoSO monteCarlo.c
        requisito_1.c
        requisito_1.h
        monteCarlo.h
        requisito_2.c
        requisito_2.h)
