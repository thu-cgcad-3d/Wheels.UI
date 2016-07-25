# ui
get_filename_component(folder ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project (wheels.${folder} VERSION 0.1 LANGUAGES CXX)

set (sources "")
set (test_sources "")

file (GLOB sources 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
file (GLOB test_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.test.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.test.hpp"
)

if(test_sources)
    list (REMOVE_ITEM sources ${test_sources})
endif()
source_group ("src" FILES ${sources})
source_group ("src" FILES ${test_sources})

# add_qt
set (Qt_DIR "" CACHE PATH "Qt root directory here")
set (Qt_MODULES_REQUIRED Core Gui Widgets OpenGL)
list (APPEND CMAKE_PREFIX_PATH ${Qt_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR on)
set (Qt_PATH ${Qt_DIR}/bin)
set (Qt_INCLUDES "")
set (Qt_LIBS "")
foreach (_M ${Qt_MODULES_REQUIRED})
    find_package ("Qt5${_M}" REQUIRED)
    string(COMPARE EQUAL ${_M} OpenGL _IsOpenGL)
    if(${_IsOpenGL})
        find_package (OPENGL REQUIRED)
        list(APPEND Qt_INCLUDES ${OPENGL_INCLUDE_DIR})
        list(APPEND Qt_LIBS ${OPENGL_LIBRARIES})
    endif()
    list(APPEND Qt_LIBS "Qt5::${_M}")
endforeach()
if (MSVC)
    message (WARNING 
        "do not forget to add QT_QPA_PLATFORM_PLUGIN_PATH=${Qt_DIR}/plugins/platforms to env")
endif ()

if (MSVC)
    add_definitions("/DNOMINMAX /wd4503")
    add_definitions("/Dwheels_data_dir_str=\"${CMAKE_CURRENT_SOURCE_DIR}/data/\"")
else ()
    add_definitions("-Dwheels_data_dir_str=\"${CMAKE_CURRENT_SOURCE_DIR}/data/\"")
endif ()

# lib
add_library(${PROJECT_NAME} ${sources})
add_dependencies(${PROJECT_NAME} wheels.vision)
target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include 
        ${wheels.vision_INCLUDE_DIRS} ${Qt_INCLUDES}> 
    $<INSTALL_INTERFACE:include ${wheels.vision_INCLUDE_DIRS} ${Qt_INCLUDES}>
    PRIVATE 
    src)

# Expose public includes to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/include 
    ${wheels.vision_INCLUDE_DIRS} ${Qt_INCLUDES}
    CACHE EXTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
set(${PROJECT_NAME}_LIBS ${wheels.vision_LIBS} ${Qt_LIBS}
    CACHE EXTERNAL "${PROJECT_NAME}: Link Libraries" FORCE)

install(TARGETS ${PROJECT_NAME}
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION modules/${folder}/include)
install(DIRECTORY src/ DESTINATION modules/${folder}/src
    PATTERN "*.test.*" EXCLUDE
    PATTERN "*.cpp" EXCLUDE)
install(DIRECTORY data/ DESTINATION modules/${folder}/data)

# test
add_executable(${PROJECT_NAME}.test ${test_sources})
add_dependencies(${PROJECT_NAME}.test ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}.test ${PROJECT_NAME} ${Qt_LIBS})
target_include_directories(${PROJECT_NAME}.test
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include 
        ${wheels.vision_INCLUDE_DIRS} ${Qt_INCLUDES} ${Catch_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include 
        ${wheels.vision_INCLUDE_DIRS} ${Qt_INCLUDES} ${Catch_INCLUDE_DIRS}>
    PRIVATE
    src)
