#include "headers.h"

/*
╔══════════════════════════════════════════════════════════════╗
║functions for draw differrent elements				                       ║
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ jun 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/

 
#define TPS 8


void loadMenu(menu* root,char* path){
	FILE* file=0;
	printf("Load menu....");
	if((file=fopen(path,"r"))==0){
		perror("fopen loadMenu");
		return;
	}
	char buf[100];
	menu* m;
	m=root;
	while(feof(file)==0){
		fscanf(file,"%s ",buf);
		if (strcmp(buf,"deap")==0){
			int i,d;
			fscanf(file,"%d\n",&d);
			fscanf(file,"%s ",buf);
			fscanf(file,"%s\n",buf);
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
			fscanf(file,"%d\n",&m->$objects);
			if (m->$objects>0){
				if ((m->objects=malloc(m->$objects*sizeof(object)))==0)
					perror("malloc objects loadMenu");
				memset(m->objects,0,m->$objects*sizeof(object));
				int i=0;
				while(i<m->$objects){
					fscanf(file,"%s ",buf);
					if(strncmp(buf,"//",2)==0){
						i++;
					}
					if(strcmp(buf,"xpos")==0){
						char c;
						fscanf(file,"%c %f\n",&c,&m->objects[i].position.x);
						switch (c){
							case 'c': 
								m->objects[i].position.x+=config.options.window.width/2.0;
								break;
							case 'r':
								m->objects[i].position.x=config.options.window.width-m->objects[i].position.x;
								break;
						}
					}
					if(strcmp(buf,"ypos")==0){
						char c;
						fscanf(file,"%c %f\n",&c,&m->objects[i].position.y);
						switch (c){
							case 'c': 
								m->objects[i].position.y+=config.options.window.height/2.0;
								break;
							case 'u':
								m->objects[i].position.y=config.options.window.height-m->objects[i].position.y;
								break;
						}
					}
					if(strcmp(buf,"text")==0){
						fscanf(file,"%s\n",m->objects[i].text);
					}
					if(strcmp(buf,"touch")==0){
						fscanf(file,"%d\n",&m->objects[i].touch);
					}
					if(strcmp(buf,"focus")==0){
						fscanf(file,"%hd\n",&m->objects[i].focus);
					}
					if(strcmp(buf,"single")==0){
						fscanf(file,"%d\n",&m->objects[i].single);
					}
					if(strcmp(buf,"arg")==0){
						fscanf(file,"%d %d %d %d\n",&m->objects[i].arg[0],
										&m->objects[i].arg[1],
										&m->objects[i].arg[2],
										&m->objects[i].arg[3]);
						continue;
					}
					if(strcmp(buf,"action")==0){
						fscanf(file,"%s\n",buf); 
						if(strcmp(buf,"showwalk")==0)
							m->objects[i].action=actionShowWalkMap;
						if(strcmp(buf,"movemap")==0)
							m->objects[i].action=actionMoveMap;
						if(strcmp(buf,"zoommap")==0)
							m->objects[i].action=actionZoomMap;
						if(strcmp(buf,"menu")==0)
							m->objects[i].action=actionToggleMenu;
						if(strcmp(buf,"exit")==0)
							m->objects[i].action=actionExit;
						if(strcmp(buf,"exitroom")==0)
							m->objects[i].action=actionExitRoom;
						if(strcmp(buf,"test")==0)
							m->objects[i].action=actionTestMenu;
						if(strcmp(buf,"maptest")==0)
							m->objects[i].action=actionMapTest;
						if(strcmp(buf,"auth")==0)
							m->objects[i].action=actionAuth;
						if(strcmp(buf,"texttest")==0)
							m->objects[i].action=actionTextTest;
						if(strcmp(buf,"textlogin")==0)
							m->objects[i].action=actionTextLogin;
						if(strcmp(buf,"textpass")==0)
							m->objects[i].action=actionTextPass;
						if(strcmp(buf,"key")==0)
							m->objects[i].action=actionKeyboard;
						if(strcmp(buf,"towerbrush")==0){
							m->objects[i].action=actionTowerSpawnBrush;
							config.map.tower_objects[m->objects[i].arg[0]]=&m->objects[i];
						}
						if(strcmp(buf,"spawnnpc")==0){
							m->objects[i].action=actionSpawnNpc;
							config.map.npc_objects[m->objects[i].arg[0]]=&m->objects[i];
						}
						if(strcmp(buf,"publicmove")==0){
							m->objects[i].action=actionPublicMove;
						}
						if(strcmp(buf,"roommgmt")==0){
							m->objects[i].action=actionRoomMgmt;
						}
						continue;
					}
					//something else
					if(strcmp(buf,"elements")==0){
						fscanf(file,"%d\n",&m->objects[i].$elements);
						if ((m->objects[i].elements=malloc(m->objects[i].$elements*sizeof(element)))==0)
							perror("malloc elements loadMenu");
						memset(m->objects[i].elements,0,m->objects[i].$elements*sizeof(element));
						int j=0;
						while(j<m->objects[i].$elements){
							fscanf(file,"%s ",buf);
							if(strncmp(buf,"//",2)==0){
								j++;
//								printf("ok\n");
								continue;
							}
							if(strcmp(buf,"color")==0){
								fscanf(file,"%f %f %f %f\n",&m->objects[i].elements[j].color.r,
												&m->objects[i].elements[j].color.g,
												&m->objects[i].elements[j].color.b,
												&m->objects[i].elements[j].color.a);
								}
							if(strcmp(buf,"fcolor")==0){
								fscanf(file,"%f %f %f %f\n",&m->objects[i].elements[j].fcolor.r,
												&m->objects[i].elements[j].fcolor.g,
												&m->objects[i].elements[j].fcolor.b,
												&m->objects[i].elements[j].fcolor.a);
								}
							if(strcmp(buf,"wirecolor")==0){
								fscanf(file,"%f %f %f %f\n",&m->objects[i].elements[j].wirecolor.r,
												&m->objects[i].elements[j].wirecolor.g,
												&m->objects[i].elements[j].wirecolor.b,
												&m->objects[i].elements[j].wirecolor.a);
								}
							if(strcmp(buf,"fwirecolor")==0){
								fscanf(file,"%f %f %f %f\n",&m->objects[i].elements[j].fwirecolor.r,
												&m->objects[i].elements[j].fwirecolor.g,
												&m->objects[i].elements[j].fwirecolor.b,
												&m->objects[i].elements[j].fwirecolor.a);
								}
							if(strcmp(buf,"xpos")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].position.x);
							}
							if(strcmp(buf,"ypos")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].position.y);
							}
							if(strcmp(buf,"fxpos")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].fposition.x);
							}
							if(strcmp(buf,"fypos")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].fposition.y);
							}
							if(strcmp(buf,"xsize")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].size.x);
								if (m->objects[i].elements[j].size.x<=0)
									m->objects[i].elements[j].size.x=config.options.window.width+m->objects[i].elements[j].size.x;
							}
							if(strcmp(buf,"ysize")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].size.y);
								if (m->objects[i].elements[j].size.y<=0)
									m->objects[i].elements[j].size.y=config.options.window.height+m->objects[i].elements[j].size.y;
							}
							if(strcmp(buf,"xtile")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].tile_size.x);
							}
							if(strcmp(buf,"ytile")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].tile_size.y);
							}
							if(strcmp(buf,"tex")==0){
								fscanf(file,"%s\n",buf);
								if (strcmp(buf,"0")==0){
									m->objects[i].elements[j].tex.frames=-1;
									continue;
								}//add tex load
								m->objects[i].elements[j].map=root->map;
								sprintf(m->objects[i].elements[j].tex_path,"%s",buf);
//								if (root->map!=0)
//									loadMTexture(&m->objects[i].elements[j].tex,buf);
//								else
//									loadTexture(&m->objects[i].elements[j].tex,buf);
							}
							if(strcmp(buf,"text")==0){
								fscanf(file,"%s\n",m->objects[i].elements[j].text);
//								printf("\nelement text %s\n",m->objects[i].elements[j].text);
							}
							if(strcmp(buf,"ctext")==0){
								fscanf(file,"%hd\n",&m->objects[i].elements[j].text_centered);
							}
							if(strcmp(buf,"textsize")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].text_size);
							}
							if(strcmp(buf,"xtext")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].text_position.x);
							}
							if(strcmp(buf,"ytext")==0){
								fscanf(file,"%f\n",&m->objects[i].elements[j].text_position.y);
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
	for(i=0;i<m->$objects;i++)
		for(j=0;j<m->objects[i].$elements;j++)
			printf("o%d e%d\n",i,j);
	
*/	
	printf("done\n");
}


void realizeMenu(menu* m){
	int i,j;
	for(i=0;i<m->submenu_size;i++)
		realizeMenu(&m->submenu[i]);
	for(j=0;j<m->$objects;j++)
		free(m->objects[j].elements);
	free(m->objects);
	free(m->submenu);
	//clean all mallocs
	memset(m,0,sizeof(menu));
}



void loadMap(char* path){
	gnode * grid;
	FILE * file;
	char fullpath[300];
	printf("load map....");
//	if (config.map.textures_size==0){
//		glDeleteTextures (config.map.textures_size,(unsigned int*)config.map.textures);
//		config.map.textures_size=0;
//	}
	sprintf(fullpath,"../maps/%s.mp",path);
	if ((file=fopen(fullpath,"rt"))==0){
		perror("fopen loadMap");
		return;
	}
	char buf[100];
	int size;
	fscanf(file,"%d\n",&size);
	if ((grid=malloc(sizeof(gnode)*(size*size+1)))==0)
		perror("malloc grid loadMap");
	memset(grid,0,sizeof(gnode)*(size*size+1));
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
		/*
		1  walk see
		0 no walk see
		-1 no see no walk
		*/
		grid[i].walkable= walk[i]=='1'?(char)1:walk[i]=='0'?(char)0:(char)-1;
		/*
		1 build
		>1 no build
		*/
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
	config.map.splash_max=config.map.bullet_max*2;
	if((config.map.splash_array=malloc(sizeof(splash)*config.map.splash_max))==0)
		perror("malloc splash loadMap");
	
	fclose(file);
	
	config.map.grid=grid;
	config.map.grid_size=size;
	
	printf("done\n");
	
	
} 

void loadMapGrafics(char* path){
//	gnode * grid;
	FILE * file;
	char fullpath[300];
	int i,j;
	char buf[150];
	printf("load map grafics....");
	if (config.map.grid_size==0 || config.map.grid==0){
		printf("error\n");
		return;
	}
		
	sprintf(fullpath,"../maps/%s.mg",path);
	if ((file=fopen(fullpath,"rt"))==0){
		perror("fopen loadMap");
		return;
	}
	
	while(1){
		int c;
		fscanf(file,"%s",buf);
		if(*buf=='-'){
			break;
		}
		sscanf(buf,"%d",&c);
		fscanf(file,"%s\n",buf);
//		loadMTexture(&config.map.tex[mapTex(c)],buf);
		sprintf(config.map.tex_path[mapTex(c)],"%s",buf);
	}
	int grid_out_size=(1+(config.map.grid_size+1)%2+config.map.grid_size)/2*(config.map.grid_size/2+config.map.grid_size%2);
	int id;
	for(i=0;i<sqr(config.map.grid_size);i++){
			fscanf(file,"%d ",&id);
			config.map.grid[i].tex=mapTex(id);
//			printf("%d\n",config.map.grid[i].tex);
		}
	for(i=0;i<4;i++){
		if((config.map.grid_out[i]=malloc(sizeof(gnode)*grid_out_size))==0)
			perror("malloc grid_out[]"); 
		memset(config.map.grid_out[i],0,sizeof(gnode)*grid_out_size);
		for(j=0;j<grid_out_size;j++){
			fscanf(file,"%d ",&id);
			config.map.grid_out[i][j].id=-2;
//			config.map.grid_out[i][j].walkable=1;
			config.map.grid_out[i][j].tex=mapTex(id);
		}
	}

	while(feof(file)==0){
		fscanf(file,"%s",buf);
		if(strcmp(buf,"walls")==0){
			fscanf(file,"%d\n",&j);
			int id;
			if((config.map.walls=malloc(sizeof(wall)*j))==0)
				perror("malloc walls loadMap");
			config.map.walls_size=j;
			for(i=0;i<j;i++){
				fscanf(file,"%d %c %d\n",
						&config.map.walls[i].position,
						&config.map.walls[i].direction,
						&id);
				config.map.walls[i].$$$=STRUCT_WALL;
				config.map.walls[i].tex=mapTex(id);
			}
		}
		if(strcmp(buf,"objects")==0){
			fscanf(file,"%d\n",&j);
			int id;
			if((config.map.map_objects=malloc(sizeof(map_object)*j))==0)
				perror("malloc objects loadMap");
			config.map.map_$objects=j;
			for(i=0;i<j;i++){
				fscanf(file,"%f %f %d\n",
						&config.map.map_objects[i].position.x,
						&config.map.map_objects[i].position.y,
						&id);
				config.map.map_objects[i].$$$=STRUCT_MAPOBJECT;
				config.map.map_objects[i].tex=mapTex(id);
			}
		}
		if(strcmp(buf,"minimap")==0){
			fscanf(file,"%hd\n",&config.map.minimap.used);
			//config.map.minimap.used=config.map.minimap.enable;
		}
	}
	
	fclose(file);
	
	
//	config.map.enable=1;
	setDefaultTransform();
	printf("done\n");
}

void realizeMap(){
	int i;
	free(config.map.grid);
	config.map.grid=0;
	free(config.map.walls);
	config.map.walls=0;
	free(config.map.map_objects);
	config.map.map_objects=0;
	free(config.map.tower_array);
	config.map.tower_array=0;
	free(config.map.npc_array);
	config.map.npc_array=0;
	free(config.map.bullet_array);
	config.map.bullet_array=0;
	free(config.map.splash_array);
	config.map.splash_array=0;
	
	for(i=0;i<4;i++)
		free(config.map.grid_out[i]);
}

int loadNpcTypes(){
	char * filepath="../data/types/npc.cfg";
	FILE * file;
	npc_type* n_n=0;
	char buf[100];
	printf("Load npc types....");
	if ((file=fopen(filepath,"rt"))==0){
		perror("fopen loadNpcTypes");
		return 1;
	}
	
	while(feof(file)==0){
		if (n_n==0){
			if((n_n=malloc(sizeof(npc_type)))==0){
				perror("malloc npc loadTypes");
				return 1;
			}
			memset(n_n,0,sizeof(npc_type));
		}
		memset(buf,0,sizeof(buf));
		fscanf(file,"%s ",buf);
//		printf("%s  ||\n",buf);
		if (strcmp(buf,"texidle")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_IDLE]);
		}
		if (strcmp(buf,"texwalkleft")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_LEFT]);
		}
		if (strcmp(buf,"texwalkleftup")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_LEFT_UP]);
		}
		if (strcmp(buf,"texwalkleftdown")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_LEFT_DOWN]);
		}
		if (strcmp(buf,"texwalkright")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_RIGHT]);
		}
		if (strcmp(buf,"texwalkrightup")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_RIGHT_UP]);
		}
		if (strcmp(buf,"texwalkrightdown")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_RIGHT_DOWN]);
		}
		if (strcmp(buf,"texwalkup")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_UP]);
		}
		if (strcmp(buf,"texwalkdown")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_WALK_DOWN]);
		}
		if (strcmp(buf,"texattackleft")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_LEFT]);
		}
		if (strcmp(buf,"texattackright")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_RIGHT]);
		}
		if (strcmp(buf,"texattackleftup")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_LEFT_UP]);
		}
		if (strcmp(buf,"texattackleftdown")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_LEFT_DOWN]);
		}
		if (strcmp(buf,"texattackrightup")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_RIGHT_UP]);
		}
		if (strcmp(buf,"texattackrightdown")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_RIGHT_DOWN]);
		}
		if (strcmp(buf,"texattackup")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_UP]);
		}
		if (strcmp(buf,"texattackdown")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ATTACK_DOWN]);
		}
		if (strcmp(buf,"texicon")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_ICON]);
		}
		if (strcmp(buf,"texdestroy")==0){
			fscanf(file,"%s\n",n_n->tex_path[TEX_DESTROY]);
		}
		if (strcmp(buf,"//-")==0){
			fscanf(file,"%s\n",buf);
			typesNpcAdd(n_n->id,n_n);
			n_n=0;
			continue;
		}
		if (strcmp(buf,"name")==0){
			fscanf(file,"%s\n",buf);
			continue;
		}
		if (strcmp(buf,"id")==0){
			fscanf(file,"%d\n",&n_n->id);
			continue;
		}
		if (strcmp(buf,"health")==0){
			fscanf(file,"%d\n",&n_n->health);
			continue;
		}
		if (strcmp(buf,"damage")==0){
			fscanf(file,"%d\n",&n_n->damage);
			continue;
		}
		if (strcmp(buf,"armor")==0){
			fscanf(file,"%f\n",&n_n->armor);
			continue;
		}		
		if (strcmp(buf,"shield")==0){
			fscanf(file,"%d\n",&n_n->shield);
			continue;
		}
		if (strcmp(buf,"energy")==0){
			fscanf(file,"%d\n",&n_n->energy);
			continue;
		}
		if (strcmp(buf,"support")==0){
			fscanf(file,"%d\n",&n_n->support);
			continue;
		}
		if (strcmp(buf,"see_distanse")==0){
			fscanf(file,"%d\n",&n_n->see_distanse);
			continue;
		}
		if (strcmp(buf,"attack_distanse")==0){
			fscanf(file,"%d\n",&n_n->attack_distanse);
			continue;
		}
		if (strcmp(buf,"attack_speed")==0){
			float tmp;
			fscanf(file,"%f\n",&tmp);
			n_n->attack_speed=TPS/tmp;
			continue;
		}
		if (strcmp(buf,"move_speed")==0){
			float tmp;
			fscanf(file,"%f\n",&tmp);
			n_n->move_speed=tmp/TPS;
			continue;
		}
		if (strcmp(buf,"cost")==0){
			fscanf(file,"%d\n",&n_n->cost);
			continue;
		}
		if (strcmp(buf,"receive")==0){
			fscanf(file,"%d\n",&n_n->receive);
			continue;
		}
		if (strcmp(buf,"bullet_type")==0){
			fscanf(file,"%d\n",&n_n->bullet_type);
			continue;
		}
		if (strcmp(buf,"type")==0){
			fscanf(file,"%d\n",&n_n->type);
			continue;
		}
		if (strcmp(buf,"tex_size")==0){
			fscanf(file,"%f\n",&n_n->t_size);
			continue;
		}
		
	}
	
	if (n_n!=0)
		free(n_n);
	
	fclose(file);
	printf("done\n");
	return 0;
}

int loadTowerTypes(){
	char * filepath="../data/types/tower.cfg";
	FILE * file;
	tower_type* t_t=0;
	char buf[100];
	printf("Load tower types....");
	if ((file=fopen(filepath,"rt"))==0){
		perror("fopen loadTowerTypes");
		return 1;
	}
	
	while(feof(file)==0){
		memset(buf,0,sizeof(buf));
		fscanf(file,"%s ",buf);
//		printf("%s  ||\n",buf);
		if (t_t==0){
			if((t_t=malloc(sizeof(tower_type)))==0){
				perror("malloc tower loadTypes");
				return 1;
			}
			memset(t_t,0,sizeof(tower_type));
		}
		if (strcmp(buf,"texidle")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_IDLE]);
		}
		if (strcmp(buf,"texicon")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ICON]);
		}
		if (strcmp(buf,"texdestroy")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_DESTROY]);
		}
		if (strcmp(buf,"texattackleft")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_LEFT]);
		}
		if (strcmp(buf,"texattackright")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_RIGHT]);
		}
		if (strcmp(buf,"texattackleftup")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_LEFT_UP]);
		}
		if (strcmp(buf,"texattackleftdown")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_LEFT_DOWN]);
		}
		if (strcmp(buf,"texattackrightup")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_RIGHT_UP]);
		}
		if (strcmp(buf,"texattackrightdown")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_RIGHT_DOWN]);
		}
		if (strcmp(buf,"texattackup")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_UP]);
		}
		if (strcmp(buf,"texattackdown")==0){
			fscanf(file,"%s\n",t_t->tex_path[TEX_ATTACK_DOWN]);
		}
		if (strcmp(buf,"//-")==0){
			fscanf(file,"%s\n",buf);
			typesTowerAdd(t_t->id,t_t);
			t_t=0;
			continue;
		}
		if (strcmp(buf,"name")==0){
			fscanf(file,"%s\n",buf);
			continue;
		}
		if (strcmp(buf,"id")==0){
			fscanf(file,"%d\n",&t_t->id);
			continue;
		}
		if (strcmp(buf,"health")==0){
			fscanf(file,"%d\n",&t_t->health);
			continue;
		}
		if (strcmp(buf,"damage")==0){
			fscanf(file,"%d\n",&t_t->damage);
			continue;
		}
		if (strcmp(buf,"energy")==0){
			fscanf(file,"%d\n",&t_t->energy);
			continue;
		}
		if (strcmp(buf,"shield")==0){
			fscanf(file,"%d\n",&t_t->shield);
			continue;
		}
		if (strcmp(buf,"attack_distanse")==0){
			fscanf(file,"%d\n",&t_t->distanse);
			continue;
		}
		if (strcmp(buf,"attack_speed")==0){
			float tmp;
			fscanf(file,"%f\n",&tmp);
			t_t->attack_speed=TPS/tmp;
			continue;
		}
		if (strcmp(buf,"cost")==0){
			fscanf(file,"%d\n",&t_t->cost);
			continue;
		}
		if (strcmp(buf,"ignor_type")==0){
			fscanf(file,"%d\n",&t_t->ignor_type);
			continue;
		}
		if (strcmp(buf,"prior_type")==0){
			fscanf(file,"%d\n",&t_t->prior_type);
			continue;
		}
		if (strcmp(buf,"bullet_type")==0){
			fscanf(file,"%d\n",&t_t->bullet_type);
			continue;
		}
		if (strcmp(buf,"tex_size")==0){
			fscanf(file,"%f\n",&t_t->t_size);
			continue;
		}
		
	}
	
	if (t_t!=0)
		free(t_t);
	
	fclose(file);
	printf("done\n");
	return 0;
}

int loadBulletTypes(){
	char * filepath="../data/types/bullet.cfg";
	FILE * file;
	bullet_type* b_b=0;
	char buf[100];
	printf("Load bullet types....");
	if ((file=fopen(filepath,"rt"))==0){
		perror("fopen loadBulletTypes");
		return 1;
	}
	
	
	while(feof(file)==0){
		memset(buf,0,sizeof(buf));
		fscanf(file,"%s ",buf);
		if (b_b==0){
			if((b_b=malloc(sizeof(bullet_type)))==0){
				perror("malloc bullet loadTypes");
				return 1;
			}
			memset(b_b,0,sizeof(bullet_type));
		}
//		printf("%s  ||\n",buf);
		if (strcmp(buf,"name")==0){
			fscanf(file,"%s\n",buf);
			continue;
		}
		if (strcmp(buf,"texidle")==0){
			fscanf(file,"%s\n",b_b->tex_path[TEX_IDLE]);
		}
		if (strcmp(buf,"//-")==0){
			fscanf(file,"%s\n",buf);
			typesBulletAdd(b_b->id,b_b);
			b_b=0;
			continue;
		}
		if (strcmp(buf,"solid")==0){
			fscanf(file,"%hd\n",&b_b->solid);
			continue;
		}
		if (strcmp(buf,"id")==0){
			fscanf(file,"%d\n",&b_b->id);
			continue;
		}
		if (strcmp(buf,"speed")==0){
			float tmp;
			fscanf(file,"%f\n",&tmp);
			b_b->speed=tmp/TPS;
			continue;
		}
		if (strcmp(buf,"attack_type")==0){
			fscanf(file,"%d\n",&b_b->attack_type);
			continue;
		}
		if (strcmp(buf,"move_type")==0){
			fscanf(file,"%d\n",&b_b->move_type);
			continue;
		}
		
	}
	
	if (b_b!=0)
		free(b_b);
	
	fclose(file);
	printf("done\n");
	return 0;
}

static int loadSplashTypes(){
	char * filepath="../data/types/splash.cfg";
	FILE * file;
	splash_type* s_s=0;
	char buf[100];
	printf("Load splash types....");
	if ((file=fopen(filepath,"rt"))==0){
		perror("fopen loadSplashTypes");
		return 1;
	}
	
	while(feof(file)==0){
		memset(buf,0,sizeof(buf));
		fscanf(file,"%s ",buf);
		if (s_s==0){
			if((s_s=malloc(sizeof(splash_type)))==0){
				perror("malloc splash loadTypes");
				return 1;
			}
			memset(s_s,0,sizeof(splash_type));
		}
//		printf("%s  ||\n",buf);
		if (strcmp(buf,"tex")==0){
			fscanf(file,"%s\n",s_s->tex_path);
		}
		if (strcmp(buf,"//-")==0){
//			fscanf(file,"%s\n",buf);
			typesSplashAdd(s_s->id,s_s);
			s_s=0;
			continue;
		}
		if (strcmp(buf,"id")==0){
			fscanf(file,"%d\n",&s_s->id);
			continue;
		}
	}
	
	if (s_s!=0)
		free(s_s);
	
	fclose(file);
	printf("done\n");
	return 0;
}



void realizeTypes(){
//	free(config.bullet_types);
//	free(config.tower_types);
//	free(config.npc_types);
//	free(config.splash_types);
	typesNpcClear();
	typesTowerClear();
	typesBulletClear();
	typesSplashClear();	
}

static inline void parseTexArg(char* str,texture * t){
	sscanf(str, "frames %d\n",&t->frames);
	sscanf(str, "loop %hd\n",&t->loop);
	sscanf(str, "lfdelay %hd\n",&t->lf_delay);
	sscanf(str, "fddelay %hd\n",&t->fd_delay);
}

int loadTex(texture * t, char * path, int(load)(FILE * f)){
	FILE* file;
	char fullpath[200];
	if (path==0)
		goto out;
	if (*path==0)
		goto out;
	//try to load tar
	sprintf(fullpath,"../textures/%s.tar",path);
//	fmemopen to open memory stream
	if ((file=fopen(fullpath,"rb"))!=0){
		int size;
		//TODO: add normal tar loader
		if (untar(file,"info.cfg",&size)>0){
			int i;
			char buf[150];
			while(size>0){
				memset(buf,0,sizeof(buf));
				fgets(buf,sizeof(buf),file);
				size-=strlen(buf);
				parseTexArg(buf,t);
			}
			//try to read tga data
			for(i=0;i<t->frames;i++){
				sprintf(buf,"%d.tga",i+1);//start on 1
//				printf("try %s\n",buf);
//				clearerr(file);
//				fseek(file,0L,SEEK_SET);	
				//TODO: change to fseek or another
				fclose (file);
				file=fopen(fullpath,"rb");
				if (untar(file, buf, &size)>0){
//					printf("find %s\n",buf);
					t->tex[i]=load(file);
				}
			}
		}
		
		fclose (file);
		return 1;
	}		
	//try to load cfg
	sprintf(fullpath,"../textures/%s.cfg",path);
	if ((file=fopen(fullpath,"r"))!=0){
		//add loader
		char buf[150];
		int i;
		while(feof(file)==0){
			fgets(buf,sizeof(buf),file);
			parseTexArg(buf,t);
			//another stuff
		}
		fclose(file);
		for(i=0;i<t->frames;i++){
			sprintf(buf,"../textures/%s%d.tga",path,i+1);
			if ((file=fopen(buf,"rb"))!=0){
				t->tex[i]=load(file);
				fclose(file);
			} //TODO: check if no frames
		}
		return 1;
	}
	//try to load tga
	sprintf(fullpath,"../textures/%s.tga",path);
	int tex;
	if ((file=fopen(fullpath,"rb"))!=0){
		if ((tex=load(file))!=0){
			t->frames=1;
			t->tex[0]=tex;
		}
		fclose(file);
		return 1;
	}
out:
	printf("error: couldn't open \"%s\"!\n", path);
	memcpy(t,&config.map.tex[ERROR_T],sizeof(texture));
	return 0;
}

int loadTexture(texture * t, char * path){
	return loadTex(t,path,loadGlobalTexture);
}


int loadMTexture(texture * t, char * path){
	return loadTex(t,path,loadMapTexture);
}


void loadFiles(){
//	loadTypes("../data/types.cfg");
	loadNpcTypes();
	loadTowerTypes();
	loadBulletTypes();
	loadSplashTypes();
	
	
	//set to config file
	
	loadMenu(&config.menu.root,"../data/menu.cfg");
	config.auth_menu.enable=1;
	loadMenu(&config.loading,"../data/loading.cfg");
	loadMenu(&config.auth_menu,"../data/authmenu.cfg");
	config.auth_menu.enable=1;
	loadMenu(&config.text.keyboard,"../data/keyboard.cfg");
//	loadMenu(&config.map.screen_menu,"../data/mapmenu.cfg");
//	loadMenu(&config.map.action_menu,"../data/actionmenu.cfg");
	
	
//	loadMap("test");
	//add load config
	//load locales
	localeLoad("ru");
}

void setTestData(){
	////test
	#define x1 3 
	#define y1 2 //9	
	#define x2 7.5
	#define y2 5.5
	
	config.map.player_id=1;
	config.map.player=&config.map.players[config.map.player_id];
	config.map.players[2].group=1;
	
	config.map.npc_array[1].$$$=STRUCT_NPC;
	config.map.npc_array[1].position.x=x1;
	config.map.npc_array[1].position.y=y1;
	config.map.npc_array[1].id=-100;
	config.map.npc_array[1].type=1;
	config.map.npc_array[1].owner=2;
	config.map.npc_array[1].health=36;
	config.map.npc_array[0].$$$=STRUCT_NPC;
	config.map.npc_array[0].position.x=x2;
	config.map.npc_array[0].position.y=y2;
	config.map.npc_array[0].id=-200;
	config.map.npc_array[0].type=1;
	config.map.npc_array[0].health=87;
	
	config.map.tower_array[0].$$$=STRUCT_TOWER;
	config.map.tower_array[0].id=235;
	config.map.tower_array[0].type=1;
	config.map.tower_array[0].owner=1;
	config.map.tower_array[0].position.x=2.5;
	config.map.tower_array[0].position.y=2.5;
	config.map.grid[to2di(2.5,2.5)].tower_id=235;
	
	config.map.bullet_array[0].$$$=STRUCT_BULLET;
	config.map.bullet_array[0].id=-1;
	config.map.bullet_array[0].type=2;
	config.map.bullet_array[0].position.x=x1;
	config.map.bullet_array[0].position.y=y1;
	config.map.bullet_array[0].source.x=x1;
	config.map.bullet_array[0].source.y=y1;
	config.map.bullet_array[0].destination.x=x2;
	config.map.bullet_array[0].destination.y=y2;
	config.map.bullet_array[0].direction.x=x2-x1;
	config.map.bullet_array[0].direction.y=y2-y1;
	
	//hard test data
	int i,j,k=2;
	int h=100;
	npc_type * type=typesNpcGet(1);
	if (type!=0)
		h=type->health;
	for (i=1;i<config.map.grid_size;i++)
		for (j=i;j<config.map.grid_size;j++){
			config.map.npc_array[k].$$$=STRUCT_NPC;
			config.map.npc_array[k].position.x=i+rand()%100/100.0;
			config.map.npc_array[k].position.y=j+rand()%100/100.0;
			config.map.npc_array[k].id=-100;
			config.map.npc_array[k].type=1;//rand()%4;
			config.map.npc_array[k].owner=rand()%3;
			config.map.npc_array[k].health=rand()%h;
			k++;
		}
	
	/////
}

void cleanMap(){
	config.map.enable=0;
	usleep(10000);
	realizeMap();
	memcpy(config.map.clean_textures,config.map.textures,config.map.textures_size*sizeof(int));
	config.map.clean_textures_size=config.map.textures_size;
	config.map.textures_size=0;
	realizeMenu(&config.map.action_menu);
	config.map.action_menu.map=1;
	realizeMenu(&config.map.screen_menu);
	config.map.screen_menu.map=1;
}

void cleanAll(){
	realizeMenu(&config.menu.root);
	realizeMenu(&config.auth_menu);
//	realizeMenu(&config.map.screen_menu);
//	realizeMenu(&config.map.action_menu);
	
	realizeMenu(&config.global.context_menu);
	
	realizeTypes();
	cleanMap();
//	cleanMap();
	
	localeTextErase();
}