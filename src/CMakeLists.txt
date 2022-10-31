add_subdirectory(gpu-controllers)
add_subdirectory(fan-controller)
add_subdirectory(utils)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${X11_INCLUDE_DIR}
    ${NVCtrl_INCLUDE_DIR}
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${X11_LIBRARIES}
    ${NVCtrl_LIBRARY}
    fan-controller
    utils
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
