if(${REBUILD_VERILATOR})
    add_custom_target(ASLatch_vl ALL
        COMMAND sh verilator_AsLatch.sh
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/verilator/"
        )

    add_custom_target(PC_vl ALL
        COMMAND sh verilator_pc.sh
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/verilator/"
        )

        add_custom_target(alu_vl ALL
        COMMAND sh verilator_alu.sh
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/verilator/"
        )
ENDIF()