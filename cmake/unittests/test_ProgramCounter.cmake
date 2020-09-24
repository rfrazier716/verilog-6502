# Create new Target
add_executable(test_PC
    "test/src/verilatorCatchMain.cpp"
    "test/src/testPC.cpp"
    ${VERILATED}
    ${VERILATED_TRACE}
)

# Link the Verilator code generation as a dependency of the target
if(${REBUILD_VERILATOR})
    add_dependencies(test_PC PC_vl)
endif()

target_link_libraries(test_PC 
    "${CMAKE_SOURCE_DIR}/verilator/obj_dir/ProgramCounter/VProgramCounter__ALL.a"
) # Link to the Verilator Generated static library

add_test(
    NAME    PCFunctional
    COMMAND test_PC
    )

