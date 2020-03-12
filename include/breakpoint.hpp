
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/ptrace.h>

#include "../library/linenoise/linenoise.h"

using namespace std;

#ifndef BREAKPOINT_H
#define BREAKPOINT_H

class breakpoint{

   public:
         breakpoint() = default;
         breakpoint(pid_t pid  ,  std::intptr_t addr){
           m_pid = pid;
           m_addr = addr;
           m_enabled = true;

         }

         void enable();
         void disable();
         auto is_enabled() { return m_enabled;}
         auto get_adress() { return m_addr; }
    private:
        pid_t m_pid;
        std::intptr_t m_addr;
        bool m_enabled;
        unsigned char m_saved_data;


};
#endif
