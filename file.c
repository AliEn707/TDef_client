#include "main.h"
#include "file.h"
#include "menu.h"




void loadMenu(char* path){
	FILE* file=0;
	printf("load menu....");
	if((file=fopen(path,"r"))==0)
		perror("fopen loadMenu");
	char buf[100];
	menu* m;
	while(feof(file)==0){
		fscanf(file,"%s ",buf);
		if (strcmp(buf,"deap")==0){
			int i,d;
			fscanf(file,"%d\n",&d);
			fscanf(file,"%s ",buf);
			fscanf(file,"%s\n",buf);
			m=&config.menu.root;
			for(i=0;i<d;i++)
				m=&m->submenu[ctoi(buf[i])];
			continue;
		}
		if (strcmp(buf,"submenu")==0){
			fscanf(file,"%d\n",&m->submenu_size);
			if (m->submenu_size>0){
				if ((m->submenu=malloc(m->submenu_size*sizeof(menu)))==0)
					perror("malloc submenu loadMenu");
				memset(m->submenu,0,m->submenu_size*sizeof(menu));
			}
		}
		if (strcmp(buf,"objects")==0){
			fscanf(file,"%d\n",&m->objects_size);
			if (m->objects_size>0){
				if ((m->objects=malloc(m->objects_size*sizeof(object)))==0)
					perror("malloc objects loadMenu");
				memset(m->objects,0,m->objects_size*sizeof(object));
				int i=0;
				while(i<m->objects_size){
					fscanf(file,"%s ",buf);
					if(strncmp(buf,"//",2)==0){
						i++;
					}
					if(strcmp(buf,"xpos")==0){
						char c;
						fscanf(file,"%c %f\n",&c,&m->objects[i].position.x);
						switch (c){
							case 'c': 
								m->objects[i].position.x+=config.window_width/2.0;
								break;
							case 'r':
								m->objects[i].position.x=config.window_width-m->objects[i].position.x;
								break;
						}
					}
					if(strcmp(buf,"ypos")==0){
						char c;
						fscanf(file,"%c %f\n",&c,&m->objects[i].position.y);
						switch (c){
							case 'c': 
								m->objects[i].position.y+=config.window_height/2.0;
								break;
							case 'u':
								m->objects[i].position.y=config.window_height-m->objects[i].position.y;
								break;
						}
					}
					if(strcmp(buf,"xsize")==0){
						fscanf(file,"%f\n",&m->objects[i].size.x);
					}
					if(strcmp(buf,"ysize")==0){
						fscanf(file,"%f\n",&m->objects[i].size.y);
					}
					if(strcmp(buf,"touch")==0){
						fscanf(file,"%d\n",&m->objects[i].touch);
					}
					if(strcmp(buf,"focus")==0){
						fscanf(file,"%d\n",&m->objects[i].focus);
					}
					if(strcmp(buf,"arg")==0){
						fscanf(file,"%s\n",buf);
						continue;
					}
					if(strcmp(buf,"action")==0){
						fscanf(file,"%s\n",buf);
						if(strcmp(buf,"test")==0)
							m->objects[i].action=actionTestMenu;
						continue;
					}
					//something else
					if(strcmp(buf,"elements")==0){
						fscanf(file,"%d\n",&m->objects[i].elements_size);
						if ((m->objects[i].elements=malloc(m->objects[i].elements_size*sizeof(element)))==0)
							perror("malloc elements loadMenu");
						memset(m->objects[i].elements,0,m->objects[i].elements_size*sizeof(element));
						int j=0;
						while(j<m->objects[i].elements_size){
							fscanf(file,"%s ",buf);
							if(strncmp(buf,"//",2)==0){
								j++;
//								printf("ok\n");
								continue;
							}
							if(strcmp(buf,"color")==0){
								fscanf(file,"%f %f %f\n",&m->objects[i].elements[j].color.r,
												&m->objects[i].elements[j].color.g,
												&m->objects[i].elements[j].color.b);
								}
							if(strcmp(buf,"fcolor")==0){
								fscanf(file,"%f %f %f\n",&m->objects[i].elements[j].fcolor.r,
												&m->objects[i].elements[j].fcolor.g,
												&m->objects[i].elements[j].fcolor.b);
								}
							if(strcmp(buf,"xpos")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].position.x);
							}
							if(strcmp(buf,"ypos")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].position.y);
							}
							if(strcmp(buf,"xsize")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].size.x);
							}
							if(strcmp(buf,"ysize")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].size.y);
							}
							if(strcmp(buf,"tex")==0){
								fscanf(file,"%s\n",&buf);
								if (buf[0]=='0')
									continue;
								//add tex load
							}
							//somesing else
						}
					}
				}
			}
		}
	}
	
	fclose(file);
/*load test	
	printf("\n");
	int i,j;
	m=&config.menu.root;
	for(i=0;i<m->objects_size;i++)
		for(j=0;j<m->objects[i].elements_size;j++)
			printf("o%d e%d\n",i,j);
	
*/	
	printf("done\n");
}


void realizeMenu(menu* m){
	int i,j;
	for(i=0;i<m->submenu_size;i++)
		realizeMenu(&m->submenu[i]);
	for(j=0;j<m->objects_size;j++)
		free(m->objects[j].elements);
	free(m->objects);
	free(m->submenu);
	//clean all mallocs
}

void loadTowerMenu(){
	
	
}

void realizeTowerMenu(){
	
}


void loadMap(char* path){
	gnode * grid;
	FILE * file;
	printf("load map...");
	if ((file=fopen(path,"r"))==0) 
		perror("fopen loadMap");
	char buf[100];
	int size;
	fscanf(file,"%d\n",&size);
	if ((grid=malloc(sizeof(gnode)*size*size))==0)
		perror("malloc grid loadMap");
	memset(grid,0,sizeof(gnode)*size*size);
	char* walk;
	char* build;
	if((walk=malloc((size*size+1)*sizeof(char)))==0)
		perror("malloc walk loadMap");
	if((build=malloc((size*size+1)*sizeof(char)))==0)
		perror("malloc build loadMap");
	fscanf(file,"%s\n",walk);
	fscanf(file,"%s\n",build);
	int i;
	for(i=0;i<size*size;i++){
		grid[i].id=i;
		grid[i].walkable= walk[i]=='1'?(char)1:walk[i]=='0'?(char)0:(char)-1;
		grid[i].buildable= build[i]=='1'?(char)1:build[i]=='0'?(char)0:(char)-1;
	}
	free(walk);
	free(build);
	
	while(feof(file)==0){
		memset(buf,0,sizeof(buf));
		fscanf(file,"%s ",buf);
//		printf("%s\n",buf);
		if (strcmp(buf,"max_npcs")==0){
			fscanf(file,"%d\n",&config.map.npc_max);
			if ((config.map.npc_array=malloc(sizeof(npc)*config.map.npc_max))==0)
				perror("malloc NPC initArrays");
			memset(config.map.npc_array,0,sizeof(npc)*config.map.npc_max);
			continue;
		}
		if (strcmp(buf,"max_towers")==0){
			fscanf(file,"%d\n",&config.map.tower_max);
			if ((config.map.tower_array=malloc(sizeof(tower)*config.map.tower_max))==0)
				perror("malloc tower initArrays");
			memset(config.map.tower_array,0,sizeof(tower)*config.map.tower_max);
			continue;
		}
		if (strcmp(buf,"max_bullets")==0){
			fscanf(file,"%d\n",&config.map.bullet_max);
			if ((config.map.bullet_array=malloc(sizeof(bullet)*config.map.bullet_max))==0)
				perror("malloc bullet initArrays");
			memset(config.map.bullet_array,0,sizeof(bullet)*config.map.bullet_max);
			continue;
		}
/*		if (strcmp(buf,"points")==0){
			int i;
			fscanf(file,"%d\n",&config.points_size);
			if ((config..map.points=malloc(config.points_size*sizeof(point)))==0)
				perror("malloc config.points loadMap");
			for(i=0;i<config.map.points_size;i++){
				int j;
				fscanf(file,"%d ",&j);
				fscanf(file,"%d\n",&config.map.points[j].position);
				config.points[j].id=j;
			}
			continue;
		}
*/		
	}
	////test
	config.map.npc_array[1].position.x=1.5;
	config.map.npc_array[1].position.y=1.5;
	config.map.npc_array[1].id=100;
	config.map.npc_array[0].position.x=2.0;
	config.map.npc_array[0].position.y=1.5;
	config.map.npc_array[0].id=200;
	/////
	
	config.map.grid=grid;
	config.map.grid_size=size;
	config.map.enable=1;
	setDefaultTransform();
	printf("done\n");
} 

void realizeMap(){
	free(config.map.grid);
	free(config.map.tower_array);
	free(config.map.npc_array);
	free(config.map.bullet_array);
}

void loadFiles(){
	loadMenu("../data/menu.cfg");
	
	loadMap("../maps/test.mp");
}

void cleanAll(){
	realizeMenu(&config.menu.root);
	realizeMap();
}