#include "include/registers.hpp"

std::vector<struct reg_descriptor> g_register_descriptors;
unsigned int n_registers = 27;

void  init_registers(){
  g_register_descriptors.push_back({ reg::r15, 15 , "r15" });
  g_register_descriptors.push_back({ reg::r14, 14 , "r14" });
  g_register_descriptors.push_back({ reg::r13, 13 , "r13" });
  g_register_descriptors.push_back({ reg::r12, 12 , "r12" });
  g_register_descriptors.push_back({ reg::rbp, 6  , "rbp" });
  g_register_descriptors.push_back({ reg::rbx, 3  , "rbx" });
  g_register_descriptors.push_back({ reg::r11, 11, "r11" });
  g_register_descriptors.push_back({ reg::r10, 10, "r10" });
  g_register_descriptors.push_back({ reg::r9, 9, "r9" });
  g_register_descriptors.push_back({ reg::r11, 11, "r11" });
  g_register_descriptors.push_back({ reg::r8, 8, "r8" });
  g_register_descriptors.push_back({ reg::rax, 0, "rax" });
  g_register_descriptors.push_back({ reg::rcx, 2, "rcx" });
  g_register_descriptors.push_back({ reg::rdx, 1, "rdx" });
  g_register_descriptors.push_back({ reg::rsi, 4, "rsi" });
  g_register_descriptors.push_back({ reg::rdx, 1, "rdx" });
  g_register_descriptors.push_back({ reg::orig_rax, -1, "orig_rax" });
  g_register_descriptors.push_back({ reg::rip, -1, "rip" });
  g_register_descriptors.push_back({ reg::cs, 51, "cs" });
  g_register_descriptors.push_back({ reg::rflags, 49, "eflags" });
  g_register_descriptors.push_back({ reg::rsp, 7, "rsp" });
  g_register_descriptors.push_back({ reg::ss, 52, "ss" });
  g_register_descriptors.push_back({ reg::fs_base, 58, "fs_base" });
  g_register_descriptors.push_back({ reg::gs_base, 59, "gs_base" });
  g_register_descriptors.push_back({ reg::ds, 53, "ds" });
  g_register_descriptors.push_back({ reg::es, 50, "es" });
  g_register_descriptors.push_back({ reg::fs, 54, "fs" });
  g_register_descriptors.push_back({ reg::gs, 55, "gs" });
}



unsigned int get_register_value(pid_t pid , reg r){
  user_regs_struct regs;

  ptrace(PTRACE_GETREGS , pid , nullptr , &regs);

  unsigned long *addr  = (unsigned long*)&regs;
  return *(addr + (unsigned long)r);

}

void set_register_value(pid_t pid , reg r ,  long value){
  user_regs_struct regs;

  ptrace(PTRACE_GETREGS , pid , nullptr , &regs);
  unsigned long *addr  = (unsigned long*)&regs;
  cout << hex <<   *(addr + (unsigned long)r) << endl;
  *(addr + (unsigned long)r) = value;
  ptrace(PTRACE_SETREGS, pid, nullptr, &regs);

}

unsigned long  get_register_value_from_dwarf(pid_t pid , reg  regnum){
  int f = 0;
  for(int i = 0; i < g_register_descriptors.size(); i++){
      if(regnum == g_register_descriptors[i].r){
         f=1; break;
      }
  }

  if(f){
    return get_register_value(pid , regnum);
  }else{
    cout << "Error   registers not found";
  }

  return -1;


}

reg get_register_from_name( string nome){

  for(int i = 0; i < g_register_descriptors.size(); i++){
      if(nome  == g_register_descriptors[i].name){
         return g_register_descriptors[i].r;
      }
  }

  return (reg)-1;

}

void dump_registers(pid_t m_pid){
  int  qtd_in_row = 1;  // max 4 register in a rom



  for(int i = 0; i < g_register_descriptors.size(); i++){
    if(qtd_in_row <= 4){
     cout <<  "| " << g_register_descriptors[i].name << " 0x" << hex <<  get_register_value(m_pid , g_register_descriptors[i].r) << "| " ;
     qtd_in_row++;
    }
    else{
     cout << endl;
     cout << endl;
     qtd_in_row = 0;
   }

  }

  cout << endl;

}
