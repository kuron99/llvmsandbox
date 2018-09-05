cmake_minimum_required(VERSION 3.10)
project(llvmsandbox)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-elide-constructors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,integer,nullability -fno-sanitize-recover=address,undefined,integer,nullability")

set(LLVM_CONFIG_BIN "/usr/bin/llvm-config-6.0")

execute_process(
    COMMAND
    ${LLVM_CONFIG_BIN} --libs
    OUTPUT_VARIABLE
    LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

execute_process(
    COMMAND
    ${LLVM_CONFIG_BIN} --includedir
    OUTPUT_VARIABLE
    LLVM_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

execute_process(
    COMMAND
    ${LLVM_CONFIG_BIN} --libdir
    OUTPUT_VARIABLE
    LLVM_LIBRARY_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

execute_process(
    COMMAND
    ${LLVM_CONFIG_BIN} --system-libs
    OUTPUT_VARIABLE
    LLVM_SYSTEM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

execute_process(
    COMMAND
    ${LLVM_CONFIG_BIN} --version
    OUTPUT_VARIABLE
    LLVM_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

execute_process(
    COMMAND
    ${LLVM_CONFIG_BIN} --build-mode
    OUTPUT_VARIABLE
    LLVM_BUILD_TYPE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

execute_process(
    COMMAND
    ${LLVM_CONFIG_BIN} --has-rtti
    OUTPUT_VARIABLE
    LLVM_HAS_RTTI
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

message(STATUS "LLVM version    : ${LLVM_VERSION}")
message(STATUS "LLVM includedir : ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libdir     : ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM build type : ${LLVM_BUILD_TYPE}")
message(STATUS "LLVM has rtti   : ${LLVM_HAS_RTTI}")

set(CMAKE_CXX_STANDARD 11)

file(GLOB SRCS "*.cpp")

foreach(src IN LISTS SRCS)
    get_filename_component(fname "${src}" NAME_WE)
    add_executable("${fname}" ${src})

    link_directories(${LLVM_LIBRARY_DIRS})
    target_include_directories(${fname} PRIVATE ${LLVM_INCLUDE_DIRS})
    target_link_libraries(${fname}
            PRIVATE
            ${LLVM_LIBS}
            ${LLVM_SYSTEM_LIBS})
endforeach()

