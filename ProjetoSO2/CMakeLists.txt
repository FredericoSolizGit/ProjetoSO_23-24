cmake_minimum_required(VERSION 3.27)
project(ProjetoSO2 C)

set(CMAKE_C_STANDARD 17)

add_executable(ProjetoSO2
        monteCarlo.c
        monteCarlo.h
        requisito_1_2ºfase.c
        requisito_1_2ºfase.h
        requisito_2_2ºfase.c
        requisito_2_2ºfase.h)
