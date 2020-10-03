# Create new Target
add_executable(test_alu
    "test/src/verilatorCatchMain.cpp"
    "test/src/testALU.cpp"
    ${VERILATED}
    ${VERILATED_TRACE}
)
# Link the Verilator code generation as a dependency of the target
if(${REBUILD_VERILATOR})
    add_dependencies(test_alu alu_vl)
endif()

target_link_libraries(test_alu 
    "${CMAKE_SOURCE_DIR}/verilator/obj_dir/alu/Valu__ALL.a"
) # Link to the Verilator Generated static library

add_test(
    NAME    alu
    COMMAND test_alu
    )