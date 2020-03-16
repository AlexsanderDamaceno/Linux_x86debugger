#include <sys/user.h>
#include <algorithm>
#include <vector>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/ptrace.h>
#include <sys/types.h>
using namespace std;


#ifndef REGISTERS_H
#define REGISTERS_H

enum class reg{
   rax , rbx , rcx , rdx ,
   rdi , rsi , rbp , rsp ,
   r8 , r9 , r10 , r11 ,
   r12,r13 , r14 , r15,
   rip , rflags , cs ,
   orig_rax , fs_base , gs_base,
   fs , gs , ss ,ds , es
};


struct reg_descriptor{
   reg r;
   int dwarf_r;
   std::string name;
};





void  init_registers();
unsigned int get_register_value(pid_t pid , reg r);
void set_register_value(pid_t pid , reg r ,  long value);
unsigned long  get_register_value_from_dwarf(pid_t pid , reg  regnum);
reg get_register_from_name(string name);
void dump_registers(pid_t  m_pid);
unsigned long  get_register_value_from_dwarf(pid_t pid ,  string nome);

#endif
