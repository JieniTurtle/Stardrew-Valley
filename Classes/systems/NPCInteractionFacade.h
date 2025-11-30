#ifndef NPC_INTERACTION_FACADE_H
#define NPC_INTERACTION_FACADE_H

class GameScene;
class TMXTiledMap;

class NPCInteractionFacade {
public:
    // 统一创建 Town 场景中的 NPC，并设置交互和任务逻辑
    static void setup(GameScene* scene, TMXTiledMap* map);
};

#endif // NPC_INTERACTION_FACADE_H


