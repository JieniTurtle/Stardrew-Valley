#pragma once
#include "cocos2d.h"
#include "global.h"
USING_NS_CC;

class Date : public Layer {
private:
	float seconds_a_day_ = 24.0f;
	Label* time_label_;
	LayerColor* color_layer_;
public:
	static Date* create();
	bool init() override;
	void update(float delta);
};