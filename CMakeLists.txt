cmake_minimum_required(VERSION 3.0.0)
project("TOTP Demo" VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(submission submission.c)
target_link_libraries(submission
                      PUBLIC qr
                      PUBLIC otp)
add_custom_command(TARGET submission
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:submission> ${PROJECT_SOURCE_DIR})

add_library(qr qr.c)
target_link_libraries(qr
                      PUBLIC ssl
                      PUBLIC crypto
                      PUBLIC gd
                      PUBLIC qrencode)

add_library(otp otp.c otpauth.c keygen.c base32.c)
target_link_libraries(otp
                      PUBLIC ssl
                      PUBLIC crypto)

add_subdirectory(test)