#include "include/debugger.h"
#include "include/colors.hpp"

//debug main loop

void debugger::run(){
   int wait_status;
   auto options = 0;
   waitpid(m_pid , &wait_status , options);
   init_registers();

   char *line = nullptr;

   while((line = linenoise("minidbg> ")) != nullptr){
     handle_command(line);

     linenoiseHistoryAdd(line);
     linenoiseFree(line);
   }
}

bool is_prefix(const string &s , const string &of){
  if(s.size() > of.size())
      return false;
  return equal(s.begin() , s.end() , of.begin());

}


std::vector<std::string> split(const string &s , char delimiter){
  std::vector<std::string> out;
  stringstream ss{s};
  string item;

  while(getline(ss , item , delimiter))
     out.push_back(item);

  return out;
}

void debugger::handle_command(const string &line){
  auto args = split(line , ' ');
  auto command = args[0];



  if(is_prefix(command ,  "continue")){
    continue_execution();
  }else if(is_prefix(command , "break")){
      string addr(args[1] , 2);
      set_breakpoint_at_adress(stol(args[1] , 0 , 16));

  }else if(is_prefix(command , "register")){
      if(is_prefix(args[1] , "dump")){

        dump_registers(m_pid);
      }else if(is_prefix(args[1] , "read")){

        std::cout <<  args[2]  << " " << "value: " << "0x" << hex << get_register_value(m_pid , get_register_from_name(args[2]))  << endl;

      }else if(is_prefix(args[1] , "write")){
        string val(args[3] , 2);
        set_register_value(m_pid , get_register_from_name(args[2]) , stol(val , 0, 16));
     }

  }else{
    cout << "Unknown command\n";
  }

}




void debugger::continue_execution(){

  ptrace(PTRACE_CONT , m_pid , nullptr , nullptr);
  int wait_status;
  auto options = 0;
  waitpid(m_pid , &wait_status , options);

}

void debugger::set_breakpoint_at_adress(intptr_t addr){
   cout << "Set breakpoint at adress 0x" << std::hex << addr <<std::endl;
   breakpoint bp(m_pid , addr);
   bp.enable();
   m_breakpoints[addr] = bp;
}


unsigned long debugger::read_memory(unsigned long adress){
  return ptrace(PTRACE_PEEKDATA , m_pid , adress , nullptr);
}

unsigned long debugger::write_memory(unsigned long adress , unsigned long value){
  return ptrace(PTRACE_POKEDATA , m_pid , adress ,  value);
}
