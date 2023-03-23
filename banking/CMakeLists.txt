cmake_minimum_required(VERSION 3.4)

project(banking)

add_library(banking STATIC Account.cpp Transaction.cpp)
