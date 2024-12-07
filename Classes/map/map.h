#ifndef MAP
#define MAP

#include "cocos2d.h"

class OutdoorScene : public cocos2d::Scene
{
private:
	cocos2d::TMXTiledMap* map_;
	cocos2d::Vec2 mapOffset_;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(OutdoorScene);
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif

