include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


FetchContent_MakeAvailable(googletest)


set(TEST_SOURCES
    test.cpp
    InvertedIndexTest.cpp
    SearchServerTest.cpp
    ${MYS_INCLUDE_DIR}/InvertedIndex.cpp
    ${MYS_INCLUDE_DIR}/SearchServer.cpp
)

add_executable(runTests ${TEST_SOURCES}
)

# Подключаем GoogleTest и include из проекта
target_link_libraries(runTests 
    PRIVATE
    GTest::gtest_main   
    
    
)
target_include_directories(runTests PRIVATE ${CMAKE_SOURCE_DIR}/include)

enable_testing()
add_test(NAME runTests COMMAND runTests)
# Регистрируем тесты
include(GoogleTest)
gtest_discover_tests(runTests)