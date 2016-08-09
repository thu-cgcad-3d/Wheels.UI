# ui
project (wheels.ui VERSION 0.1 LANGUAGES CXX)

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


AddWheelsProject(ui)

add_dependencies(${PROJECT_NAME} wheels.geometry)
target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    $<BUILD_INTERFACE:  ${wheels.geometry_INCLUDE_DIRS} ${Qt_INCLUDES}> 
    $<INSTALL_INTERFACE:${wheels.geometry_INCLUDE_DIRS} ${Qt_INCLUDES}>
    PRIVATE)
target_link_libraries(${PROJECT_NAME}.test ${Qt_LIBS})
target_include_directories(${PROJECT_NAME}.test
    PUBLIC
    $<BUILD_INTERFACE:  ${wheels.geometry_INCLUDE_DIRS} ${Qt_INCLUDES}>
    $<INSTALL_INTERFACE:${wheels.geometry_INCLUDE_DIRS} ${Qt_INCLUDES}>
    PRIVATE)

# Expose public includes to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/include 
    ${wheels.geometry_INCLUDE_DIRS} ${Qt_INCLUDES}
    CACHE EXTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
set(${PROJECT_NAME}_LIBS wheels.ui.lib ${wheels.geometry_LIBS} ${Qt_LIBS}
    CACHE EXTERNAL "${PROJECT_NAME}: Link Libraries" FORCE)