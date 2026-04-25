import os

EXEC_CMDS = {
    "DIR": "/home/dreamy/Engine/core",                     # DIRECTORY
    "CMPL": "g++ post.cpp -o post > /dev/null 2>&1",       # COMPILE
    "STR": "./post",                                       # START
}

def execute(EXEC_MSG):
    print("[.py_exec/.post/exec.py]", EXEC_MSG)
    os.chdir(EXEC_CMDS["DIR"])                             # EXECUTE DIRECTORY
    os.system(EXEC_CMDS["CMPL"])                           # COMPILE START
    os.system(EXEC_CMDS["STR"])                            # START PROGRAMM

execute("Executing..")
