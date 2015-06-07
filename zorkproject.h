#include <string>
#include "string.h"

using namespace std;
struct player{
  int room;
  string inventory [10];
};

struct item{
  string writing;
  int turnon;
  string action;
  string print;
  string status;
  string name;
};

struct room_struct{
  string name;
  string type;
  string description;
  string border_n [4];
  string border_d [4];
  int container_iter;
  struct trigger* trigger;
  struct container* container[10];
};

struct trigger{
  string type;
  string command;
  string c_has;
  string c_object;
  string c_owner;
  string c_status;
  string print;
};

struct container{
  string name;
  string status;
  string accept;
  //string print;
  //string action;
  int item_iter;
  string item[10];
  struct trigger* trigger;
};

struct creature{
  string name;
  string vulnerability;
  string print;
  string trigger_print;
  string trigger_type;
  string trigger_object;
};



  
