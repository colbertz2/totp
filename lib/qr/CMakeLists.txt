include_directories(${PROJECT_SOURCE_DIR}/inc)

add_library(qr 
            qr.c
            base32.c)
target_link_libraries(qr
                      PUBLIC ssl
                      PUBLIC crypto
                      PUBLIC gd 
                      PUBLIC qrencode)

add_executable(test_base32 test_base32.c)
target_link_libraries(test_base32 PUBLIC qr)

add_test(qr-base32-enc/dec test_base32)