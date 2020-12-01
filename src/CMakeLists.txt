
add_library(${CMAKE_PROJECT_NAME} SHARED
vcard.cpp
json_io.cpp
text_io.cpp
xml_io.cpp
utils.cpp )

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS ${CMAKE_PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})