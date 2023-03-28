cmake_minimum_required(VERSION 3.17)

project(cisab C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PACKAGE_TESTS "Build the tests" ON)

set(cisab_INSTALL_default ON)
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
   set(cisab_INSTALL_default OFF)
endif()
set(cisab_INSTALL ${cisab_INSTALL_default} CACHE BOOL "Generate installation target")
set(cisab_INSTALL_BINDIR "bin" CACHE STRING "Installation directory for executables")
set(cisab_INSTALL_LIBDIR "lib" CACHE STRING "Installation directory for libraries")
set(cisab_INSTALL_INCLUDEDIR "include" CACHE STRING "Installation directory for headers")
set(cisab_INSTALL_CMAKEDIR "lib/cmake/${PACKAGE_NAME}" CACHE STRING "Installation directory for cmake config files")

#### third_parties ####

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#### local libraries ####

include_directories(
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
    $<INSTALL_INTERFACE:.>
)

# types
add_library(types INTERFACE)

# fmts
add_library(fmts cisab/fmts/format.cc cisab/fmts/ireadable.cc)
target_link_libraries(fmts PUBLIC types)

# logs
add_library(logs cisab/logs/logger.cc)
target_link_libraries(logs PUBLIC fmts)

#### install ####
if(cisab_INSTALL)
    foreach(_HDR
        cisab/fmts/deprecated.h
        cisab/fmts/format.h
        cisab/fmts/human_readable.h
        cisab/fmts/ireadable.h
        cisab/fmts/istreamable.h
        cisab/fmts/istringable.h
        cisab/logs/ilogs.h
        cisab/logs/logger.h
        cisab/types/iterators.h
        cisab/types/math.h
        cisab/types/strs.h
    )
        get_filename_component(_PATH ${_HDR} PATH)
        install(FILES ${_HDR} DESTINATION "${cisab_INSTALL_INCLUDEDIR}/${_PATH}")
    endforeach()

    set(TargetFile ${PROJECT_NAME}Targets)
    install(TARGETS
        fmts
        logs
        types
        EXPORT ${TargetFile}
        RUNTIME DESTINATION ${cisab_INSTALL_BINDIR}
        LIBRARY DESTINATION ${cisab_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${cisab_INSTALL_LIBDIR}
    )
    install(EXPORT ${TargetFile}
        DESTINATION ${cisab_INSTALL_CMAKEDIR}
        NAMESPACE ${PROJECT_NAME}::
    )
endif()

#### tests ####

if(PACKAGE_TESTS)
    enable_testing()
endif()

# fmts
set(FMTS_TEST fmts_test)
add_executable(${FMTS_TEST} cisab/fmts/test/main.cc)
target_link_libraries(${FMTS_TEST} ${CONAN_LIBS_GTEST} fmts)
add_test(NAME ${FMTS_TEST} COMMAND ${FMTS_TEST})

# logs
set(LOGS_TEST logs_test)
add_executable(${LOGS_TEST} cisab/logs/test/main.cc)
target_link_libraries(${LOGS_TEST} ${CONAN_LIBS_GTEST} logs)
add_test(NAME ${LOGS_TEST} COMMAND ${LOGS_TEST})

# types
set(TYPES_TEST types_test)
add_executable(${TYPES_TEST} cisab/types/test/main.cc)
target_link_libraries(${TYPES_TEST} ${CONAN_LIBS_GTEST} types)
add_test(NAME ${TYPES_TEST} COMMAND ${TYPES_TEST})
