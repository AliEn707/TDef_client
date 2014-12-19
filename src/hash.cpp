#include <iostream>
#include <map>
#include <cstdlib>

/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Yaroslav Zotov						                       ║
║ nov 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


#define SHIFT 1e-3

using namespace std;

map <float, void*> hashMap; //float x + (y - gridsize)
typedef map <float, void*>::iterator it;

extern "C" {

	int hashAdd(float key, void * value) {
		if (hashMap.find(key) != hashMap.end()) { //already exist
			float newKey = key + SHIFT;
			while (hashMap.find(newKey) != hashMap.end())
				newKey += SHIFT;
			hashMap[newKey] = value;
			return 1;
		}
		hashMap[key] = value;
		return 0;
	}
	
	void printV(void * value) {
		cout << "value is " <<  value << endl;	
	}
	
	int hashForEach(void (function)(void*v)) {
//		cout << "map size before"<<hashMap.size()<< endl;
		for (it i = hashMap.begin(); i != hashMap.end(); i++)
			function(i->second);
//		cout << "map size "<<hashMap.size()<< endl;
		hashMap.clear();
		return 0;
	}	
	
	int printAll() {
		for (it i = hashMap.begin(); i != hashMap.end(); i++)
			cout << i->first << " : " << i->second << endl;
		return 0;
	}
	
	int hashSize(){
		return hashMap.size();
	}
}
/*
int main() {
	int length = 20;
	void * array[length];
	for (int i = 0; i < length; i++) {
		array[i] = (void*)rand();
		cout << "At " << i << " value is" << array[i] << endl;
	}
	hashAdd(0.5, array[3]);
	hashAdd(0.5, array[4]);
	hashAdd(0.5, array[5]);
	hashAdd(0.5 + SHIFT, array[15]);
	printAll();	
	hashForEach(printV);
	return 0;
}
*/