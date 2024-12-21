#include "ingredient.h"
USING_NS_CC;

bool CookLayer::init() {
    // 创建物品
    
    createItems();

    // 创建UI界面
    createMenu();
    this->retain();
    // 初始化键盘事件监听器
    initKeyboardListener();
    toggleMenuVisibility();//一开始先关闭界面
    listener->setEnabled(false); // 禁用按键监听
    updateUI();
    return true;
}

void CookLayer::createItems() {
    ingredients.push_back(Iingredients("Milk", &milk_number, "milk.png"));
    ingredients.push_back(Iingredients("Egg", &egg_number, "egg.png"));
    ingredients.push_back(Iingredients("Cake", &cake_number, "cake.png"));
    ingredients.push_back(Iingredients("Carrot", &carrot_number, "carrot.png"));
    ingredients.push_back(Iingredients("Wheat", &wheat_number, "wheat.png"));
    ingredients.push_back(Iingredients("Soup", &soup_number, "soup.png"));
}

void CookLayer::createMenu() {
    // 获取屏幕的可见尺寸和原点
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // 居中位置的X和Y坐标
    float centerX = origin.x + visibleSize.width / 2;
    float centerY = origin.y + visibleSize.height / 2;

    // 初始横向排列起始位置
    float startX = centerX - 150;  // 让第一个物品略偏左以居中排列
    float yPos = centerY+100;  // 所有物品的Y坐标保持一致

    // 创建物品
    for (int i = 0; i < ingredients.size(); ++i) {
        auto& item = ingredients[i];

        // 创建物品图标
        auto itemSprite = cocos2d::Sprite::create(item.imageFile);
        itemSprite->setPosition(cocos2d::Vec2(startX + (i % 3) * 150, yPos));  // 横向排列
        this->addChild(itemSprite);
        if (i == 0)
        {
            itemSprite->setScale(0.5f);  // 缩小图标
            itemSprite->setTag(i + 8);
        }
        else if (i == 1)
        {
            itemSprite->setScale(0.625f);  // 缩小图标
            itemSprite->setTag(i + 8);
        }
        else if (i == 2)
        {
            itemSprite->setScale(0.533f);  // 缩小图标
            itemSprite->setTag(i + 8);
        }
        else if (i == 3)
        {
            itemSprite->setScale(0.3125f);  // 缩小图标
            itemSprite->setTag(i + 8);
        }
        else if (i == 4)
        {
            itemSprite->setScale(0.8f);  // 缩小图标
            itemSprite->setTag(i + 8);
        }
        else if (i == 5)
        {
            itemSprite->setScale(0.4f);  // 缩小图标
            itemSprite->setTag(i + 8);
        }
        // 创建物品数量的显示
        auto quantityLabel = cocos2d::Label::createWithTTF(std::to_string(*item.quantity), "fonts/Marker Felt.ttf", 24);
        quantityLabel->setPosition(cocos2d::Vec2(startX + (i % 3) * 150, yPos - 50));  // 放在图标下方
        quantityLabel->setTag(i);  // 设置标签以便在按钮点击事件中获取
        this->addChild(quantityLabel);

        // 创建合成按钮
        if (i == 2) {
            auto combineButton = cocos2d::ui::Button::create("combine_button.jpg", "combine_button.jpg");  // 设置按钮的背景图片
            combineButton->setTitleText("Combine");
            combineButton->setTitleFontSize(15);  // 设置按钮文本字体大小
            combineButton->setTitleColor(cocos2d::Color3B::BLACK);  // 设置按钮文本颜色
            combineButton->setPosition(cocos2d::Vec2(startX + (i % 3) * 150, yPos - 80));  // 合成按钮放在数量标签下方
            combineButton->addClickEventListener([this, &item](cocos2d::Ref* sender) {
                onCombineButtonClicked1(item);  // 按钮点击事件
                });
            this->addChild(combineButton);
            if(i==2)
                combineButton->setTag(6);// 设置标签以便在按钮点击事件中获取
            else if(i==5)
                combineButton->setTag(7);// 设置标签以便在按钮点击事件中获取
        }
        // 创建合成按钮
        if (i == 5) {
            auto combineButton = cocos2d::ui::Button::create("combine_button.jpg", "combine_button.jpg");  // 设置按钮的背景图片
            combineButton->setTitleText("Combine");
            combineButton->setTitleFontSize(15);  // 设置按钮文本字体大小
            combineButton->setTitleColor(cocos2d::Color3B::BLACK);  // 设置按钮文本颜色
            combineButton->setPosition(cocos2d::Vec2(startX + (i % 3) * 150, yPos - 80));  // 合成按钮放在数量标签下方
            combineButton->addClickEventListener([this, &item](cocos2d::Ref* sender) {
                onCombineButtonClicked2(item);  // 按钮点击事件
                });
            this->addChild(combineButton);
            if (i == 2)
                combineButton->setTag(6);// 设置标签以便在按钮点击事件中获取
            else if (i == 5)
                combineButton->setTag(7);// 设置标签以便在按钮点击事件中获取
        }

        if (i == 2) {
            yPos -= 150;  // 换行
        }
    }
}


void CookLayer::onCombineButtonClicked1(Iingredients& ingredient) {
    if (ingredient.ingredientsName == "Cake") {
        bool canCombine = true;

        for (auto& material : ingredients) {
            if (material.ingredientsName == "Milk" && !material.consumeQuantity(5)) {
                canCombine = false;
            }
            if (material.ingredientsName == "Egg" && !material.consumeQuantity(5)) {
                canCombine = false;
            }
        }

        if (canCombine) {
            ingredient.addQuantity(1);
            CCLOG("Successfully created a Potion!");
        }
        else {
            CCLOG("Not enough materials.");
        }
    }

    //updateUI();
}
void CookLayer::onCombineButtonClicked2(Iingredients& ingredient) {
    if (ingredient.ingredientsName == "Soup") {
        bool canCombine = true;

        for (auto& material : ingredients) {
            if (material.ingredientsName == "Carrot" && !material.consumeQuantity(5)) {
                canCombine = false;
            }
            if (material.ingredientsName == "Wheat" && !material.consumeQuantity(5)) {
                canCombine = false;
            }
        }

        if (canCombine) {
            ingredient.addQuantity(1);
            CCLOG("Successfully created a Potion!");
        }
        else {
            CCLOG("Not enough materials.");
        }
    }

    updateUI();
}
void CookLayer::updateUI() {

    this->schedule([this](float dt) {
        // 这里编写需要执行的逻辑
        for (int i = 0; i < ingredients.size(); i++) {
            auto label = (cocos2d::Label*)this->getChildByTag(i);
            label->setString(std::to_string(*(ingredients[i].quantity)));
        }
        }, 0.2f, "lambda_key_cook"); // 使用一个键来标识这个调度
}

CookLayer* CookLayer::create() {
    CookLayer* ret = new CookLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

void CookLayer::initKeyboardListener() {
     listener = cocos2d::EventListenerKeyboard::create();

    // 设置按键事件处理函数
    listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
        if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_I) {
            toggleMenuVisibility();  // 切换合成菜单的显示与关闭
        }
        };

    // 获取事件分发器并添加监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// 切换菜单显示与隐藏
void CookLayer::toggleMenuVisibility() {
    menuVisible = !menuVisible;  // 切换状态
    for (int i = 0; i < ingredients.size()+8; i++) {
        this->getChildByTag(i)->setVisible(menuVisible);  // 设置合成按钮的可见性
    }
}
