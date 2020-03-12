#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/ptrace.h>
#include <map>

#include "../library/linenoise/linenoise.h"
#include "breakpoint.hpp"
#include "registers.hpp"

#ifndef DEBUGEE_H
#define DEBUGEE_H

using namespace std;

class debugger{

     public:
           debugger(string prog_name , pid_t pid){
               m_proc_name = prog_name;
               m_pid       = pid;
            }

            void run();
            void continue_execution();
            void handle_command(const string &line);
            void set_breakpoint_at_adress(intptr_t addr);

            unsigned long read_memory(unsigned long adress);
            unsigned long write_memory(unsigned long adress , unsigned long value);

     private:
         string m_proc_name;
         pid_t m_pid;
         map< intptr_t , breakpoint> m_breakpoints;

};

#endif
