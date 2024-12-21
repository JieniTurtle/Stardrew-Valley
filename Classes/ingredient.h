#ifndef ingredients_h
#define ingredients_h
#include "global.h"
#include"MainCharacter.h"
#include "cocos2d.h"
USING_NS_CC;
class Iingredients {
public:
    std::string ingredientsName;
    int *quantity;
    std::string imageFile;

    Iingredients(std::string ingreName, int *itemQuantity, std::string itemImageFile)
        : ingredientsName(ingreName), quantity(itemQuantity), imageFile(itemImageFile) {}

    void addQuantity(int qty) {
        *quantity += qty;
    }

    bool consumeQuantity(int qty) {
        if (*quantity >= qty) {
            *quantity -= qty;
            return true;
        }
        return false;
    }
};

// CookLayer 类，展示物品和合成按钮
class CookLayer : public cocos2d::Layer {
public:
    CookLayer() : selectedItem(nullptr) {}

    //初始化
    bool init();

    // 创建物品
    void createItems();

    //创建菜单
    void createMenu();

    //点击合成
    void onCombineButtonClicked1(Iingredients& ingredient);
    void onCombineButtonClicked2(Iingredients& ingredient);

    //更新UI
    void updateUI();

    //创建实例
    static CookLayer* create();

    // 初始化键盘监听器
    void initKeyboardListener();

    // 切换菜单显示与隐藏
    void toggleMenuVisibility();

    EventListenerKeyboard* listener;
private:
    std::vector<Iingredients> ingredients;
    Iingredients* selectedItem;
    bool menuVisible;  // 记录菜单的显示状态
};


#endif 
#pragma once
