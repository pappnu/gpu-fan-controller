add_library(
    fan-controller STATIC
    fancontroller.hpp
    fancontroller.cpp
)
target_include_directories(fan-controller PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(fan-controller PUBLIC gpu-controllers)

target_compile_features(fan-controller PUBLIC cxx_std_20)
set_target_properties(fan-controller PROPERTIES CXX_EXTENSIONS OFF)
