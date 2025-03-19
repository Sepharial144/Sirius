
cmake_minimum_required (VERSION 3.20)

project("Sirius" VERSION 0.0.2 DESCRIPTION "Software for graph analisys")

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Включите подпроекты.
add_subdirectory ("SiriusWhite")
#add_subdirectory ("SiriusBlack")
#add_subdirectory ("D3DExample")
