cmake_minimum_required(VERSION 3.26)
project(Dino-Shift)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
        ptsd
        URL https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# 設定資源資料夾路徑
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GA_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE GA_RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

# Include 目錄
target_include_directories(${PROJECT_NAME}
        SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
        SDL2::SDL2main
        PTSD
)
