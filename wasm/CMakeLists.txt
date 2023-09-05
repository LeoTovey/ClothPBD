cmake_minimum_required(VERSION 3.0.0)
project(PBD)


include_directories(deps/eigen)
include_directories(./)
file(GLOB HEADERS "${CMAKE_CURRENT_LIST_DIR}/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

set(EMSCRIPTENOPTIONS "SHELL:--post-js /Users/leo/threejs/ClothPBD/wasm/glue.js -s ALLOW_MEMORY_GROWTH=1 
-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']
-s EXPORT_ES6=1 -s MODULARIZE=1 -s --no-entry")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O1")
set(CMAKE_CXX_STANDARD 17)


add_executable(PBD wasm.cpp wasm_wrapper.cpp)

target_link_options(PBD PRIVATE ${EMSCRIPTENOPTIONS})
# set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
# set_target_properties(PBD PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1 ")
# set_target_properties(PBD PROPERTIES LINK_FLAGS    "-Os -s WASM=1 -s SIDE_MODULE=1 -s STANDALONE_WASM --no-entry")



