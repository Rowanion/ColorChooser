cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(ColorChooser)

#######################################################################
# cmake includes
# contains convenience functions
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
include(Functions)

#######################################################################
## global configs ##

set( CMAKE_CXX_STANDARD 11 )
setupCompiler()

# change default value for install target
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH
      "Install path prefix, prepend onto install directories."
      FORCE)
endif()

#######################################################################
## Libs and project settings

#######################################################################
## 3rdParty Libs

setupQt5()

useQt5Module( Core )
useQt5Module( Gui )
useQt5Module( Widgets )

if( WIN32 )
	useQt5Module( WinExtras )
	get_target_property( QtPlatformPlugin_location Qt5::QWindowsIntegrationPlugin LOCATION )
endif()

#######################################################################
## This project's sources

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui
	${CMAKE_BINARY_DIR}
)

# add directories containing sources (.c .cpp .h .hpp) in this way 
# to create a matching folder structure within MSVC
addFilesInSubdir( src )
addFilesInSubdir( src/ui )

addUiInSubdir( src/ui )

# set(${PROJECT_NAME}_RESOURCES 
# 	resources/qt_html.qrc
# )

set(${PROJECT_NAME}_TS_FILES
)

QT4_ADD_TRANSLATION(${PROJECT_NAME}_QM_FILES ${${PROJECT_NAME}_TS_FILES})

# here you should add all libraries this project links to.
# Qt-stuff is already included!
set(${PROJECT_NAME}_LINK_LIBS
    ${QT5_LINK_LIBS}
)

IF(WIN32)
	set(${PROJECT_NAME}_WIN_RESOURCES 
	 	resources/win_icon.rc
	)
	set(BINARY_PROPERTIES WIN32)
ENDIF()

# create the mocs - output is .moc_basename.cxx instead of .moc!
# detectMocs(FILE_TO_MOC ${${PROJECT_NAME}_HEADER_FILES}) 
# qt5_wrap_cpp(${PROJECT_NAME}_MOC ${FILE_TO_MOC})

# create the UI-headers
qt5_wrap_ui(${PROJECT_NAME}_UIS_H ${${PROJECT_NAME}_UIS})

# compile the resource files

qt5_add_resources(${PROJECT_NAME}_RESOURCES_CPP ${${PROJECT_NAME}_RESOURCES})
# create groups for our generated files and source files.
setupGroupFolders()

#######################################################################
## create/setup the binary
add_executable(${PROJECT_NAME} ${BINARY_PROPERTIES}
	${${PROJECT_NAME}_SOURCE_FILES}
	${${PROJECT_NAME}_HEADER_FILES}
	${${PROJECT_NAME}_UIS_H}
    ${${PROJECT_NAME}_RESOURCES_CPP}
    ${${PROJECT_NAME}_WIN_RESOURCES}
    ${${PROJECT_NAME}_TS_FILES}
    ${${PROJECT_NAME}_QM_FILES}
)

# enable filegroups (for Visual Studio)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)

# link against these libs
target_link_libraries(${PROJECT_NAME} 
    ${${PROJECT_NAME}_LINK_LIBS}
)

###############################################################################
## install scripts
INSTALL(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)")
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/)

INSTALL(FILES ${${PROJECT_NAME}_QM_FILES} DESTINATION
	${CMAKE_INSTALL_PREFIX})

if( WIN32 )
	install( FILES
        ${QT5_LOCATIONS}

        DESTINATION ${CMAKE_INSTALL_PREFIX}

        CONFIGURATIONS RELEASE
	)
	install( FILES
		${QtPlatformPlugin_location}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/platforms
		CONFIGURATIONS RELEASE
	)

endif()
