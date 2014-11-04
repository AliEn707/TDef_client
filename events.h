typedef
struct event{
	int id;
	int $rooms;
	struct object * o;
} event;

//c
event * eventsCreateEvent();

void eventsFillMapMenu();
//cpp
event * eventsGet(int id);

event * eventsAdd(int id);

int eventsDel(int id);

int eventsCheck();

int eventsDraw();

int eventsFreeEvent(event* e);
