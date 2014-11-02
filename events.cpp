#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

/*
╔══════════════════════════════════════════════════════════════╗
║ contaners for storing room data					                       ║
║ 										                       ║
║ created by Yaroslav Zotov						                       ║
║ edited by Dennis Yarikov						                       ║
║ nov 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/



extern "C" {
	#include "events.h"
}
typedef struct object object;

using namespace std;
map <int, event * > events;



extern "C" {
//#include "locale.h"
	
	event * eventsAdd(int id){
		if (events.find(id) == events.end()){
			event * e=eventsCreateEvent();
			if (e!=0){
				e->id=id;
				events[id]=e;
			}else
				return 0;
		}
		return events[id];
	}
	
	event * eventsGet(int id){
		if (events.find(id) != events.end())
			return events[id];
		return 0;
	}
	
	int eventsDel(int id){
		//$_$ helps us
		return 0;
	}

	struct object * checkMouseOnObject(struct object * o);// from engine.h
	
	int eventsCheck(){
		typedef map <int, event *>::iterator it_type;
		for(it_type iterator = events.begin(); iterator != events.end(); iterator++) {
//			std::cout << iterator->second << std::endl;
			checkMouseOnObject(iterator->second->o);
		}
		
		//for each event check object
		return 0;		
	}

	void drawObject(struct object * o);//from drawer.h
	
	int eventsDraw(){
		typedef map <int, event *>::iterator it_type;
		for(it_type iterator = events.begin(); iterator != events.end(); iterator++) {
//			std::cout << iterator->second << std::endl;
			drawObject(iterator->second->o);
		}
		//for each event draw object
		return 0;
	}
}
