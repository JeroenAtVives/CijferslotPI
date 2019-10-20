cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Cijferslot)

add_executable(Cijferslot main.c)

target_link_libraries(Cijferslot PJ_RPI)
