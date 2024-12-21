#pragma once
#include <string>
#include "cocos2d.h"
#include "global.h"
USING_NS_CC;

class Interaction : public Sprite {
protected:
	std::string name_;
	Vec2 position_ = Vec2::ZERO;
	TMXTiledMap* map_;
	int* done_flag_;

	Vec2 transfer_vision_to_map(Vec2 click_position);
	virtual void on_mouse_down(Event* event) {}
public:
	static Interaction* create(TMXTiledMap* map);
	virtual bool init();
};

class Bridge : public Interaction {
private:
	void on_mouse_down(Event* event) override;
public:
	static Bridge* create(TMXTiledMap* map);
	bool init() override;
};

class Chest : public Interaction {
private:
	void on_mouse_down(Event* event) override;
	void remove(float dt) { this->removeFromParent(); }
	int ID_;
public:
	static Chest* create(TMXTiledMap* map, int ID);
	bool init() override;
};

class GobangBoard : public Interaction {
private:
	void on_mouse_down(Event* event) override;
public:
	static GobangBoard* create(TMXTiledMap* map);
	bool init() override;
};