SET(PROJ_NAME sbase_test)

INCLUDE_DIRECTORIES(../src)

ADD_EXECUTABLE(${PROJ_NAME} main.cc)

TARGET_LINK_LIBRARIES(${PROJ_NAME}
    sbase
    sqlite3
)