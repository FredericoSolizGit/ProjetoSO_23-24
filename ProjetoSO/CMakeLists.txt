cmake_minimum_required(VERSION 3.27)
project(ProjetoSO C)

set(CMAKE_C_STANDARD 17)

add_executable(ProjetoSO monteCarlo.c
        requisito_1.c
        requisito_1.h
        monteCarlo.h
        requisito_2.c
        requisito_2.h
        requisito_3.h
        requisito_3.c
        requisito_4.c
        requisito_4.h
        requisito_5.c
        requisito_5.h
)
