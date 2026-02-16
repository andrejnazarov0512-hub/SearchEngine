set(find_package)

add_executable(search_engine
    main.cpp 
    ConverterJSON.cpp
    InvertedIndex.cpp 
    SearchServer.cpp  
    ${MY_INCLUDE_DIR}/ConverterJSON.h
    ${MY_INCLUDE_DIR}/InvertedIndex.h
    ${MY_INCLUDE_DIR}/SearchServer.h )

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})




