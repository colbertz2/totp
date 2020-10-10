include_directories(${PROJECT_SOURCE_DIR}/inc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(test_otp test_otp.c)
target_link_libraries(test_otp PUBLIC otp)

add_executable(test_qr test_qr.c)
target_link_libraries(test_qr PUBLIC qr)

add_executable(submission submission.c)
target_link_libraries(submission
                      PUBLIC otp
                      PUBLIC qr)