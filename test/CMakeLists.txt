INCLUDE_DIRECTORIES(../include)

set(SRC_LIST test.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -g")


get_property(HEADERS_LIST GLOBAL PROPERTY "HEADERS_LIST" ) 
ADD_EXECUTABLE(test ${SRC_LIST})
set(LIBS Util)

MESSAGE(STATUS "liblist " ${LIBS})

TARGET_LINK_LIBRARIES(test ${LIBS})
set_property(TARGET test PROPERTY CXX_STANDARD 11)
#set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

