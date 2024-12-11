#ifndef __ANIMALS_H__
#define __ANIMALS_H__

#include "cocos2d.h"
USING_NS_CC;

class Animal : public Sprite {
public:
	//Animal动画 动画路径示例：Animal/filename/moveup1.png
	Animate* moveup;
	Animate* moveleft;
	Animate* moveright;
	Animate* movedown;

	//标记Animal是否被选中
	bool ifSelected;

	//路径相关
	std::vector<Vec2> Animalpath;//路径参数
	int currentPathIndex;//路径索引
	float speed;//速度

	//Anima名称
	std::string animalName;

	//更新Animal的移动和动画
	void updatemove(float dt);

	// 播放动画
	void playAnimation(const std::string& direction);

	// 设置移动路径
	void setPath(const std::vector<Vec2>& path);
};

class Cow : public Animal {
public:
	//Animal静止图片路径为 Animal/filename/static.png
	static Cow* create(const std::string& filename);

	//Animal精灵大小
	static int Animalsize_x ;
	static int Animalsize_y ;

	//Animal纹理的动画顺序
	static int Animalorder_up;
	static int Animalorder_left;
	static int Animalorder_right;
	static int Animalorder_down;

	//初始化
	bool init();
//播放动画
	static void move(Cow* cow, TMXTiledMap* map);
};

class Sheep : public Animal {
public:
	//Animal静止图片路径为 Animal/filename/static.png
	static Sheep* create(const std::string& filename);

	//Animal精灵大小
	static int Animalsize_x;
	static int Animalsize_y;

	//Animal纹理的动画顺序
	static int Animalorder_up;
	static int Animalorder_left;
	static int Animalorder_right;
	static int Animalorder_down;

	//初始化
	bool init();
	//播放动画
	static void move(Sheep* sheep, TMXTiledMap* map);
};

class Chicken : public Animal {
public:
	//Animal静止图片路径为 Animal/filename/static.png
	static Chicken* create(const std::string& filename);

	//Animal精灵大小
	static int Animalsize_x;
	static int Animalsize_y;

	//Animal纹理的动画顺序
	static int Animalorder_up;
	static int Animalorder_left;
	static int Animalorder_right;
	static int Animalorder_down;

	//初始化
	bool init();
	//播放动画
	static void move(Chicken* chicken, TMXTiledMap* map);
};
#endif 
#pragma once
