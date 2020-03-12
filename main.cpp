#include <iostream>
#include <sys/ptrace.h>
#include <unistd.h>

#include "library/linenoise/linenoise.h"
#include "include/debugger.h"
using namespace std;



int main(int argc , char * argv[]){

    if(argc < 2){
      cout << "Program name missing";
      return -1;
    }

    auto program_name = argv[1];

    int pid = fork();

    if(pid == 0){
      // allow parent trace of this process
       ptrace(PTRACE_TRACEME , 0 , nullptr , nullptr);
       if(execl(program_name , program_name , nullptr) < 0){
         cout << "Error for open the program" << endl;
         return -1;
       }
    }else if(pid >= 1){

    // call debug loop
      debugger dbg(program_name , pid);

      dbg.run();
    }

}
