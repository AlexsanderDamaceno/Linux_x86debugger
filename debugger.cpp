#include "include/debugger.hpp"
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

    }else if(is_prefix(command , "show")){

         if(is_prefix(args[1] , "child_pid")){

           cout << "child pid is: " << m_pid << endl;

         }else if(is_prefix(args[1] , "breakpoint")){

           show_breakpoints();

         }

    }else if(is_prefix(command , "remove")){

         if(is_prefix(args[1] , "breakpoint")){
           string val(args[2]);
           remove_breakpoint(stol(val));
         }

    }else{

       cout << "Unknown command\n";

    }

}




void debugger::continue_execution(){
 // maybe we are on a breakpoint , so verify and disable  breakpoint , execute instruction
 // of breakpoint and enable again

  step_over_breakpoint();

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

unsigned long debugger::get_pc(){
  return get_register_value(m_pid , reg::rip);
}


void debugger::set_pc(unsigned long pc){
 set_register_value(m_pid ,  get_register_from_name("rip") , pc);
}

void debugger::step_over_breakpoint(){

   auto possible_breakpoint_location = get_pc() - 1;

   if(m_breakpoints.count(possible_breakpoint_location)){
     auto &bp = m_breakpoints[possible_breakpoint_location];

     if(bp.is_enabled()){
       auto previous_instruction_address = possible_breakpoint_location;

       set_pc(previous_instruction_address);
       bp.disable();
       ptrace(PTRACE_SINGLESTEP , m_pid , nullptr , nullptr);
       wait_for_signal();
       bp.enable();
     }

   }
}


void debugger::wait_for_signal(){
  int wait_status;
  auto options = 0;
  waitpid(m_pid , &wait_status , options);
}


void debugger::show_breakpoints(){


   unordered_map<intptr_t , breakpoint>::iterator it;

   int index_breakpoint = 1;

   for(it = m_breakpoints.begin() ; it != m_breakpoints.end(); it++){
      cout << index_breakpoint << " breakpoint at adress: 0x" << hex << it->first << endl;
      index_breakpoint++;
   }

}


void debugger::remove_breakpoint(int breakpoint_number){

     int count = 0;
     intptr_t addr = 0;

     breakpoint_number--;

     bool find = false;

     unordered_map<intptr_t , breakpoint>::iterator it;
     for(it = m_breakpoints.begin() ; it != m_breakpoints.end(); it++){
        if(count == breakpoint_number){
          addr  = it->first;
          breakpoint bp = it->second;
          bp.disable();
          find = true;
          break;
        }
        count++;
     }
   if(find)
     m_breakpoints.erase(addr);
   else{
     cout << "invalid breakpoint number: " << breakpoint_number + 1 << endl;
   }

   return;

}
