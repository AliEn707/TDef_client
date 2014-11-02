 #define PUBLIC_SERVER "localhost" //"public.tdef.tk"
 #define PUBLIC_PORT 7001
 
 
// screen frames per texture frames
#define FpF 5
//write to config
#define Df config.options.tdf//0.26f
#define TEX_FPS 13.0f
//base
#define BASE 0

#define MAX_TEXTURES 10000
//texture global
#define MAX_TEX_GLOBAL 100

//global drawing
#define SCREEN_OFFSET 20
#define MAP_OFFSET 30

//minimap
#define MINIMAP_SIZE 190
#define MINIMAP_AREA_WIDTH (MINIMAP_SIZE*1.41)
#define MINIMAP_AREA_HEIGHT (MINIMAP_AREA_WIDTH/2)
#define MINIMAP_AREA_SHIFT 12

//textures map
#define MAP_COMON_TEXTURES_MAX 500
#define ERROR_T 1
#define WALKABLE 2
#define BUILDABLE 3
#define NO_SEE 4
#define LIGHT 5
#define DARKNESS 6
#define COMON_TEXTURES_START 7

//process object
#define MOUSE 1
#define KEYBOARD 2


#define TEXTURES 20
#define TEXTURE_FRAMES 25

#define BASE_TEX_LAYERS 10

//textures npc
#define TEX_IDLE 0
#define TEX_DESTROY 1
#define TEX_WALK_LEFT 2
#define TEX_WALK_UP 3
#define TEX_WALK_DOWN 4
#define TEX_WALK_RIGHT 5
#define TEX_WALK_RIGHT_UP 6
#define TEX_WALK_RIGHT_DOWN 7
#define TEX_WALK_LEFT_UP 8
#define TEX_WALK_LEFT_DOWN 9
#define TEX_ATTACK 10
#define TEX_ATTACK_LEFT 11
#define TEX_ATTACK_RIGHT 12
#define TEX_ATTACK_UP 13
#define TEX_ATTACK_DOWN 14
#define TEX_ATTACK_RIGHT_UP 15
#define TEX_ATTACK_RIGHT_DOWN 16
#define TEX_ATTACK_LEFT_UP 17
#define TEX_ATTACK_LEFT_DOWN 18

//map
//msg to client
#define MSG_TEST 0
#define MSG_NPC 1
#define MSG_TOWER 2
#define MSG_BULLET 3
#define MSG_PLAYER 4
//msg to server
#define MSG_SPAWN_TOWER 1
#define MSG_SPAWN_NPC 2

//public
//msg to server
#define MESSAGE_ROOM_ACT 76//'L' ask for add or edit rooms
#define MESSAGE_MOVE 77//'M'  ask for change status
//move actions
#define MESSAGE_LOBBY 78 //N  move to lobby
#define MESSAGE_MAP 77 //M  move to map
//room actions
#define MESSAGE_CREATE_ROOM 99 //'c'
#define MESSAGE_FAST_ROOM 2
#define MESSAGE_FIND_ROOM 4

//msg from server
#define MESSAGE_PLAYER_CHANGE 1
#define MESSAGE_EVENT_CHANGE 2
//
#define MESSAGE_CREATED 2
#define MESSAGE_CHANGED 2
#define MESSAGE_DELETED 2

//player statuses
#define PLAYER_CONNECTED 1
#define PLAYER_NONE 0
#define PLAYER_IN_LOBBY 2
#define PLAYER_IN_GAME 3
#define PLAYER_AT_MAP 4

//player bitmasks
#define BM_PLAYER_CONNECTED 1
#define BM_PLAYER_STATUS 2
#define BM_PLAYER_TIMESTAMP 4

//bit mask
#define setMask(z,x) z->bit_mask|=x
//#define checkMask(z,x) z->bit_mask&x

#define PLAYER_HEALTH 1
#define PLAYER_CREATE 4

#define NPC_HEALTH 1
#define NPC_POSITION 2
#define NPC_CREATE 4
#define NPC_LEVEL 8

#define TOWER_HEALTH 1
#define TOWER_TARGET 2
#define TOWER_CREATE 4
#define TOWER_LEVEL 8

#define BULLET_POSITION 1
#define BULLET_DETONATE 2
#define BULLET_CREATE 4

//global controls
#define CURSOR_SPEED cursor.speed
#define CAMERA_SPEED config.options.camera.move_speed
#define CAMERA_ZOOM config.options.camera.zoom_speed

//in game
#define NPC_SET_SIZE 9
#define TOWER_SET_SIZE 9

//brush
#define BRUSH_TOWER_SPAWN 1