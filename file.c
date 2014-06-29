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
		if (strcmp(buf,"objects")==0){
			fscanf(file,"%d\n",&m->submenu_size);
			if ((m->submenu=malloc(m->submenu_size*sizeof(menu)))==0)
				perror("malloc submenu");
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