cmake_minimum_required(VERSION 3.10)
project(SGBD_HashIndex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(HashIndex
    src/main.cpp
    src/record.cpp
    src/bucket.cpp
    src/directory.cpp
    src/hash_index.cpp
)
