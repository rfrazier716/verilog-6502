module_name=ProgramCounter
obj_dir="obj_dir/$module_name"
mkdir -p $obj_dir
verilator -Wall -trace -cc "../rtl/$module_name.v" "../rtl/AsynchronousLatch.v" --Mdir $obj_dir
cd $obj_dir
make -f "V$module_name.mk"