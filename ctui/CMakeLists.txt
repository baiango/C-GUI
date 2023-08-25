# CMakeList.txt : CMake project for C-GUI, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("C-GUI")

# Add source to this project's executable.
file(GLOB EXTERNAL_SOURCES "external/source/*.c" "src/gllib/*.c")
add_executable(C-GUI "main.c" ${EXTERNAL_SOURCES})
target_include_directories(C-GUI PRIVATE "${CMAKE_SOURCE_DIR}/external/include")
target_link_directories(C-GUI PRIVATE "${CMAKE_SOURCE_DIR}/external/lib")
target_link_libraries(C-GUI PRIVATE glfw3.lib)

# Copy GLSL shader files to build directory
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/src/shaders/")
file(COPY "${CMAKE_SOURCE_DIR}/src/shaders/rdrect.vert"
          "${CMAKE_SOURCE_DIR}/src/shaders/rdrect.frag"
    DESTINATION "${CMAKE_BINARY_DIR}/src/shaders/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET C-GUI PROPERTY C_STANDARD 99)
endif()

# TODO: Add tests and install targets if needed.
