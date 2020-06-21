INCLUDE_DIRECTORIES(../include/)
INCLUDE_DIRECTORIES(../3rd/crypt/)

file(GLOB_RECURSE AES_HEADERS  *.h *.hpp)
source_group("Include" FILES ${AES_HEADERS})
file(GLOB AES_SRC_LIST ../3rd/crypt/aes.cpp ./aes/*.cpp )

#define MODULE "aes"
add_definitions(-DMODULE="aes")
add_definitions(-DAESEXPORT)

message(${AES_SRC_LIST})

add_library(AES SHARED ${AES_SRC_LIST} ${AES_HEADERS})
set(LIBS Util)
TARGET_LINK_LIBRARIES(AES ${LIBS})
set_target_properties(AES PROPERTIES FOLDER "Plugins")
