add_library(qr 
            qr.c
            base32.c)
target_link_libraries(qr
                      PUBLIC ssl
                      PUBLIC crypto
                      PUBLIC gd 
                      PUBLIC qrencode)
target_include_directories(qr
                           PRIVATE ${PROJECT_SOURCE_DIR}/inc)