add_executable(fallinc
  main.c
  console.c
  console_term.c
)

set_target_properties(fallinc PROPERTIES
  C_STANDARD 99
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
