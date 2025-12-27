// Refactored with Facade Pattern
#ifndef FARM_SCENE_FACADE_H
#define FARM_SCENE_FACADE_H

class GameScene;
class TMXTiledMap;

class FarmSceneFacade {
public:
    static void setupUI(GameScene* scene);
    static void initCommonResources(GameScene* scene, TMXTiledMap* map);
    static void initTownNPCs(GameScene* scene, TMXTiledMap* map);
    static void initFarmScene(GameScene* scene, TMXTiledMap* map);
};

#endif // FARM_SCENE_FACADE_H

