import os

def execute(EXEC_MSG):
    print("[.py_exec/.post/exec.py]", EXEC_MSG)
    
    original_dir = os.getcwd()
    
    os.chdir("/home/dreamy/Engine/core")
    compile_result = os.system("g++ -o post post.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11")
    if compile_result != 0:
        print("[ERROR] Compilation failed!")
        os.chdir(original_dir)
        return
    
    os.chdir("/home/dreamy/Engine")
    os.system("./core/post")
    
    os.chdir(original_dir)

execute("Executing..")
