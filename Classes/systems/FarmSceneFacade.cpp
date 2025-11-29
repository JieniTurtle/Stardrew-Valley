#include "FarmSceneFacade.h"

#include "GameScene.h"
#include "ToolFactory.h"
#include "SeedFactory.h"
#include "AnimalFactory.h"
#include "ToolUIFacade.h"
#include "NPCInteractionFacade.h"
#include "Date.h"
#include "MiniGame.h"

USING_NS_CC;

namespace {
    void addLayerIfNeeded(GameScene* scene, Layer* layer, int zOrder = 1) {
        if (layer) {
            scene->addChild(layer, zOrder);
        }
    }
}

void FarmSceneFacade::setupUI(GameScene* scene) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    std::string expString = "Experience: " + std::to_string(experience);
    auto label = Label::createWithTTF(expString, "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9.5 / 10));
    label->setColor(Color3B::GREEN);
    scene->addChild(label, 1);

    scene->schedule([label](float) {
        label->setString("Experience: " + std::to_string(experience));
    }, "update_label_key_exp");

    money = 10000;
    std::string moneyString = "Money: " + std::to_string(money);
    auto moneyLabel = Label::createWithTTF(moneyString, "fonts/Marker Felt.ttf", 24);
    moneyLabel->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9 / 10));
    moneyLabel->setColor(Color3B::YELLOW);
    moneyLabel->setVisible(true);
    scene->addChild(moneyLabel, 3);

    scene->schedule([moneyLabel](float) {
        moneyLabel->setString("Money: " + std::to_string(money));
    }, "update_label_key");
}

void FarmSceneFacade::initCommonResources(GameScene* scene, TMXTiledMap* map) {
    scene->mineral = Mineral::create(map);
    addLayerIfNeeded(scene, scene->mineral, 1);

    scene->seeds = static_cast<Seeds*>(SeedFactory::createSeed(SeedType::SEEDS, map));
    addLayerIfNeeded(scene, scene->seeds, 1);

    scene->wheat = static_cast<Wheat*>(SeedFactory::createSeed(SeedType::WHEAT, map));
    addLayerIfNeeded(scene, scene->wheat, 1);

    scene->seedstwo = static_cast<Seedstwo*>(SeedFactory::createSeed(SeedType::SEEDSTWO, map));
    addLayerIfNeeded(scene, scene->seedstwo, 1);

    scene->carrot = static_cast<Carrot*>(SeedFactory::createSeed(SeedType::CARROT, map));
    addLayerIfNeeded(scene, scene->carrot, 1);

    scene->wood = Wood::create(map);
    addLayerIfNeeded(scene, scene->wood, 1);

    scene->farmproduct = FarmProduct::create(map);
    addLayerIfNeeded(scene, scene->farmproduct, 1);

    scene->fertilizer = Fertilizer::create(map);
    addLayerIfNeeded(scene, scene->fertilizer, 1);

    scene->cookLayer = CookLayer::create();
    addLayerIfNeeded(scene, scene->cookLayer, 1);

    scene->store = Store::create(map);
    addLayerIfNeeded(scene, scene->store, 3);
}

void FarmSceneFacade::initTownNPCs(GameScene* scene, TMXTiledMap* map) {
    NPCInteractionFacade::setup(scene, map);
}

void FarmSceneFacade::initFarmScene(GameScene* scene, TMXTiledMap* map) {
    auto addToolToScene = [scene](Layer* toolLayer) {
        if (toolLayer) {
            scene->addChild(toolLayer, 1);
        }
    };

    scene->tools = static_cast<Tools*>(ToolFactory::createTool(ToolType::TOOLS, map));
    addToolToScene(scene->tools);

    scene->gloves = static_cast<Gloves*>(ToolFactory::createTool(ToolType::GLOVES, map));
    addToolToScene(scene->gloves);

    scene->axe = static_cast<Axe*>(ToolFactory::createTool(ToolType::AXE, map));
    addToolToScene(scene->axe);

    scene->kettle = static_cast<Kettle*>(ToolFactory::createTool(ToolType::KETTLE, map));
    addToolToScene(scene->kettle);

    scene->pickaxe = static_cast<Pickaxe*>(ToolFactory::createTool(ToolType::PICKAXE, map));
    addToolToScene(scene->pickaxe);

    scene->fishing = Fishing::create(map, scene->character);
    addLayerIfNeeded(scene, scene->fishing, 1);

    auto cow = static_cast<Cow*>(AnimalFactory::createAnimal(AnimalType::COW, "cow"));
    cow->setMaincharacter(scene->character);
    cow->setMap(map);
    Cow::move(cow, map);

    auto sheep = static_cast<Sheep*>(AnimalFactory::createAnimal(AnimalType::SHEEP, "sheep"));
    sheep->setMaincharacter(scene->character);
    sheep->setMap(map);
    Sheep::move(sheep, map);

    auto chicken = static_cast<Chicken*>(AnimalFactory::createAnimal(AnimalType::CHICKEN, "chicken"));
    chicken->setMaincharacter(scene->character);
    chicken->setMap(map);
    Chicken::move(chicken, map);

    ToolUIFacade::setup(scene);
}

