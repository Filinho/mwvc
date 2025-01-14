#Arquivos pcg devem estar na pasta include

cmake_minimum_required(VERSION 3.10)
include(CPack)
project(mwvc)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall -Wunused -Wno-unknown-pragmas -fPIC -DIL_STD -fexceptions  -std=c++0x ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -lm -lpthread -ldl -mtune=native")

find_package(Boost 1.49.0 COMPONENTS program_options chrono)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(main main.cpp)
target_link_libraries(main ${Boost_LIBRARIES})


