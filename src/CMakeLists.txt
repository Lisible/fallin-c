set(fallinc_srcs
  main.c
  console.c
)

if (WASM)
 list(APPEND fallinc_srcs
  console_wasm.c
 )
else()
 list(APPEND fallinc_srcs
  console_term.c
 )
endif()

add_executable(fallinc
  ${fallinc_srcs}
)

set_target_properties(fallinc PROPERTIES
  C_STANDARD 99
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (WASM)
 target_compile_definitions(fallinc PRIVATE FAL_WASM)
 target_link_options(fallinc PRIVATE
  "-nostartfiles"
  "-Wl,--import-memory"
  "-Wl,--export=wasm_main"
  "-Wl,--no-entry"
  "-Wl,--allow-undefined"
 )
endif()
