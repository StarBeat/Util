set(SRC_LIST test.cpp)

ADD_EXECUTABLE(test ${SRC_LIST})
set(LIBS Util)

MESSAGE(STATUS "liblist " ${LIBS})

TARGET_LINK_LIBRARIES(test ${LIBS})
#set_property(TARGET test PROPERTY CXX_STANDARD 14)

set_target_properties(test PROPERTIES FOLDER "test")

#set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

ADD_EXECUTABLE(rpc_client testrpc_client/main.cpp)
TARGET_LINK_LIBRARIES(rpc_client ${LIBS})

ADD_EXECUTABLE(rpc_server testrpc_server/main.cpp)
TARGET_LINK_LIBRARIES(rpc_server ${LIBS})

set_target_properties(rpc_client PROPERTIES FOLDER "test")
set_target_properties(rpc_server PROPERTIES FOLDER "test")
