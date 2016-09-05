include_directories(${RAPTOR_SOURCE_DIR}/src)
link_directories(${RAPTOR_SOURCE_DIR}/src)

file(GLOB sources "*.cc")
file(GLOB headers "*.hh")

add_executable(raptor ${sources} ${headers})

target_link_libraries(raptor curl)
target_link_libraries(raptor archive)
