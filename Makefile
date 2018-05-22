#################################################################
# Makefile语法模板
#################################################################

# 类似函数中的定义变量
UsrApp = c_src_file_1.o c_src_file_2.o c_src_file_3.o main_func_file.o

# 希望最终生成的exe文件的名字：生成该exe所需依赖的中间文件名字
want_exe_file_name : $(UsrApp)
	gcc -o want_exe_file_name $(UsrApp)
	del $(UsrApp)
	
# 每一个.c文件都需生成一个.o中间文件，用来最终链接成可执行程序
c_src_file_1.o : c_src_file_1.c c_src_file_1.h
	gcc -c c_src_file_1.c
	
c_src_file_2.o : c_src_file_2.c c_src_file_2.h
	gcc -c c_src_file_2.c
	
c_src_file_3.o : c_src_file_3.c c_src_file_3.h
	gcc -c c_src_file_3.c
	
main_func_file.o : main_func_file.c main_func_file_include.h
	gcc -c main_func_file.c
	
	
#################################################################

clean :
	del want_exe_file_name.exe $(UsrApp)
	
#################################################################