set(NCENG_SOURCES
)

if(NCSC_BUILD_SHARED)
    add_library(noctis_script SHARED ${NCENG_SOURCES})
else()
    add_library(noctis_script STATIC ${NCENG_SOURCES})
endif()

add_library(noctis_engine::noctis_engine ALIAS noctis_engine)

target_include_directories(noctis_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include/noctis_engine
)