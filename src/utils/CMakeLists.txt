add_library(
    utils STATIC
    utils.hpp
    utils.cpp
)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_features(utils PUBLIC cxx_std_17)
set_target_properties(utils PROPERTIES CXX_EXTENSIONS OFF)
