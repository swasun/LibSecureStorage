 ##########################################################################################
 # Copyright (C) 2018 by Charly Lamothe                                                   #
 #                                                                                        #
 # This file is part of LibUnknownEcho.                                                   #
 #                                                                                        #
 #   LibUnknownEcho is free software: you can redistribute it and/or modify               #
 #   it under the terms of the GNU General Public License as published by                 #
 #   the Free Software Foundation, either version 3 of the License, or                    #
 #   (at your option) any later version.                                                  #
 #                                                                                        #
 #   LibUnknownEcho is distributed in the hope that it will be useful,                    #
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of                       #
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                        #
 #   GNU General Public License for more details.                                         #
 #                                                                                        #
 #   You should have received a copy of the GNU General Public License                    #
 #   along with LibUnknownEcho.  If not, see <http://www.gnu.org/licenses/>.              #
 ##########################################################################################

if (LIBUECM_SYSTEM)
    if (WIN32)
        set(LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE_DIR "C:\\LibUnknownEchoUtilsModule\\$ENV{name}\\include")
        set(LIBUNKNOWNECHOCRYPTOMODULE_LIBRARIES "C:\\LibUnknownEchoUtilsModule\\$ENV{name}\\lib\\uecm_static.lib")
    elseif (UNIX)
        find_library(LIBUNKNOWNECHOCRYPTOMODULE_LIBRARIES
            NAMES uecm_static libuecm_static uecm libuecm
            HINTS ${CMAKE_INSTALL_PREFIX}/lib)
        find_path(LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE_DIR uecm)
    endif ()
else (LIBUECM_SYSTEM)
    include (ExternalProject)

    set(LIBUECM_URL https://github.com/swasun/LibUnknownEchoUtilsModule.git)
    set(LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE_DIR ${LIBUECM_INSTALL}/external/libuecm_archive)
    set(LIBUECM_BUILD ${ROOT_BUILD_DIR}/libuecm/src/libuecm)

    if (WIN32)
        set(LIBUNKNOWNECHOCRYPTOMODULE_LIBRARIES "${LIBUECM_INSTALL}\\lib\\uecm_static.lib")
    else()
        set(LIBUNKNOWNECHOCRYPTOMODULE_LIBRARIES ${LIBUECM_INSTALL}/lib/libuecm_static.a)
    endif()

    ExternalProject_Add(libuecm
        PREFIX libuecm
        GIT_REPOSITORY ${LIBUECM_URL}    
        BUILD_IN_SOURCE 1
        BUILD_BYPRODUCTS ${LIBUNKNOWNECHOCRYPTOMODULE_LIBRARIES}
        DOWNLOAD_DIR "${DOWNLOAD_LOCATION}"
        CMAKE_CACHE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:STRING=${LIBUECM_INSTALL}
            -DROOT_BUILD_DIR:STRING=${ROOT_BUILD_DIR}
            -DCMAKE_C_FLAGS:STRING=-fPIC
    )
endif (LIBUECM_SYSTEM)
