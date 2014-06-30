#include "main.h"
#include "file.h"




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
			if ((m->submenu=malloc(m->submenu_size*sizeof(menu)))==0)
				perror("malloc submenu loadMenu");
		}
		if (strcmp(buf,"objects")==0){
			fscanf(file,"%d\n",&m->objects_size);
			if ((m->objects=malloc(m->objects_size*sizeof(object)))==0)
				perror("malloc objects loadMenu");
			int i=-1;
			while(i<m->object_size){
				fscanf(file,"%s ",buf);
				if(strcmp(buf,"//")==0){
					i++;
				}
				if(strcmp(buf,"xpos")==0){
					char c;
					fscanf(file,"%c %d\n",&c,&);
				}
				//something else
				if(strcmp(buf,"elements")==0){
					#define element m->objects[i].elements[j]
					fscanf(file,"%d\n",&m->objects[i].elements_size);
					if ((m->objects[i].elements=malloc(m->objects[i].elements_size*sizeof(element)))==0)
						perror("malloc elements loadMenu");
					int j=-1;
					while(j<m->objects[i].elements_size){
						fscanf(file,"%s ",buf);
						if(strcmp(buf,"//")==0){
							j++;
							continue;
						}
						if(strcmp(buf,"color")==0){
							fscanf(file,"%f %f %f\n",&element.color.r,&element.color.g,&element.color.b);
						}
						if(strcmp(buf,"xpos")==0){
							fscanf(file,"%d\n",&element.position.x);
						}
						if(strcmp(buf,"ypos")==0){
							fscanf(file,"%d\n",&element.position.y);
						}
						if(strcmp(buf,"xsize")==0){
							fscanf(file,"%d\n",&element.size.x);
						}
						if(strcmp(buf,"ysize")==0){
							fscanf(file,"%d\n",&element.size.y);
						}
						if(strcmp(buf,"tex")==0){
							fscanf(file,"%s\n",&buf);
							if (buf[0]=='0')
								continue;
							//add tex load
						}
						//somesing else
					}
					#undef element
				}
			}
		}
	}
	
	fclose(file);
	printf("done\n");
}

void realizeMenu(){
	//clean all mallocs
}

void loadFiles(){
	loadMenu("data/menu.cfg");
}