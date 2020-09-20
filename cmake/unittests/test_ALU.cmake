# Create new Target
add_executable(test_alu
    "test/src/verilatorCatchMain.cpp"
    "test/src/testALU.cpp"
    ${VERILATED}
)

target_link_libraries(test_alu 
    "${CMAKE_SOURCE_DIR}/verilator/obj_dir/alu/Valu__ALL.a"
) # Link to the Verilator Generated static library

add_test(
    NAME    alu
    COMMAND test_alu
    )