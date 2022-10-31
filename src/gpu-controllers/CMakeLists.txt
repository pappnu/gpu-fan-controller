add_library(
    gpu-controllers STATIC
    gpucontrollers.hpp
    nvidiacontroller.cpp
)
target_include_directories(gpu-controllers PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_features(gpu-controllers PUBLIC cxx_std_17)
set_target_properties(gpu-controllers PROPERTIES CXX_EXTENSIONS OFF)
