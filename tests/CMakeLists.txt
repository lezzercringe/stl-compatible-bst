add_executable(tests traversals.cc basic_procedures.cc)

target_link_libraries(
  tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
