cmake_minimum_required(VERSION 3.15)
project(22s_final_proj)

set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCES
        *.h
        *.cpp
        rapidjson/*.h
        rapidjson/error/*.h
        rapidjson/internal/*.h
        rapidjson/msinttypes/*.h
)

add_executable(22s_final_proj ${SOURCES})
