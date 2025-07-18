cmake_minimum_required(VERSION 3.16)
project(WorkTimer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source files
set(SOURCES
    src/main.cpp
    src/worktimer.cpp
    src/worktimer_methods.cpp
    src/settings.cpp
)

set(HEADERS
    include/worktimer.h
    include/settings.h
)

# Create executable
add_executable(WorkTimer ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(WorkTimer PRIVATE include)

# Link Qt6 libraries
target_link_libraries(WorkTimer PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
)

# Windows specific settings
if(WIN32)
    set_target_properties(WorkTimer PROPERTIES
        WIN32_EXECUTABLE TRUE
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    # Copy sounds folder to output
    add_custom_command(TARGET WorkTimer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/sounds
        ${CMAKE_BINARY_DIR}/bin/sounds
    )
endif()

# Install
install(TARGETS WorkTimer
    RUNTIME DESTINATION bin
)

install(DIRECTORY sounds/
    DESTINATION bin/sounds
    FILES_MATCHING PATTERN "*.mp3"
) 