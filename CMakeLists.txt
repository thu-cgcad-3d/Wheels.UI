# ui
project (wheels.ui VERSION 0.1 LANGUAGES CXX)

AddWheelsModule(ui core tensor io math geometry)

target_link_libraries(${PROJECT_NAME} ${Qt_LIBS})
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    $<BUILD_INTERFACE:  ${Qt_INCLUDES}> 
    $<INSTALL_INTERFACE:${Qt_INCLUDES}>
    PRIVATE)
target_link_libraries(${PROJECT_NAME}.test ${Qt_LIBS})
target_include_directories(${PROJECT_NAME}.test
    PUBLIC
    $<BUILD_INTERFACE:  ${Qt_INCLUDES}>
    $<INSTALL_INTERFACE:${Qt_INCLUDES}>
    PRIVATE)

# Expose public includes to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include 
    ${Qt_INCLUDES}
    CACHE EXTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
set(${PROJECT_NAME}_LIBS 
    ${Qt_LIBS}
    CACHE EXTERNAL "${PROJECT_NAME}: Link Libraries" FORCE)