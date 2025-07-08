project( commrouter )
include_directories( "${CMAKE_SOURCE_DIR}/defs" )
include_directories( "${CMAKE_SOURCE_DIR}/daemons/CommRouter/defs" )
add_executable( ${PROJECT_NAME} )
target_sources( ${PROJECT_NAME} PRIVATE
    main.cpp
    messagelogger.cpp
    threadobject.cpp
)

add_dependencies( ${PROJECT_NAME} SepiaComm )
target_link_libraries( ${PROJECT_NAME} SepiaComm )
target_link_libraries( ${PROJECT_NAME} -lrt )
