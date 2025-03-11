
cmake_minimum_required (VERSION 3.20)

set(TARGET_NAME "SiriusBlack")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(package.cmake)
include(config.cmake)

file(GLOB_RECURSE SIRIUS_BLACK_SOURCE "source/*.hpp" "source/*.cpp")

add_executable(${TARGET_NAME} ${SIRIUS_BLACK_SOURCE})

add_subdirectory("tests")

target_include_directories(${TARGET_NAME} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/source"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/cl"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/logger"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/exception"
	${PACKAGE_DIRECTORIES}
)

target_link_libraries(${TARGET_NAME} 
    PRIVATE 
	${PACKAGE_LIBRARIES}
)

install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Debug
        ARCHIVE DESTINATION "debug/${TARGET_NAME}/lib"
        RUNTIME DESTINATION "debug/${TARGET_NAME}/bin"
        PUBLIC_HEADER DESTINATION "debug/${TARGET_NAME}")
install(TARGETS ${TARGET_NAME}
        CONFIGURATIONS Release
        ARCHIVE DESTINATION "release/${TARGET_NAME}/lib"
        RUNTIME DESTINATION "release/${TARGET_NAME}/bin"
        PUBLIC_HEADER DESTINATION "release/${TARGET_NAME}")