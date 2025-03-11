message(STATUS "Load ... package.cmake")

if(DEFINED VCPKG_CONFIG)
	include(${VCPKG_CONFIG})
endif()

# include directories
SET(PACKAGE_DIRECTORIES
	"${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/
	"${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/backends
)

# link libraries
SET(PACKAGE_LIBRARIES
)