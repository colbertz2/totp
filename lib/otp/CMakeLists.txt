add_library(otp otp.c)
target_link_libraries(otp
                      PUBLIC ssl
                      PUBLIC crypto)
target_include_directories(otp
                           PRIVATE ${PROJECT_SOURCE_DIR}/inc)