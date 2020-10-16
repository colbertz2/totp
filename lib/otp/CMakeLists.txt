include_directories(${PROJECT_SOURCE_DIR}/inc)

add_library(otp otp.c)
target_link_libraries(otp
                      PUBLIC ssl
                      PUBLIC crypto)

add_executable(test_otp test_otp.c)
target_link_libraries(test_otp PUBLIC otp)

add_test(otp-all test_otp)