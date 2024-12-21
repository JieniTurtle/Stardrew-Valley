#include "global.h"

TaskManager* task_manager = new TaskManager;

int bridge_repaired = 0;
int chest_opened[2] = { 0, 0 };
int is_playing_gobang = 0;

int date = 0;
int hour = 9;

int mineral_number = 0;
int seed_number = 10;
int stone_number = 0;
int seeds_number = 2;
int seedstwo_number = 2;
int wheat_number = 200;
int wood_number = 0;
int carrot_number = 0;
int milk_number = 0;
int egg_number = 0;
int fish_number = 0;
int wool_number = 0;
int fertilizer_number = 2;
int cake_number = 0;
int soup_number = 2;
int experience = 0;
int money = 10000;
bool NewFishingListeningbool = 0;//½âËøµöÓã
bool NewPickaxeListeningbool = 0;//½âËøÍÚ¿ó
bool NewCookLayerListeningbool = 0;//½âËøÅëâ¿
int weather = 0;//0Õı³££¬1±©É¹£¬2ÏÂÓê
void global_init()
{
	// task
	Task* repair_bridge = new Task;
	repair_bridge->name = "repair_bridge";
	repair_bridge->content = "Please repair the bridge in the east of mountain";
	task_manager->add_task(repair_bridge);
}