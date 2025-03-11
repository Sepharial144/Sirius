message(STATUS "Load ... package.cmake")

if(DEFINED VCPKG_CONFIG)
	include(${VCPKG_CONFIG})
	find_package(OpenGL REQUIRED)
	find_package(glfw3 CONFIG REQUIRED)
	find_package(GLEW REQUIRED)
endif()

# include imgui and implot library
file(GLOB_RECURSE IMGUI  "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/*.cpp")
file(GLOB_RECURSE IMPLOT "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/implot/*.cpp")
list(FILTER IMPLOT EXCLUDE REGEX "implot_demo.cpp")

# include directories
SET(PACKAGE_SOURCES
	${IMGUI}
	${IMPLOT}
)

# include directories
SET(PACKAGE_DIRECTORIES
	"${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui"
	"${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/backends"
	"${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/implot"
)

# link libraries
SET(PACKAGE_LIBRARIES
	OpenGL::GL
	GLEW::GLEW
	glfw
)