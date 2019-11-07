# Add as a subdirectory SimpleRenderEngineProject using add_subdirectory
IF (NOT DEFINED all_libs)
    message( FATAL_ERROR "This CMakeLists.txt must be included using the command add_subdirectory(name)" )
ENDIF (NOT DEFINED all_libs)
# Ensure SimpleRenderEngineProject is up-to-date
IF (WIN32)
    IF (NOT DEFINED DLLFileList)
        message( FATAL_ERROR "The SimpleRenderEngineProject has been updated. Clone the project (https://github.com/mortennobel/SimpleRenderEngineProject) again the project again and start over." )
    ENDIF(NOT DEFINED DLLFileList)
ENDIF(WIN32)

# Add hpp and cpp files
file(GLOB LOTM_sources
        "*.hpp"
        "*.h"
        "*.cpp"
        )
		
# Add all asset file here (png, json, etc.)
set(LOTM_assets
	# write the name of the files below, including their extension. No quotation marks ("") required. One file per line.

	)

# Visual Leak Detector
include_directories("C:/Program Files (x86)/Visual Leak Detector/include")
link_directories("C:/Program Files (x86)/Visual Leak Detector/lib/Win32")

# Compile cpp files
add_executable(LOTM ${LOTM_sources})
target_link_libraries(LOTM ${all_libs} vld)

# copy assets to dest
foreach(asset ${LOTM_assets})
	file(COPY ${asset} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	file(COPY ${asset}  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
	file(COPY ${asset}  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
    file(COPY ${DLLFileList} DESTINATION .)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(LOTM PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)
