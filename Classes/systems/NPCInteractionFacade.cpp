#include "NPCInteractionFacade.h"

#include <vector>

#include "cocos2d.h"

#include "GameScene.h"
#include "NPC.h"
#include "Task.h"
#include "global.h"

USING_NS_CC;

namespace {

void handleNpcTask(NPC* npc) {
    if (!npc || !npc->Dialog_NPC) {
        return;
    }
    if (npc->Dialog_NPC->TaskStatus) {
        if (npc->Dialog_NPC->TaskStatus == 1) {
            Task* npcTask = new Task;
            npcTask->name = npc->NPCname + "TaskName";
            npcTask->content = npc->NPCname + "TaskContent";
            task_manager->add_task(npcTask);
        }
        else if (npc->Dialog_NPC->TaskStatus == 2) {
            Task* npctask = task_manager->get_task_by_name(npc->NPCname + "TaskName");
            task_manager->remove_task(npctask);
        }
        npc->Dialog_NPC->TaskStatus = 0;
    }
}

void closeDialog(GameScene* scene, NPC* npc) {
    if (!scene || !npc || !npc->Dialog_NPC) {
        return;
    }

    npc->startMovement();

    if (npc->relation == 100) {
        scene->relationFull->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 3));
        scene->addChild(scene->relationFull);
        scene->scheduleOnce([scene](float) {
            scene->relationFull->removeFromParent();
            }, 1.0f, "hide_relation_full_key");
    }
    else {
        if (npc->Dialog_NPC->ChangeToClose == 1) {
            npc->relation += 10;
            scene->relationTip = Label::createWithSystemFont("RealtionDegree+10!", "Arial", 30);
            scene->relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));
            scene->addChild(scene->relationTip);

            scene->scheduleOnce([scene](float) {
                if (scene->relationTip) {
                    scene->relationTip->removeFromParent();
                }
                }, 1.0f, "remove_message_key");
        }
        else if (npc->Dialog_NPC->ChangeToClose == 2) {
            npc->relation += 70;
            scene->relationTip = Label::createWithSystemFont("RealtionDegree+70!", "Arial", 30);
            scene->relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));
            scene->addChild(scene->relationTip);

            scene->scheduleOnce([scene](float) {
                if (scene->relationTip) {
                    scene->relationTip->removeFromParent();
                }
                }, 1.0f, "remove_message_key");
        }
    }
    npc->Dialog_NPC->ChangeToClose = 0;
}

void setupMouseListener(GameScene* scene) {
    if (!scene) {
        return;
    }

    auto listener = EventListenerMouse::create();
    listener->onMouseDown = [scene](Event* event) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto e = static_cast<EventMouse*>(event);
        Vec2 clickPos = e->getLocation() / GameScene::mapscale;
        clickPos.y = visibleSize.height / GameScene::mapscale - clickPos.y;

        if (scene->MouseStatus == notTaken) {
            auto tryNpc = [scene, &clickPos](NPC* npc) -> bool {
                if (!npc) {
                    return false;
                }
                if (npc->JudgeClickNPC(clickPos, GameScene::mapscale)) {
                    npc->stopMovement();
                    scene->addChild(npc->Dialog_NPC);
                    scene->MouseStatus = TakenByNPC;
                    return true;
                }
                return false;
            };

            if (tryNpc(scene->NPC_Willy)) return;
            if (tryNpc(scene->NPC_Gus)) return;
            if (tryNpc(scene->NPC_Jodi)) return;
            if (tryNpc(scene->NPC_Harvey)) return;
        }
    };

    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, scene);
}

void setupUpdate(GameScene* scene) {
    if (!scene) {
        return;
    }

    scene->schedule([scene](float) {
        auto processNpc = [scene](NPC* npc) {
            if (!npc || !npc->Dialog_NPC) {
                return;
            }
            if (npc->Dialog_NPC->ChangeToClose) {
                scene->MouseStatus = notTaken;
                closeDialog(scene, npc);
            }
            handleNpcTask(npc);
        };

        processNpc(scene->NPC_Willy);
        processNpc(scene->NPC_Gus);
        processNpc(scene->NPC_Jodi);
        processNpc(scene->NPC_Harvey);
    }, 0.1f, "npc_mouse_update");
}

} // namespace

void NPCInteractionFacade::setup(GameScene* scene, TMXTiledMap* map) {
    if (!scene || !map) {
        return;
    }

    std::vector<Vec2> path_Willy = { Vec2(28, 51), Vec2(29, 19), Vec2(-10, 19), Vec2(-10, 55), Vec2(26, 55), Vec2(28, 55) };
    scene->NPC_Willy = scene->initNPC("Willy", path_Willy, map);
    if (scene->NPC_Willy) {
        scene->NPC_Willy->retain();
    }

    std::vector<Vec2> path_Gus = { Vec2(81, 87),Vec2(20, 75), Vec2(20, 55), Vec2(-100, 55), Vec2(-100, 200), Vec2(81, 200) };
    scene->NPC_Gus = scene->initNPC("Gus", path_Gus, map);
    if (scene->NPC_Gus) {
        scene->NPC_Gus->retain();
    }

    std::vector<Vec2> path_Jodi = { Vec2(-10, 55), Vec2(28, 55), Vec2(28, 51), Vec2(29, 19), Vec2(-10, 19), Vec2(-10, 55), Vec2(26, 55) };
    scene->NPC_Jodi = scene->initNPC("Jodi", path_Jodi, map);
    if (scene->NPC_Jodi) {
        scene->NPC_Jodi->retain();
    }

    std::vector<Vec2> path_Harvey = { Vec2(-100, 200), Vec2(81, 200), Vec2(81, 87),Vec2(20, 75), Vec2(20, 55), Vec2(-100, 55) };
    scene->NPC_Harvey = scene->initNPC("Harvey", path_Harvey, map);
    if (scene->NPC_Harvey) {
        scene->NPC_Harvey->retain();
    }

    scene->relationFull = Sprite::create("NPC/relationFull.png");
    if (scene->relationFull) {
        scene->relationFull->retain();
    }

    setupMouseListener(scene);
    setupUpdate(scene);
}


