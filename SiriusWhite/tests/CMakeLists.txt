enable_testing()
find_package(GTest REQUIRED)

## --- Unit testing
file(GLOB_RECURSE UNIT_SRC_FILES "unit/*.hpp" "unit/*.cpp")

list(FILTER INTERNAL_ENGINE_SOURCE EXCLUDE REGEX "main.cpp")

set(TEST_EXECUTION sirius-white-unit-test)

add_executable(${TEST_EXECUTION} ${UNIT_SRC_FILES} ${INTERNAL_ENGINE_SOURCE})

target_link_libraries(${TEST_EXECUTION} PRIVATE 
	GTest::gmock GTest::gtest 
	GTest::gmock_main 
	GTest::gtest_main 
	${PACKAGE_LIBRARIES}
)

target_include_directories(${TEST_EXECUTION} 
	PUBLIC 
	"${CMAKE_SOURCE_DIR}/SiriusWhite/source"
	${PACKAGE_INCLUDE_DIR}
)

## ! --- Unit testing