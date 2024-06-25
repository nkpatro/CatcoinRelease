cmake_minimum_required(VERSION 3.10)
project(leveldb)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check if the compiler supports the specified standard
include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(NOT COMPILER_SUPPORTS_CXX11)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set compile options
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g2 -DNDEBUG")
set(PLATFORM_CCFLAGS "-fno-builtin-memcmp -pthread -DLEVELDB_ATOMIC_PRESENT")
set(PLATFORM_CXXFLAGS "-std=c++0x -fno-builtin-memcmp -pthread -DLEVELDB_ATOMIC_PRESENT")

set(PLATFORM_SHARED_CFLAGS "-fPIC")
set(PLATFORM_SHARED_EXT "so")
set(PLATFORM_LDFLAGS "-pthread")

# Platform-specific settings parsed from build_config.mk
if(DEFINED PLATFORM_CCFLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PLATFORM_CCFLAGS}")
endif()

if(DEFINED PLATFORM_CXXFLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PLATFORM_CXXFLAGS}")
endif()

if(DEFINED PLATFORM_LDFLAGS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PLATFORM_LDFLAGS}")
endif()

# Platform-specific source files
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/port/port_posix.cc")
        set(PLATFORM_SOURCES "port/port_posix.cc")
    else()
        message(WARNING "port/port_posix.cc does not exist")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/port/port_posix.cc")
        set(PLATFORM_SOURCES "port/port_posix.cc")
    else()
        message(WARNING "port/port_posix.cc does not exist")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/port/port_win.cc")
        set(PLATFORM_SOURCES "port/port_win.cc")
    else()
        message(WARNING "port/port_win.cc does not exist")
    endif()
endif()

# Add the source files
#file(GLOB SOURCES "db/*.cc" "util/*.cc" "table/*.cc" "port/*.cc" "helpers/*.cc")
set(SOURCE_FILES
        db/builder.cc
        db/c.cc
        db/db_impl.cc
        db/db_iter.cc
        db/dbformat.cc
        db/dumpfile.cc
        db/filename.cc
        db/log_reader.cc
        db/log_writer.cc
        db/memtable.cc
        db/repair.cc
        db/table_cache.cc
        db/version_edit.cc
        db/version_set.cc
        db/write_batch.cc
        table/block.cc
        table/block_builder.cc
        table/filter_block.cc
        table/format.cc
        table/iterator.cc
        table/merger.cc
        table/table.cc
        table/table_builder.cc
        table/two_level_iterator.cc
        util/arena.cc
        util/bloom.cc
        util/cache.cc
        util/coding.cc
        util/comparator.cc
        util/crc32c.cc
        util/env.cc
        util/env_posix.cc
        util/env_win.cc
        util/filter_policy.cc
        util/hash.cc
        util/histogram.cc
        util/logging.cc
        util/options.cc
        util/status.cc
        port/port_posix.cc)

set(HEADER_FILES
        db/builder.h
        db/db_impl.h
        db/db_iter.h
        db/filename.h
        db/dbformat.h)

# Add the library target
add_library(leveldb STATIC ${SOURCE_FILES} ${PLATFORM_SOURCES} ${HEADER_FILES})

# Add platform-specific compile definitions to the leveldb target
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_compile_definitions(${PROJECT_NAME} PRIVATE LEVELDB_PLATFORM_POSIX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_compile_definitions(${PROJECT_NAME} PRIVATE LEVELDB_PLATFORM_WINDOWS)
endif()

# Link libraries and set platform-specific linker flags
target_link_libraries(${PROJECT_NAME} ${PLATFORM_LIBS})
