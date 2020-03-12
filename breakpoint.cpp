#include "include/breakpoint.hpp"

void breakpoint::enable(){

   auto data = ptrace(PTRACE_PEEKDATA , m_pid , m_addr , nullptr);
   m_saved_data = static_cast<unsigned char>(data & 0xff);
   unsigned long long int3 = 0xcc;
   unsigned long long  data_with_int3 = ((data & ~0xff) |  int3);
   ptrace(PTRACE_POKEDATA , m_pid , m_addr  , data_with_int3);
   m_enabled = true;
}

void breakpoint::disable(){
   auto data = ptrace(PTRACE_PEEKDATA , m_pid , m_addr , nullptr);
   auto restored_data = ((data & ~0xff) | m_saved_data);
   ptrace(PTRACE_POKEDATA , m_pid , m_addr , restored_data);
}
