## @brief LibSecureStorage
## @author Charly Lamothe

# --- PROJECT ---

PROJECT(securestorage C)
cmake_minimum_required(VERSION 3.8)


# --- GLOBAL OPTIONS ---

set(CMAKE_VERBOSE_MAKEFILE on)

# --- PATH ---

set(${CMAKE_CURRENT_SOURCE_DIR} ..)
set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(EXAMPLES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/examples")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")
	set(EXAMPLES_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/release/examples")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")
	set(EXAMPLES_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug/examples")
endif ()


# --- COMPILE OPTIONS ---

# Compile options for Microsoft Visual Studio
if (MSVC)
	# Add O2 optimization if build type is Release
	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		add_compile_options("$<$<CONFIG:RELEASE>:/O2>")
	endif ()
	add_compile_options("/W4")
	if (MSVC_VERSION GREATER_EQUAL 1900)
		add_compile_options("/permissive-") # Add /permissive- (kind of -pedantic) available since Visual Studio 2017
	endif ()
    add_compile_options("/W4") 
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # This variable is used to initialize the property on each target as it is created
	set(CMAKE_C_FLAGS "${CMAKE_ENABLE_C99}") # Compile with C99 rules

	# Use secure functions by defaualt and suppress warnings about
    # deprecated" functions
	# TODO: try to remove this flags and fix the issue raised
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_NONSTDC_NO_WARNINGS=1 /D _CRT_SECURE_NO_WARNINGS=1")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # If compiler is Clang
	if (CMAKE_BUILD_TYPE STREQUAL "Release") # Release build mode
		# Set all this flags explicitly, even if some of them may be covered by -Wall, in order to fine-tune the warning flags we want
		# Note that -Weverything isn't available in some version of Mac and iOS
		# Source: https://github.com/macmade/SeriousCode
		add_compile_options("-Wall" "-Wbad-function-cast" "-Wcast-align" "-Wconversion" "-Wdeclaration-after-statement"
			"-Wdeprecated-implementations" "-Wextra" "-Wfloat-equal" "-Wformat=2" "-Wformat-nonliteral" "-Wfour-char-constants"
			"-Wimplicit-atomic-properties" "-Wmissing-braces" "-Wmissing-declarations" "-Wmissing-field-initializers"
			"-Wmissing-format-attribute" "-Wmissing-noreturn" "-Wmissing-prototypes" "-Wnested-externs" "-Wnewline-eof"
			"-Wold-style-definition" "-Woverlength-strings" "-Wparentheses" "-Wpointer-arith" "-Wredundant-decls"
			"-Wreturn-type" "-Wsequence-point" "-Wshadow" "-Wshorten-64-to-32" "-Wsign-compare" "-Wsign-conversion"
			"-Wstrict-prototypes" "-Wstrict-selector-match" "-Wswitch -Wswitch-default" "-Wswitch-enum" "-Wundeclared-selector"
			"-Wuninitialized" "-Wunknown-pragmas" "-Wunreachable-code" "-Wunused-function" "-Wunused-label" "-Wunused-parameter"
			"-Wunused-value" "-Wunused-variable" "-Wwrite-strings")
	else ()
		# Minimum flags for debug mode
		add_compile_options("-Wall" "-Werror" "-Wextra" "-g")
	endif ()
else ()
	# Unix plateform
	if (UNIX)
		add_compile_options("-std=gnu99" "-pthread" "-ldl")
		if (CMAKE_BUILD_TYPE STREQUAL "Release") # Release build mode
			if (CMAKE_COMPILER_IS_GNUCC) # GNU compiler
				add_compile_options("-O2") # O2 optimization
				set(CMAKE_EXE_LINKER_FLAGS "-s") # Strip binary (only in release mode as it prevent debugging)

				# Hardening options
				## -fstack-protector-strong
				### Stack smashing protector
				## -D_FORTIFY_SOURCE=2
				### Compile-time protection against static sized buffer overflows.
				### No known regressions or performance loss
				## -Wl,-z,now
				### Disable lazy binding
				## -Wl,-z,relro
				### Read-only segments after relocation
				add_compile_options("-fstack-protector-strong" "-D_FORTIFY_SOURCE=2" "-Wl,-z,now" "-Wl,-z,relro")
			endif ()
		else () # Debug mode
			add_compile_options("-g") # Add debug symbols
		endif ()
	endif ()
	# It should work on any unix platform ?
	if (CMAKE_BUILD_TYPE STREQUAL "Release") # Release mode
		add_compile_options("$<$<CONFIG:RELEASE>:-O2>" "-fno-builtin")
	endif ()
	## -Werror=format-security
	### Reject potentially unsafe format string arguments
	## -Werror=implicit-function-declaration
	### Reject missing function prototypes
    	#add_compile_options("-Wall" "-Wextra" "-Werror" "-pedantic" "-Werror=format-security" "-Werror=implicit-function-declaration")
	add_compile_options("-Wall" "-Wextra" "-pedantic" "-Werror=format-security" "-Werror=implicit-function-declaration")
endif ()

if (WIN32)
    macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})
        set(${version} "0x${ver}")
    endif()
    endmacro()
    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
endif ()

# CCache

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

# --- SEARCH DEPENDENCIES ---

# Search LibErrorInterceptor

if (WIN32)
	set(LIBERRORINTERCEPTOR_INCLUDE "C:\\LibErrorInterceptor\\$ENV{name}\\include")
	set(LIBERRORINTERCEPTOR_LIB "C:\\LibErrorInterceptor\\$ENV{name}\\lib\\ei_static.lib")
endif ()

# Search LibUnknownEchoCryptoModule

if (WIN32)
	set(LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE "C:\\LibUnknownEchoCryptoModule\\$ENV{name}\\include")
	set(LIBUNKNOWNECHOCRYPTOMODULE_LIB "C:\\LibUnknownEchoCryptoModule\\$ENV{name}\\lib\\uecm_static.lib")
endif ()

# Search LibUnknownEchoUtilsModule

if (WIN32)
	set(LIBUNKNOWNECHOUTILSMODULE_INCLUDE "C:\\LibUnknownEchoUtilsModule\\$ENV{name}\\include")
	set(LIBUNKNOWNECHOUTILSMODULE_LIB "C:\\LibUnknownEchoUtilsModule\\$ENV{name}\\lib\\ueum_static.lib")
endif ()

# Search pthread

if (UNIX)
	find_package (Threads)
endif ()


# --- BUILD UNKNOWNECHOCRYPTOMODULE ---

# Options
set(PROJECT_NAME_STATIC "${PROJECT_NAME}_static")
set(PROJECT_NAME_SHARED "${PROJECT_NAME}_shared")

# Load source files
file(GLOB_RECURSE PROGRAM_TARGET_SRC_FILES "${SRC_PATH}/*.*")

# Build shared library
add_library(${PROJECT_NAME_SHARED} SHARED ${PROGRAM_TARGET_SRC_FILES})
target_include_directories(${PROJECT_NAME_SHARED} PUBLIC "${SRC_PATH}")
set_target_properties(${PROJECT_NAME_SHARED} PROPERTIES OUTPUT_NAME sstorage)
target_link_libraries(${PROJECT_NAME_SHARED} ${CMAKE_THREAD_LIBS_INIT})
if (WIN32)
	target_include_directories(${PROJECT_NAME_SHARED} PUBLIC "${LIBERRORINTERCEPTOR_INCLUDE}")
	target_link_libraries(${PROJECT_NAME_SHARED} "${LIBERRORINTERCEPTOR_LIB}")
	target_include_directories(${PROJECT_NAME_SHARED} PUBLIC "${LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE}")
	target_link_libraries(${PROJECT_NAME_SHARED} "${LIBUNKNOWNECHOCRYPTOMODULE_LIB}")
	target_include_directories(${PROJECT_NAME_SHARED} PUBLIC "${LIBUNKNOWNECHOUTILSMODULE_INCLUDE}")
	target_link_libraries(${PROJECT_NAME_SHARED} "${LIBUNKNOWNECHOUTILSMODULE_LIB}")
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME_SHARED} "-lei -luecm -lueum")
endif ()

# Build static library
add_library(${PROJECT_NAME_STATIC} STATIC ${PROGRAM_TARGET_SRC_FILES})
target_include_directories(${PROJECT_NAME_STATIC} PUBLIC "${SRC_PATH}")
set_target_properties(${PROJECT_NAME_STATIC} PROPERTIES OUTPUT_NAME sstorage_static)
target_link_libraries(${PROJECT_NAME_STATIC} ${CMAKE_THREAD_LIBS_INIT})
if (WIN32)
	target_include_directories(${PROJECT_NAME_STATIC} PUBLIC "${LIBERRORINTERCEPTOR_INCLUDE}")
	target_link_libraries(${PROJECT_NAME_STATIC} "${LIBERRORINTERCEPTOR_LIB}")
	target_include_directories(${PROJECT_NAME_STATIC} PUBLIC "${LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE}")
	target_link_libraries(${PROJECT_NAME_STATIC} "${LIBUNKNOWNECHOCRYPTOMODULE_LIB}")
	target_include_directories(${PROJECT_NAME_STATIC} PUBLIC "${LIBUNKNOWNECHOUTILSMODULE_INCLUDE}")
	target_link_libraries(${PROJECT_NAME_STATIC} "${LIBUNKNOWNECHOUTILSMODULE_LIB}")
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME_STATIC} "-lei -luecm -lueum")
endif ()


# --- INSTALL ---

if (UNIX)
    # Copy includes
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION /usr/include FILES_MATCHING PATTERN "*.h")

    # Copy static library
    install (TARGETS ${PROJECT_NAME_STATIC} ARCHIVE DESTINATION "/usr/lib")

    # Copy shared library
    install (TARGETS ${PROJECT_NAME_SHARED} LIBRARY DESTINATION "/usr/lib")
elseif (WIN32)
	# Copy includes
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")

	# Copy static library
	install (
		TARGETS ${PROJECT_NAME_STATIC}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
		LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	)

	install (
		TARGETS ${PROJECT_NAME_SHARED}
		RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	)
endif ()

# Build examples
file(GLOB files "${EXAMPLES_PATH}/*.c")
foreach (filePath ${files} )
	get_filename_component(executableName ${filePath} NAME_WE)
	message(STATUS "Build Program : ${executableName}")
	add_executable(${executableName} ${filePath})
    target_link_libraries(${executableName} ${PROJECT_NAME_STATIC})
    if (WIN32)
		target_include_directories(${executableName} PUBLIC "${LIBERRORINTERCEPTOR_INCLUDE}")
		target_link_libraries(${executableName} "${LIBERRORINTERCEPTOR_LIB}")
		target_include_directories(${executableName} PUBLIC "${LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE}")
		target_link_libraries(${executableName} "${LIBUNKNOWNECHOCRYPTOMODULE_LIB}")
		target_include_directories(${executableName} PUBLIC "${LIBUNKNOWNECHOUTILSMODULE_INCLUDE}")
		target_link_libraries(${executableName} "${LIBUNKNOWNECHOUTILSMODULE_LIB}")
	elseif (UNIX)
		target_link_libraries(${executableName} "-lei -luecm -lueum")
	endif ()
    set_target_properties(${executableName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_PATH})
endforeach ()
