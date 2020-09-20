set(TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/unittests")
message("Building UnitTests in ${TEST_DIR}")
#Individual Unittests included below
#-------------------------------------------------
include("${TEST_DIR}/test_ALU.cmake")
include("${TEST_DIR}/test_AsynchronousLatch.cmake")
include("${TEST_DIR}/test_ProgramCounter.cmake")