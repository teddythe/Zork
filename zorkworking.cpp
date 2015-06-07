#include "rapidxml.hpp"
#include "zorkproject.h"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "string.h"

using namespace std;
using namespace rapidxml;
//=====================================================================================================
room_struct* createRoom(string name){
  room_struct* neww = new room_struct();
  neww->name = name;
  return neww;
}

item* createItem(string name){
  item* neww = new item();
  neww->name = name;
  return neww;
}

container* createContainer(string name){
  container* neww = new container();
  neww->name = name;
  return neww;
}

int findName(string name, room_struct* array[]){
  for(int i=0;i<10;i++){
    if(name==array[i]->name){
      return i;
    }
  }
  return 0;
}
//=====================================================================================================
int main(){
  string input;
  string room[4] = {};
  room_struct* room_ [10];
  creature* creature_;
  item* item_[10];
  container* container_[10];
  string description[4] = {};
  string room_name [10] = {"room1","room2","room3","room4","room5","room6","room7","room8","room9","room10",};

  for(int i=0;i<4;i++)
    {
      // cout<<room[i]<<"\n";
    }
  player* person = new player();
  person->room = 0;
  string inventory[10] = {};
  string take = "take ";
  int exit = 0;
  int valid = 0;
  //=====================================================================================================
  //LOAD XML
  xml_document<> doc;
  std::ifstream f("sample.txt.xml");
  std::stringstream buffer;
  buffer << f.rdbuf();
  f.close();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  xml_node<> *pRoot = doc.first_node();
  xml_node<> *pNode = pRoot->first_node();
  xml_node<> *borderNode;
  xml_attribute<> *pAttr = pNode->first_attribute();
  //=====================================================================================================
  //STORING FROM XML
  int i = 0;
  int j = 0;
  int k = 0;
  
  for(xml_node<> *pNode=pRoot->first_node();pNode;pNode=pNode->next_sibling()){
    //ROOM
    if(strcmp(pNode->name(),"room")==0){
      room_[i] =  createRoom(pNode->first_node("name")->value());
      room_[i]->container_iter=0;
      //DESCRIPTION
      room_[i]->description =  pNode->first_node("description")->value();

      //TYPE
      if(pNode->first_node("type") != NULL){
	room_[i]->type = pNode->first_node("type")->value();
      }

      //BORDER
      if(pNode->first_node("border") != NULL){
	borderNode = pNode->first_node("border");
	while(strcmp(borderNode->name(), "border") == 0){
	  room_[i]->border_d[k] = borderNode->first_node("direction")->value();
	  room_[i]->border_n[k] = borderNode->first_node("name")->value();
	  borderNode = borderNode->next_sibling();
	  if(borderNode == NULL)break;
	  k++;
	}
      }
      //TRIGGER
      if(pNode->first_node("trigger") != NULL){
	room_[i]->trigger = new trigger();
	if(pNode->first_node("trigger")->first_node("type") != NULL){
	  room_[i]->trigger->type =pNode->first_node("trigger")->first_node("type")->value();
	}
	if(pNode->first_node("trigger")->first_node("print") != NULL){
	  room_[i]->trigger->print =pNode->first_node("trigger")->first_node("print")->value();
	}
	if(pNode->first_node("trigger")->first_node("command") != NULL){
	  room_[i]->trigger->command =pNode->first_node("trigger")->first_node("command")->value();
	}
	if(pNode->first_node("trigger")->first_node("condition")->first_node("has") != NULL){
	  room_[i]->trigger->c_has =pNode->first_node("trigger")->first_node("condition")->first_node("has")->value();
	}
	if(pNode->first_node("trigger")->first_node("condition")->first_node("object") != NULL){
	  room_[i]->trigger->c_object =pNode->first_node("trigger")->first_node("condition")->first_node("object")->value();
	}
	if(pNode->first_node("trigger")->first_node("condition")->first_node("owner") != NULL){
	  room_[i]->trigger->c_owner =pNode->first_node("trigger")->first_node("condition")->first_node("owner")->value();
	}
	if(pNode->first_node("trigger")->first_node("condition")->first_node("status") != NULL){
	  room_[i]->trigger->c_status =pNode->first_node("trigger")->first_node("condition")->first_node("status")->value();
	}
      }
      //CONTAINER
      if(pNode->first_node("container") != NULL){
	//cout<<"CONTAINER"<<endl;
	//cout<<pNode->name()<<endl;
	xml_node<>* tempptrr = pNode->first_node("container");
	//cout<<"check1"<<endl;
	//cout<<tempptrr->value()<<endl;
	while(tempptrr != NULL && strcmp(tempptrr->name(),"container")==0){
	  //cout<<"check2"<<endl;
	  room_[i]->container[room_[i]->container_iter] = new container();
	  //cout<<"check3"<<endl;
	  room_[i]->container[room_[i]->container_iter]->name = tempptrr->value();
	  //cout<<"check4"<<endl;
	  room_[i]->container[room_[i]->container_iter]->trigger = new trigger();
	  //cout<<tempptrr->value()<<endl;
	  if(tempptrr->first_node("trigger") != NULL){
	    //cout<<"check5"<<endl;
	    //cout<<tempptrr->first_node("trigger")->first_node("print")->value()<<endl;
	    room_[i]->container[room_[i]->container_iter]->trigger->print = tempptrr->first_node("trigger")->first_node("print")->value();
	  }
          
	  (room_[i]->container_iter)++;
          
	  tempptrr = tempptrr->next_sibling();
	}
      } 
    }

    //ITEM
    if(strcmp(pNode->name(),"item")==0){
      item_[i] =  createItem(pNode->first_node("name")->value());
      //NAME
      if(pNode->first_node("name") != NULL){
	item_[i]->name = pNode->first_node("name")->value(); }
      //WRITING
      if(pNode->first_node("writing") != NULL)
	item_[i]->writing = pNode->first_node("writing")->value();
      if(pNode->first_node("turnon") != NULL){
	//PRINT
	if(pNode->first_node("turnon")->first_node("print")!=NULL)
	  item_[i]->print = pNode->first_node("turnon")->first_node("print")->value();
	//ACTION
	if(pNode->first_node("turnon")->first_node("action")!=NULL)
	  item_[i]->action = pNode->first_node("turnon")->first_node("action")->value();
      }
      //STATUS
      if(pNode->first_node("status") != NULL)
	item_[i]->status = pNode->first_node("status")->value();
    }

    //CONTAINER
    if(strcmp(pNode->name(),"container")==0){
      container_[i] = createContainer(pNode->first_node("name")->value());
      container_[i]->item_iter = 0;
      //NAME
      if(pNode->first_node("name") != NULL)
	container_[i]->name = pNode->first_node("name")->value();
      //STATUS
      if(pNode->first_node("status")!=NULL)
	container_[i]->status = pNode->first_node("status")->value();
      //ACCEPT
      if(pNode->first_node("accept")!=NULL)
	container_[i]->accept = pNode->first_node("accept")->value();
      //TRIGGER
      if(pNode->first_node("trigger") != NULL){
	container_[i]->trigger = new trigger();
	if(pNode->first_node("trigger")->first_node("type") != NULL){
	  container_[i]->trigger->type =pNode->first_node("trigger")->first_node("type")->value();
	}
	if(pNode->first_node("trigger")->first_node("print") != NULL){
	  container_[i]->trigger->print =pNode->first_node("trigger")->first_node("print")->value();
	}
	if(pNode->first_node("trigger")->first_node("command") != NULL){
	  container_[i]->trigger->command =pNode->first_node("trigger")->first_node("command")->value();
	}
	if(pNode->first_node("trigger")->first_node("condition")->first_node("has") != NULL){
	  container_[i]->trigger->c_has =pNode->first_node("trigger")->first_node("condition")->first_node("has")->value();
	}
	if(pNode->first_node("trigger")->first_node("condition")->first_node("object") != NULL){
	  container_[i]->trigger->c_object =pNode->first_node("trigger")->first_node("condition")->first_node("object")->value();
	}
	if(pNode->first_node("trigger")->first_node("condition")->first_node("owner") != NULL){
	  container_[i]->trigger->c_owner =pNode->first_node("trigger")->first_node("condition")->first_node("owner")->value();
	}
      }
      //ITEM
      if(pNode->first_node("item")!=NULL){
	//cout<<"check1"<<endl;
	xml_node<>* tempptr = pNode->first_node("item");
	for(int k=0;k<3;k++){
	  //cout<<"k = "<<k<<endl;
	  for(int j=0;j<10;j++){
	    //cout<<"j = "<<j<<endl;
	    //cout<<"check2"<<endl;
	    //cout<<room_[1]->container[0]->name<<endl;
	    //cout<<pNode->first_node("name")->value()<<endl;
	    if(room_[k] !=NULL &&(room_[k]->container[j]!=NULL)&&room_[k]->container[j]->name == pNode->first_node("name")->value()){
	      //cout<<"check3"<<endl;
	      room_[k]->container[j]->item[room_[k]->container[j]->item_iter] = tempptr->value();
	      //cout<<pNode->first_node("item")->value();
	      //cout<<room_[k]->container[j]->item[room_[k]->container[j]->item_iter]<<endl;
	      //cout<<room_[k]->container[j]->item_iter<<endl;
	      tempptr = tempptr->next_sibling();
	      (room_[k]->container[j]->item_iter)++;
	    }
	    //cout<<"check4"<<endl;
	  }
	}
      }

      
      /*if(pNode->first_node("item")!=NULL){
	xml_node<>* tempptr = pNode->first_node("item");
	while(tempptr != NULL){
	container_[i]->item[container_[i]->item_iter] = pNode->first_node("item")->value();
	//cout<<pNode->first_node("item")->value();
	//cout<<container_[i]->item[container_[i]->item_iter]<<endl;
	tempptr = pNode->first_node("item")->next_sibling();
	(container_[i]->item_iter)++;
	}
	}*/
    }
      

    //CREATURE
    if(strcmp(pNode->name(),"creature")==0){
      creature_ =  new creature();
      creature_->name = pNode->first_node("name")->value();
      creature_->vulnerability = pNode->first_node("vulnerability")->value();
      creature_->print = pNode->first_node("attack")->first_node("print")->value();
      creature_->trigger_print = pNode->first_node("trigger")->first_node("print")->value();
      creature_->trigger_type = pNode->first_node("trigger")->first_node("type")->value();
      creature_->trigger_object = pNode->first_node("trigger")->first_node("condition")->first_node("object")->value();
    }
    i++; 
  }
  cout << description[0] << "\n";

  //=====================================================================================================
  //CODE
  int currentRoom;
  string contains = "";
  int n=0,s=0,e=0,w=0;
  int currentBorder;
  int inv=0;
  int readdetect = 0;
  int readerror = 0;
  int takedetect = 0;
  int putdetect = 0;
  int dropdetect = 0;
  int turnondetect = 0;
  int opendetect = 0;
  int attackdetect = 0;
  int check=0;
  int single=0;
  int getkey = 0;
  int canattack = 0;
  cout<<room_[currentRoom]->description<<endl;
  while(exit != 2){
    currentRoom = person->room;
    
   
    getline(cin,input);
    //border
    for(int iter=0;iter<=4;iter++){
      if(room_[currentRoom]->border_d[iter] == "north")
	n = 1;
      if(room_[currentRoom]->border_d[iter] == "south")
	s = 1;
      if(room_[currentRoom]->border_d[iter] == "east")
	e = 1;
      if(room_[currentRoom]->border_d[iter] == "west")
	w = 1;
    }

    //TAKE
    int takes = -1;
    int adds = -1;
    int deletes = -1;
    int turnons = -1;
    int putss = -1;
    int drops = -1;
    int attacks = -1;
    int reads = -1;
    int opens = -1;
    int item_exist=0;
    int cantake = 1;

   
    takes = input.find("take ");
    adds = input.find("add ");
    deletes = input.find("delete ");
    turnons = input.find("turn on ");
    putss = input.find("put ");
    drops = input.find("drop ");
    attacks = input.find("attack ");
    reads = input.find("read ");
    opens = input.find("open ");
    //TAKE
    //TAKE
    if(takes == 0){
      takedetect=0;
      cantake = 1;
      string takearg;
      std::string::size_type found = input.find_last_of(" \\");
      takearg = input.substr(found+1);
      //cout<<takearg<<endl;
      for(int k=0;k<10;k++){
	if(takearg == inventory[k]){
	  cantake=0;
	  //break;
	}
      }

      if(cantake == 1){
	for(int i=0;i<10;i++){
	  if((item_[i] != NULL) && (input == "take "+item_[i]->name)){
	    if(item_[i]->name == "key"){
	      if(getkey == 1){
        
		inventory[inv] = item_[i]->name;
		inv++;
		cout<<"Item "<<item_[i]->name<<" added to the inventory.\n";
		takedetect=1;
		for(k=0;k<3;k++){
		  for(j=0;j<10;j++){
		    if(room_[person->room]->container[k]!=NULL && room_[person->room]->container[k]->item[j] == item_[i]->name){
		      room_[person->room]->container[k]->item[j] = "";
		    }
		  }
		}
	      }
	      else takedetect=0;
	    }
	    else{
	      //cout<<inv<<endl;
	      //if(inventory[inv] != item_[i]->name){
	      inventory[inv] = item_[i]->name;
	      //cout<<"!!"<<inventory[inv]<<endl;
	      inv++;
	      cout<<"Item "<<item_[i]->name<<" added to the inventory.\n";
	      takedetect=1;
	      for(k=0;k<3;k++){
		for(j=0;j<10;j++){
		  if(room_[person->room]->container[k]!=NULL && room_[person->room]->container[k]->item[j] == item_[i]->name){
		    room_[person->room]->container[k]->item[j] = "";
		  }
		}
	      }
	      //}
	    }
	  }
	}
      }
      else takedetect=0;       
      if(!takedetect) cout<<"Error"<<endl;
    }
    
    //TURN ON
    else if(turnons == 0){
      turnondetect=0;
      for(int i=0;i<10;i++){
	if((item_[i] != NULL) && (input == "turn on "+item_[i]->name)){
	  for(k=0;k<10;k++){
	    if(inventory[k] == item_[i]->name){
	      item_[i]->turnon = 1;
	      cout<<"You activate the "<<item_[i]->name<<endl;
	      turnondetect = 1;
	      cout<<item_[i]->print<<endl;
	      if((item_[i]->name == "torch") && (single==0)){
		cout<<creature_->trigger_print<<endl;
		canattack=1;
	      }
	      if(creature_->trigger_type == "single")single = 1;
	    }}
	}
      }
      if(!turnondetect) cout<<"Error"<<endl;
    }
    //ADD
    else if(adds==0){
      int x = 0;
      for(int i=0;i<10;i++){
        if(item_[i] == NULL && x == 0){
          item_[i] = new item();
          string addarg;
          std::string::size_type found = input.find_last_of(" \\");
          addarg = input.substr(found+1);
          item_[i]->name = addarg;
          x=1;
        }
      }
    }
    //DELETE
    else if(deletes==0){
      string delarg;
      std::string::size_type found = input.find_last_of(" \\");
      delarg = input.substr(found+1);
      //item_[i]->name = addarg;
      for(int i=0;i<10;i++){
        if(item_[i]!=NULL&&item_[i]->name == delarg){
          for(int j=0;j<10;j++){
            if(delarg == inventory[j]){
              inventory[j] = "";
            }
          }
          item_[i] = NULL;
        }
      }
    }
    //READ
    else if(reads == 0){
      readdetect=0;
      for(int i=0;i<10;i++){
	if((item_[i] != NULL)&&(input == "read "+item_[i]->name)){
	  cout<<item_[i]->writing<<endl;
	  readdetect = 1;
	}
      }
      if(readdetect != 1) cout << "Error" << endl;;
    }
      
    //PUT
    else if(putss == 0){
      putdetect=0;
      for(int i=0;i<10;i++){
	for(int j=0;j<10;j++){   
	  if((room_[person->room]->container[j] != NULL) && (input == "put "+inventory[i]+" in "+room_[person->room]->container[j]->name)){
	    if(room_[person->room]->container[j] != NULL &&room_[person->room]->container[j]->name != "lock"){
	      room_[person->room]->container[j]->item[room_[person->room]->container[j]->item_iter] = inventory[i];
	      putdetect = 1;
	    }
	    room_[person->room]->container[j]->item_iter++;
	    if(room_[person->room]->container[j]->name != "lock"){
	      cout<<"Item "<<inventory[i]<<" added to "<<room_[person->room]->container[j]->name<<"."<<endl;
	      inventory[i]="";}
	    for(int k=0;k<10;k++){
	      if((container_[k] !=NULL)&&(room_[person->room]->container[j]->name == container_[k]->name)){
		putdetect = 1;
		if(container_[k]->trigger!=NULL&&room_[person->room]->container[j]->name != "lock") cout<<container_[k]->trigger->print<<endl;
		if(room_[person->room]->container[j]->name == "lock" && inventory[i]=="key"){
		  cout<<container_[k]->trigger->print<<endl;
		  room_[person->room]->trigger->c_status = "unlocked";
		}
	      }
	    }
	  }
	}
      }
      if(!putdetect) cout<<"Error"<<endl;
    }
    
    //ATTACK
    else if(attacks == 0){
      attackdetect = 0;
      if((creature_ != NULL) && (input == "attack gnome with "+creature_->vulnerability) && canattack==1){
	for(int j=0;j<10;j++){
	  if(item_[j]!= NULL && item_[j]->name == creature_->vulnerability){
	    for (int k=0;k<10;k++){
	      if(item_[j]->name == inventory[k]){
		if(item_[j]->turnon == 1){
		  cout<<creature_->print<<endl;
		  getkey = 1;
		  attackdetect = 1;

		}
	      }
	    }
	  }
	}
      }
      //else{cout<<"Error"<<endl;}
      if(!attackdetect)cout<<"Error"<<endl;
    }
    
    //DROP
    else if(drops == 0){
      dropdetect = 0;
      for(int i=0;i<10;i++){
	if(input == "drop "+inventory[i]){
	  cout<<inventory[i]<<" dropped."<<endl;
	  inventory[i] = "";
	  dropdetect = 1;
	  for(int i=0;i<10;i++){
	    if(!inventory[i].empty())check=1;
	  }
	}
      }
      if(!dropdetect) cout<<"Error"<<endl;
    }
    
    //INVENTORY
    else if(input == "i"){
      check = 0;
      for(int i=0;i<10;i++){
	if(!inventory[i].empty())check=1;
      }
      if(check==1)
	{
	  for(int iter=0; iter<10; iter++)
	    {
	      if(!inventory[iter].empty())
		{
		  cout << "Inventory: " << inventory[iter] << "\n";
		}
	    }
	}
      else
	{
	  cout << "Inventory: empty \n";
	}
    }
    //==========================================================
    //else if n,s,w,e
    else if(input == "n"){
      if(n == 0){
	cout << "Can't go that way" << endl;
      }else{
        
	currentBorder = 0;
	for(int iter=0;iter<4;iter++){
	  if(room_[person->room]->border_d[iter] == "north"){
	    currentBorder = iter;
	    break;
	  }
	}

	int temp = person->room;
	int have = 0;
	for(int i=0;i<10;i++){
	  if(inventory[i] == room_[person->room]->trigger->c_object) have = 1;  
	  else if(room_[person->room]->trigger->c_status == "unlocked") have = 1;  
	}
	if(have){
	  person->room = findName(room_[person->room]->border_n[currentBorder],room_);
	  cout<<room_[person->room]->description<<endl;
	}
	else{
	  person->room = temp;
	  cout << room_[person->room]->trigger->print << endl;
	}
  
	if(room_[person->room]->type == "exit"){
      
	  exit = 1;
	  n=0;s=0;e=0;w=0;
	  //break;
	  //cout<<room_[person->room]->description<<endl;
	  //cout<<"Game Over!\n";
	}
	//}
	//else{
	//}
  
	//cout << "n" << endl;
      }
    }
    
    //else if open ...
    //OPEN
    else if(opens == 0){
      opendetect = 0;
      int emptycontainer=1;
      for(int i=0;i<10;i++){
	if((room_[person->room]->container[i] != NULL) && (input == "open "+room_[person->room]->container[i]->name ))
	  for(int k=0;k<10;k++){
	    //cout<<room_[person->room]->container[i]->item[j]<<" ";
	    if(room_[person->room]->container[i]->item[k] != ""){   
	      emptycontainer=0;
	    }
	  }
	if((room_[person->room]->container[i] != NULL) && (input == "open "+room_[person->room]->container[i]->name ) && emptycontainer==0){
	  cout<<room_[person->room]->container[i]->name<<" contains "; 
	  for(int j=0;j<10;j++){
	    //cout<<room_[person->room]->container[i]->item[j]<<" ";
	    if(room_[person->room]->container[i]->item[j] != ""){
	      cout<<room_[person->room]->container[i]->item[j]<<" ";
	    }
	  }
	  cout<<endl;
	  opendetect = 1;
	}
	if((room_[person->room]->container[i] != NULL) && (input == "open "+room_[person->room]->container[i]->name ) && emptycontainer==1){
	  cout<<room_[person->room]->container[i]->name<<" is empty. \n";
	  opendetect = 1;
	}
	if(input == "open exit"){
	  cout<<"Game Over!\n";
	  opendetect = 1;
	  exit=2;
	  break;
	}
  
      }
      
       
      if(!opendetect) cout<<"Error"<<endl;
    }

    else if(input == "s"){
      if(s == 0){
	cout << "Can't go that way" << endl;
      }else{
        
	currentBorder = 0;
	for(int iter=0;iter<4;iter++){
	  if(room_[person->room]->border_d[iter] == "south"){
	    currentBorder = iter;
	    //cout<<"Current Border: "<<currentBorder<<endl;
	    break;
	  }
	}

	int temp = person->room;
	int have = 0;
	for(int i=0;i<10;i++){
	  //cout<<"Check1\n";
	  if(room_[person->room]->trigger != NULL){
	    if(inventory[i] == room_[person->room]->trigger->c_object) have = 1;  
	    else if(room_[person->room]->trigger->c_status == "unlocked") have = 1;
	  }
	}
	int x=0;
	int y=1;
	if(have||temp >= person->room){
	  person->room = findName(room_[person->room]->border_n[currentBorder],room_);
	  if(person->room == 0) s=0;
	  cout<<room_[person->room]->description<<endl;
	  //cout<<"person room = "<<person->room<<endl;
	  //cout<<"ONE"<<endl;
	  y=0;
	}
	else{
	  person->room = temp;
	  x = 1;
	}
	/*
	  if(temp >= person->room){
	  person->room = findName(room_[person->room]->border_n[currentBorder],room_);
	  if(person->room == 0) s=0;
	  cout<<room_[person->room]->description<<endl;
	  cout<<"person room = "<<person->room<<endl;
	  cout<<"TWO"<<endl;
	  y=0;
	  }*/
	//cout<<"x = "<<x<<" y = "<<y<<endl;
	if(x==1&&y==1){
	  if(room_[person->room]->trigger!=NULL)
	    cout << room_[person->room]->trigger->print << endl;
	}
  
	if(room_[person->room]->type == "exit"){
	  exit = 1;
	  n=0;s=0;e=0;w=0;
	}
      }
    }
    
    else if(input == "e"){
      if(e == 0){
	cout << "Can't go that way" << endl;
      }else{
	currentBorder = 0;
	for(int iter=0;iter<4;iter++){
	  if(room_[person->room]->border_d[iter] == "east"){
	    currentBorder = iter;
	    break;
	  }
	}
	person->room = findName(room_[person->room]->border_n[currentBorder],room_);
	if(room_[person->room]->type == "exit"){
      
	  exit = 1;
	  cout<<room_[person->room]->description<<endl;
	  cout<<"Game Over!\n";
	}
	//cout << "e" << endl;
      }
    }
    else if(input == "w"){
      if(w == 0){
	cout << "Can't go that way" << endl;
      }else{
	currentBorder = 0;
	for(int iter=0;iter<4;iter++){
	  if(room_[person->room]->border_d[iter] == "west"){
	    currentBorder = iter;
	    break;
	  }
	}
	person->room = findName(room_[person->room]->border_n[currentBorder],room_);
	if(room_[person->room]->type == "exit"){
      
	  exit = 1;
	  cout<<room_[person->room]->description<<endl;
	  cout<<"Game Over!\n";
	}
	//cout << "w" << endl;
      }
    }
    else{
      cout << "Error" << endl;
    }
  }
}



