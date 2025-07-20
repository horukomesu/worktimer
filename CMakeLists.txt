cmake_minimum_required(VERSION 3.16)
project(WorkTimer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 configuration - will be overridden by CMakePresets.json in Qt Creator
if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/msvc2022_64")
endif()

# Find Qt6 (required for Qt Creator 11)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia UiTools)
message(STATUS "Found Qt6: ${Qt6_VERSION}")

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

set(RESOURCES
    resources.qrc
)

set(UI_FILES
    ui/worktimer.ui
)

# Create executable
add_executable(WorkTimer ${SOURCES} ${HEADERS} ${RESOURCES} ${UI_FILES})

# Enable Qt MOC
set_target_properties(WorkTimer PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Add UI files to AUTOUIC
set_target_properties(WorkTimer PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

# Include directories
target_include_directories(WorkTimer PRIVATE include)

# Link Qt6 libraries
target_link_libraries(WorkTimer PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::UiTools
)

# Windows specific settings
if(WIN32)
    set_target_properties(WorkTimer PROPERTIES
        WIN32_EXECUTABLE TRUE
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    # Set application icon for Windows
    set_target_properties(WorkTimer PROPERTIES
        WIN32_EXECUTABLE TRUE
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
    # Add icon to executable
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources/worktimer_icon.ico")
        set_target_properties(WorkTimer PROPERTIES
            WIN32_EXECUTABLE TRUE
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
        
        # Create Windows resource file for icon
        set(ICON_RC "${CMAKE_BINARY_DIR}/icon.rc")
        file(WRITE ${ICON_RC} "IDI_ICON1 ICON \"${CMAKE_SOURCE_DIR}/resources/worktimer_icon.ico\"")
        target_sources(WorkTimer PRIVATE ${ICON_RC})
    endif()
    
    # Copy sounds folder to output
    add_custom_command(TARGET WorkTimer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/sounds
        ${CMAKE_BINARY_DIR}/bin/sounds
    )
    
    # Deploy Qt6 dependencies using windeployqt
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET WorkTimer POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --no-compiler-runtime
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                --dir "${CMAKE_BINARY_DIR}/bin"
                "$<TARGET_FILE:WorkTimer>"
            COMMENT "Deploying Qt6 dependencies with windeployqt"
        )
    else()
        message(WARNING "windeployqt not found. Qt dependencies may be missing.")
    endif()
endif()

# Install
install(TARGETS WorkTimer
    RUNTIME DESTINATION bin
)

install(DIRECTORY sounds/
    DESTINATION bin/sounds
    FILES_MATCHING PATTERN "*.mp3"
) 