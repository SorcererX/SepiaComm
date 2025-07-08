cmake_minimum_required( VERSION 3.16.3 )
project(SepiaComm C CXX ASM)
include( ExternalProject )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -pthread")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

set(BUILD_EXAMPLES ON CACHE BOOL "Build examples")
set(BUILD_ROUTER ON CACHE BOOL "Build router")

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(FlatBuffers REQUIRED)

find_package(cppzmq REQUIRED)
set( PUBLIC_INCLUDES
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/receiver.h
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/dispatcher.h
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/observer.h
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/observerall.h
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/observerbase.h
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/messagesender.h
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/scopedmessagesender.h
    ${CMAKE_SOURCE_DIR}/defs/sepia/comm2/global.h
)

add_library( ${PROJECT_NAME} ${SRC_LIST} ${HEADER_FILES} ${PROTO_SRCS} ${PROTO_HDRS}  )

target_sources( ${PROJECT_NAME} PRIVATE
    src/global.cpp
    src/messagesender.cpp
    src/receiver.cpp
)
include_directories(defs)
install( FILES ${PUBLIC_INCLUDES} DESTINATION include/sepia/comm2 )

target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})
target_link_libraries( ${PROJECT_NAME} ${cppzmq_LIBRARY} ${ZeroMQ_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} ${FLATBUFFERS_LIBRARIES})
target_link_libraries( ${PROJECT_NAME} -lrt )

add_subdirectory(examples)
add_subdirectory(router)

install( TARGETS ${PROJECT_NAME} DESTINATION lib )
install( FILES ${SEPIA_INCLUDES} DESTINATION include/sepia )
