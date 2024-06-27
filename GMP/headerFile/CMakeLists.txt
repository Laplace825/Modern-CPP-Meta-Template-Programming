add_executable(headerFile main.cc header.cc)
target_include_directories(headerFile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)