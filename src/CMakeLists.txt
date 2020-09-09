set(SOURCES
    CurrentTX.c
    EVSE.c
    LCD.c
)

add_executable(version1 ${SOURCES})
target_include_directories(version1 PRIVATE ./)

target_link_libraries(version1)
