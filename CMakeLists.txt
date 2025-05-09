cmake_minimum_required(VERSION 3.29)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(bulk_server VERSION 1.0.1)

add_executable(bulk_server 
main.cpp
ProcessorCommands.h
ProcessorManager.h
Server.cpp Server.h
Session.cpp Session.h
)

add_library(async SHARED
async.cpp async.h
BulkProcessor.cpp BulkProcessor.h
MultiThreadOutputter.cpp MultiThreadOutputter.h
BulkCommands.h
BulkCommandFactory.h
)

set_target_properties(bulk_server async PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(async PRIVATE 
	"${CMAKE_BINARY_DIR}"
)

target_link_libraries(bulk_server PRIVATE
    async
)

if (MSVC)
    target_compile_options(bulk_server PRIVATE /W4)
	target_compile_options(async PRIVATE /W4)
else ()
    target_compile_options(bulk_server PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(async PRIVATE -Wall -Wextra -pedantic) 
endif()

install(TARGETS async
	LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION async
)

install(TARGETS bulk_server RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)