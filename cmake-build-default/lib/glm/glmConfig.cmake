set(GLM_VERSION "0.9.8")
set(GLM_INCLUDE_DIRS "/Users/thomas/Documents/COMP371/lib/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
