add_library(breader io.cc)

target_include_directories(breader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../extern/mio/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/breader)