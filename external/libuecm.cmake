 ###############################################################################
 # Copyright (C) 2018 Charly Lamothe                                           #
 #                                                                             #
 # This file is part of LibSecureStorage.                                      #
 #                                                                             #
 #   Licensed under the Apache License, Version 2.0 (the "License");           #
 #   you may not use this file except in compliance with the License.          #
 #   You may obtain a copy of the License at                                   #
 #                                                                             #
 #   http://www.apache.org/licenses/LICENSE-2.0                                #
 #                                                                             #
 #   Unless required by applicable law or agreed to in writing, software       #
 #   distributed under the License is distributed on an "AS IS" BASIS,         #
 #   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  #
 #   See the License for the specific language governing permissions and       #
 #   limitations under the License.                                            #
 ###############################################################################

if (LIBUECM_SYSTEM)
    if (WIN32)
        set(LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE_DIR "C:\\LibUnknownEchoCryptoModule\\$ENV{name}\\include")
        set(LIBUNKNOWNECHOCRYPTOMODULE_LIBRARIES "C:\\LibUnknownEchoCryptoModule\\$ENV{name}\\lib\\uecm_static.lib")
    elseif (UNIX)
        find_library(LIBUNKNOWNECHOCRYPTOMODULE_LIBRARIES
            NAMES uecm_static libuecm_static uecm libuecm
            HINTS ${CMAKE_INSTALL_PREFIX}/lib)
        find_path(LIBUNKNOWNECHOCRYPTOMODULE_INCLUDE_DIR uecm)
    endif ()
else (LIBUECM_SYSTEM)
    include (ExternalProject)

    set(LIBUECM_URL https://github.com/swasun/LibUnknownEchoCryptoModule.git)
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
