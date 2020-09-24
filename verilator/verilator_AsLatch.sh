module_name=AsynchronousLatch
obj_dir="obj_dir/$module_name"
mkdir -p $obj_dir
verilator -Wall -trace -cc "../rtl/$module_name.v" --Mdir $obj_dir
cd $obj_dir
make -f "V$module_name.mk"