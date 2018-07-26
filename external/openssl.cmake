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

if (OPENSSL_SYSTEM)
    if (WIN32)
        set(OPENSSL_INCLUDE_DIR "C:\\OpenSSL-Win64\\include")
        set(OPENSSL_LIBRARIES
            "C:\\OpenSSL-Win64\\lib\\libssl.lib"
            "C:\\OpenSSL-Win64\\lib\\libcrypto.lib"
    )
    elseif (UNIX)
        set(OPENSSL_INCLUDE_DIR "lib/openssl/include")
        set(OPENSSL_LIBRARIES
            "${CMAKE_CURRENT_SOURCE_DIR}/lib/openssl/lib/linux/libssl.a"
            "${CMAKE_CURRENT_SOURCE_DIR}/lib/openssl/lib/linux/libcrypto.a"
        )
    endif ()
else (OPENSSL_SYSTEM)
    include (ExternalProject)

    if (UNIX)
        ExternalProject_Add(openssl
        PREFIX openssl
        URL http://www.openssl.org/source/openssl-1.1.0h.tar.gz
        CONFIGURE_COMMAND ./config no-crypto-mdebug no-shared
            no-crypto-mdebug-backtrace no-unit-test no-weak-ssl-ciphers
            no-zlib no-zlib-dynamic no-idea no-mdc2 no-rc5 --prefix=${ROOT_BUILD_DIR}
        BUILD_COMMAND make depend && make
        INSTALL_COMMAND make install_sw
        BUILD_IN_SOURCE 1
        DOWNLOAD_DIR "${DOWNLOAD_LOCATION}"
    )
    else ()
        ExternalProject_Add(openssl
        PREFIX openssl
        URL http://www.openssl.org/source/openssl-1.1.0h.tar.gz
        CONFIGURE_COMMAND perl Configure VC-WIN64A no-crypto-mdebug no-shared # Fix for x86 with VC-WIN32
            no-crypto-mdebug-backtrace no-unit-test no-weak-ssl-ciphers
            no-zlib no-zlib-dynamic no-idea no-mdc2 no-rc5 "--prefix=${ROOT_BUILD_DIR}"
        #BUILD_COMMAND "ms\\do_win64a.bat"
        #COMMAND nmake -f "ms\\ntdll.mak"
        BUILD_COMMAND nmake
        INSTALL_COMMAND nmake install
        BUILD_IN_SOURCE 1
        #INSTALL_COMMAND nmake -f "ms\\ntdll.mak" install
        DOWNLOAD_DIR "${DOWNLOAD_LOCATION}"
    )
    endif ()

    set(OPENSSL_INCLUDE_DIR ${ROOT_BUILD_DIR}/openssl/src/openssl/)

    if (UNIX)
        set(OPENSSL_LIBRARIES
            ${ROOT_BUILD_DIR}/openssl/src/openssl/libssl.a
            ${ROOT_BUILD_DIR}/openssl/src/openssl/libcrypto.a
    )
    else ()
        set(OPENSSL_LIBRARIES
            ${ROOT_BUILD_DIR}/openssl/src/openssl/libssl.lib
            ${ROOT_BUILD_DIR}/openssl/src/openssl/libcrypto.lib
    )
    endif ()
endif (OPENSSL_SYSTEM)
