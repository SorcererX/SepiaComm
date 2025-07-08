if(BUILD_COMM2)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set( FLATC_SCHEMA_ARGS --gen-mutable --gen-name-strings )
find_package(FlatBuffers REQUIRED)
FLATBUFFERS_GENERATE_C_HEADERS( TESTMSGS ${CMAKE_SOURCE_DIR}/examples/comm2tester/comm2tester_msgs.fbs )

PROTOBUF_GENERATE_CPP(PROTO_SRCS
                      PROTO_HDRS
                      ${CMAKE_SOURCE_DIR}/examples/comm2tester/comm2tester_msgs.proto)

include_directories(defs)
add_executable( comm2tester )
target_sources( ${PROJECT_NAME} PRIVATE
     commtester.cpp
     receivetester.cpp
     threadobject.cpp
)
add_dependencies( comm2tester SepiaStream )
target_link_libraries( comm2tester -pthread )
target_link_libraries( comm2tester SepiaStream )
target_link_libraries( comm2tester ${FLATBUFFERS_LIBRARIES} )
target_link_libraries( comm2tester ${PROTOBUF_LIBRARIES} )

endif(BUILD_COMM2)
