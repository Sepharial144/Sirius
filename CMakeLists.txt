cmake_minimum_required (VERSION 3.20)

project("Sirius" VERSION 0.0.2 DESCRIPTION "Quates data collector" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE HDR_FILES "source/*.hpp")
file(GLOB_RECURSE SRC_FILES "source/*.cpp")

find_library(WSOCK32_LIBRARY wsock32.lib)

set(SOURCE_DEPENDENCIES ${HDR_FILES} ${SRC_FILES})

message(STATUS "Dependencies: ${SOURCE_DEPENDENCIES}")

set(LIB_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} ${SOURCE_DEPENDENCIES})

#set_target_properties(${PROJECT_NAME} 
#    PROPERTIES 
#    PUBLIC_HEADER "${HDR_FILES}")

target_link_libraries(${PROJECT_NAME} ${LIB_DEPENDENCIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${NET_INCLUDE_DIR})

message(STATUS "INSTALL PREFIX: ${CMAKE_INSTALL_PREFIX}")

add_subdirectory(tests)


# TODO: finish configure project
# fix execution install folder, does not work
message(STATUS "Build path: ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        ARCHIVE DESTINATION "debug/sirius/lib"
        RUNTIME DESTINATION "debug/sirius/bin"
        PUBLIC_HEADER DESTINATION "debug/sirius")
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        ARCHIVE DESTINATION "release/sirius/lib"
        RUNTIME DESTINATION "release/sirius/bin"
        PUBLIC_HEADER DESTINATION "release/sirius")