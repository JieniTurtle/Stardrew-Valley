#ifndef __NPC_SCENE_H__
#define __NPC_SCENE_H__
#include "cocos2d.h"
#include "Dialog.h"
#include "Task.h"

USING_NS_CC;

class NPC : public cocos2d::Sprite
{
public:
    //析构
    ~NPC() {
        moveup->release();
        moveleft->release();
        moveright->release();
        movedown->release();
        movestatic->release();
    }
    //NPC名称
    std::string NPCname;

    TMXTiledMap* NPCmap;//地图指针
    //传入地图指针
    void setMap(TMXTiledMap* map) { NPCmap = map; }

    //NPC精灵大小
    static int NPCsize_x;
    static int NPCsize_y;

    //NPC纹理的动画顺序
    static int NPCorder_up;
    static int NPCorder_left;
    static int NPCorder_right;
    static int NPCorder_down;

    //NPC动画 动画路径示例：NPC/filename/moveup1.png
    Animate* movestatic;
    Animate* moveup;
    Animate* moveleft;
    Animate* moveright;
    Animate* movedown;

    //标记NPC是否被选中
    bool ifSelected;

    //NPC任务
    Task NPCtask;

    //NPC对话
    Dialog* Dialog_NPC;

    //路径相关
    int currentPathIndex;//路径索引
    float speed;//速度
    std::vector<Vec2> NPCpath;//路径参数

    // 设置移动路径
    void setPath(const std::vector<Vec2>& path);

    //NPC静止图片路径为 NPC/filename/static.png
    static NPC* create(const std::string& filename);

    //初始化
    bool init();

    //NPC移动的更新
    void updatemove(float dt);

    //NPC静止的更新
    void updatestatic(float dt);

    // 播放动画
    void playAnimation(const std::string& direction);

    // 停止NPC的动画和移动
    void stopMovement();

    // 恢复NPC的动画和移动
    void startMovement();

    //判断鼠标位置是否在NPC上
    bool JudgeClickNPC(Vec2 clickPos, int mapscale);
};
#endif // __NPC_SCENE_H__;