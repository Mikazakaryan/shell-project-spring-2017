#include "shell.h"

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::stringstream;
using std::find;

struct Path {
  vector<string> paths;

  void show_path(){
    for (size_t i = 0; i < paths.size(); i++) {
      cout<<paths[i]<<endl;
    }
  }

  void add_path(string path){
    bool shouldAdd = true;
    for (size_t i = 0; i < paths.size(); i++) {
      if(paths[i] == path){
        shouldAdd = false;
      }
    }
    if(shouldAdd){
      paths.push_back(path);
    }
    else{
      cout<<"The path already exists \n";
    }
  }

  void delete_path(string path){
    for (size_t i = 0; i < paths.size(); i++) {
      if(paths[i] == path){
        paths.erase(paths.begin() + i,paths.begin() + i + 1);
        break;
      }
    }
  }

};

void start_up(Path & all_pathes){
  vector <string> default_pathes {"/usr/local/sbin", "/usr/local/bin","/usr/sbin","/usr/bin","/sbin","/bin","/usr/games","/usr/local/games"};
  for (size_t i = 0; i < default_pathes.size(); i++) {
    all_pathes.add_path(default_pathes[i]);
  }
}

vector<string> parsing (string input_line){
  for (size_t i = 0; i < input_line.size(); i++) {
    if(input_line.at(i) == '|'){
      add_spaces(input_line, i, i + 2);
    }
    if(input_line.at(i) == '<'
    && input_line.at(i + 1) != '<'){
      add_spaces(input_line, i, i + 2);
    }
    if(input_line.at(i) == '>'
    && input_line.at(i + 1) != '>'){
      add_spaces(input_line, i, i + 2);
    }
    if(input_line.at(i) == '>'
    && input_line.at(i + 1) == '>'){
      add_spaces(input_line, i, i + 3);
    }
    if(input_line.at(i) == '<'
    && input_line.at(i + 1) == '<'){
      add_spaces(input_line, i, i + 3);
    }
  }
  stringstream ss (input_line) ;
  vector<string> return_value ;
  for (string word; ss>>word;) {
    return_value.push_back(word);
  }
  return return_value;
}

void add_spaces(string input_line, size_t& pos1, size_t pos2) {
    input_line.insert(pos1, " ");
    input_line.insert(pos2, " ");
    pos1 = pos2 - pos1 - 1;
}

void fork_exec(vector<string> vector_arg){
    //execvp accepts only an array of string pointers
    const char *program =(vector_arg[0]).c_str();
    const char **arg = new const char* [vector_arg.size()+2];  //extra space for program name and sentinel
    arg [0] = program;
    for (size_t i = 0;  i < vector_arg.size()+1;  ++i)
            arg [i+1] = (vector_arg[i]).c_str();

    arg [vector_arg.size()+1] = NULL;  // end of arguments sentinel is NULL

    int pid =fork();
    if(pid == -1){
        cout << "OH NO :( I am sick, can't do that";
    }
    else if(pid == 0){
        //child
        //exec is execute, 'p': give the name of the program and the operating system will look for the program in the path
        execvp(program, (char **)arg );
    }
    else{
        //parent
          waitpid(pid, NULL, 0);
    }
}

void get_comand(void) {
  string input_line;
  static bool firs_call = true;
  static Path all_pathes;
  if(firs_call){
    start_up(all_pathes);
    firs_call = false;
  }
  vector<string> parsed_commands;
  cout << "[oh my gosh shell] $ ";
  getline (cin, input_line) ;
  parsed_commands = parsing(input_line);
  for (size_t i = 0; i < parsed_commands.size(); i++) {
    if(parsed_commands[i] == "delete_path"){
      if(parsed_commands.size() >= i + 1){
        all_pathes.delete_path(parsed_commands[i + 1]);
      }else{
        cout<<"Missing argument\n";
      }
    }
    else if(parsed_commands[i] == "add_path"){
        if(parsed_commands.size() >= i + 1){
          all_pathes.add_path(parsed_commands[i + 1]);
        }else{
          cout<<"Missing argument\n";
        }
    }
    else if(parsed_commands[i] == "show_path"){
        all_pathes.show_path();
    }
  }
  fork_exec(parsed_commands);
}
