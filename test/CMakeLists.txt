include_directories(${PROJECT_SOURCE_DIR})

add_executable(test_qr qr.c)
target_link_libraries(test_qr PUBLIC qr)

add_executable(test_base32 base32.c)
target_link_libraries(test_base32 PUBLIC otp)

add_executable(test_keygen keygen.c)
target_link_libraries(test_keygen PUBLIC otp)

add_executable(test_otpauth otpauth.c)
target_link_libraries(test_otpauth PUBLIC otp)

add_executable(test_otp otp.c)
target_link_libraries(test_otp PUBLIC otp)

add_test(qr test_qr)
add_test(otp/base32 test_base32)
add_test(otp/keygen test_keygen)
add_test(otp/otpauth test_otpauth)
add_test(otp test_otp)