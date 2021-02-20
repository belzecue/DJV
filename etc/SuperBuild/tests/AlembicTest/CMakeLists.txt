find_package(Alembic REQUIRED)

set(source main.cpp)

add_executable(AlembicTest ${header} ${source})
target_link_libraries(AlembicTest Alembic)

add_test(AlembicTest AlembicTest)
