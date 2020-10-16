include_directories(${PROJECT_SOURCE_DIR}/inc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(submission submission.c)
target_link_libraries(submission
                      PUBLIC otp
                      PUBLIC qr)