# Create new Target
add_executable(test_PC
    "test/src/verilatorCatchMain.cpp"
    "test/src/testPC.cpp"
    ${VERILATED}
)

target_link_libraries(test_PC 
    "${CMAKE_SOURCE_DIR}/verilator/obj_dir/ProgramCounter/VProgramCounter__ALL.a"
) # Link to the Verilator Generated static library

add_test(
    NAME    PCFunctional
    COMMAND test_PC
    )