#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "Task.h"

extern TaskManager* task_manager;
extern int bridge_repaired;
extern int seed_number;
extern int chest_opened[2];
extern int mineral_number;
extern int stone_number;
extern int seeds_number;
extern int seedstwo_number;
extern int wheat_number;
extern int wood_number;
extern int carrot_number;
extern int milk_number;
extern int egg_number;
extern int fish_number;
extern int wool_number;
extern int fertilizer_number;
extern int experience;
extern int money;
extern int cake_number ;
extern int soup_number ;
extern bool NewFishingListeningbool;//½âËøµöÓã
extern bool NewPickaxeListeningbool;//½âËøÍÚ¿ó
extern bool NewCookLayerListeningbool;//½âËøÅëâ¿
extern int weather;

extern int hour;
extern int date;

void global_init();

#endif