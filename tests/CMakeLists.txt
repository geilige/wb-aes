file(GLOB SRCS *.cpp)

add_executable(testwb ${SRCS})

target_link_libraries(testwb
        wb-aes
        libgtest
        libgmock
        )

install(TARGETS testfoo DESTINATION bin)
