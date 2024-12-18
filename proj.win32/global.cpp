#include "global.h"

TaskManager* task_manager = new TaskManager;
int bridge_repaired = 0;
int chest_opened[2] = { 0, 0 };
int mineral_number = 0;

int seed_number = 10;

void global_init()
{
	// task
	Task* repair_bridge = new Task;
	repair_bridge->name = "repair_bridge";
	repair_bridge->content = "Please repair the bridge in the east of mountain";
	task_manager->add_task(repair_bridge);
}