# Try to find NVCtrl
# Once done, this will define
#
# NVCtrl_FOUND          - system has NVCtrl
# NVCtrl_INCLUDE_DIR    - the NVCtrl include directories
# NVCtrl_LIBRARY        - link these to use NVCtrl

# find the NVCtrl lib path
find_library(
    NVCtrl_LIBRARY XNVCtrl
    PATHS /usr/lib
)

# find the NVCtrl include directory
find_path(
    NVCtrl_INCLUDE_DIR NVCtrlLib.h
    PATHS /usr/include/NVCtrl
)

# Check if include dir and lib path are found
if(NVCtrl_INCLUDE_DIR AND NVCtrl_LIBRARY)
	set(NVCtrl_FOUND TRUE)
else ()
	set(NVCtrl_FOUND FALSE)
endif()

# Display result
if (NOT NVCtrl_FOUND)
    if (NVCtrl_FIND_REQUIRED)
        # fatal error
        message(FATAL_ERROR "Could not find NVCtrl")
    elseif(NOT NVCtrl_FIND_QUIETLY)
        # error but continue
        message("Could not find NVCtrl")
    endif()
endif()
