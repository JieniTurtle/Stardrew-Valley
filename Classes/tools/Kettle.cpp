#include "Kettle.h"
#include "SimpleAudioEngine.h"

int MatureTime = 8; //成熟时间只浇水

bool Kettle::init(TMXTiledMap* map) {
    if (!ToolBase::init(map)) {
        return false;
    }
    
    // 初始化向后兼容的成员变量
    iskettle = isActive;
    
    scheduleCounter = 0;
    
    // 使用基类的统一方法设置复选框
    setCheckbox("kettle1.png", "kettle2.png", 2.5f / 8.0f, 1.0f / 4.0f);
    
    // 同步向后兼容的成员变量
    kettlecheckbox = checkbox;
    
    // 使用基类的统一方法设置鼠标监听
    setupMouseListener(map);
    
    return true;
}

Kettle* Kettle::create(TMXTiledMap* map) {
    Kettle* ret = new Kettle();
    if (ret && ret->init(map)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Kettle::handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) {
    // 水壶特定的处理逻辑：浇水
    auto tileLayer = map->getLayer("soil");
    if (!tileLayer) {
        return;
    }
    
    int tileGID = tileLayer->getTileGIDAt(Vec2(tileX, tileY));
    
    if (tileGID == RawPlantNoWaterID) {
        tileLayer->setTileGID(RawPlantWaterID, Vec2(tileX, tileY));
        auto callback = [this, tileX, tileY, tileLayer]() {
            tileLayer->setTileGID(MaturePlantID, Vec2(tileX, tileY));
        };
        
        if (weather == 2) {
            MatureTime = 4;
        } else if (weather == 1) {
            MatureTime = 10;
        } else {
            MatureTime = 8;
        }
        
        Action* rebackaction = Sequence::create(DelayTime::create(MatureTime), CallFunc::create(callback), nullptr);
        this->runAction(rebackaction);
        
        if (scheduleCounter > 1000000) {
            scheduleCounter = 0;
        }
        std::string updateKey = "update_function_key_" + std::to_string(scheduleCounter++);
        this->schedule([=](float dt) {
            myUpdateFunction(rebackaction, tileLayer, tileX, tileY, updateKey);
        }, 0.1f, updateKey);
    }
    
    if (tileGID == RawPlantNoWaterTwoID) {
        tileLayer->setTileGID(RawPlantWaterTwoID, Vec2(tileX, tileY));
        auto callback = [this, tileX, tileY, tileLayer]() {
            tileLayer->setTileGID(MaturePlantTwoID, Vec2(tileX, tileY));
        };
        
        Action* rebackaction = Sequence::create(DelayTime::create(MatureTime), CallFunc::create(callback), nullptr);
        this->runAction(rebackaction);
        
        if (scheduleCounter > 1000000) {
            scheduleCounter = 0;
        }
        std::string updateKey = "update_function_key_" + std::to_string(scheduleCounter++);
        this->schedule([=](float dt) {
            myUpdateFunction(rebackaction, tileLayer, tileX, tileY, updateKey);
        }, 0.1f, updateKey);
    }
}

void Kettle::myUpdateFunction(Action* rebackaction, TMXLayer* tileLayer, int tileX, int tileY, std::string updateKey) {
    if (rebackaction) {
        if ((tileLayer->getTileGIDAt(Vec2(tileX, tileY)) != RawPlantWaterID) && 
            (tileLayer->getTileGIDAt(Vec2(tileX, tileY)) != RawPlantWaterTwoID)) {
            this->stopAction(rebackaction);
            this->unschedule(updateKey);
        }
    }
}
