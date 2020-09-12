###########################################
#
# Build Instructions for Verilog 6502 
# Verilator Synthesis
#
###########################################

# Create new Target
add_executable(blinky
    "registers/blinky/sim_blinky.cpp"
    ${VERILATED}
)

# Add a custom shell command executed before before building target
# This runs the verilator to generate as well as make the code

if(${REBUILD_VERILATOR})
    add_custom_target(blinky_vl ALL
        COMMAND sh generate_verilator_blinky.sh
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/registers/blinky"
        )
    add_dependencies(blinky blinky_vl)
ENDIF()

target_link_libraries(blinky 
    "${CMAKE_SOURCE_DIR}/registers/blinky/obj_dir/Vblinky__ALL.a"
) # Link to the Verilator Generated static library