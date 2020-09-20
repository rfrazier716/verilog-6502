# Create new Target
add_executable(test_asyncLatch
    "test/src/verilatorCatchMain.cpp"
    "test/src/testAsyncLatch.cpp"
    ${VERILATED}
)

target_link_libraries(test_asyncLatch 
    "${CMAKE_SOURCE_DIR}/verilator/obj_dir/AsynchronousLatch/VAsynchronousLatch__ALL.a"
) # Link to the Verilator Generated static library

add_test(
    NAME    aSyncLatch
    COMMAND test_asyncLatch
    )