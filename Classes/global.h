#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "Task.h"

extern TaskManager* task_manager;
extern int bridge_repaired;
extern int seed_number;
extern int chest_opened[2];
extern int mineral_number;

void global_init();

#endif