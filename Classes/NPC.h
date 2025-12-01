#ifndef __NPC_SCENE_H__
#define __NPC_SCENE_H__
#include "cocos2d.h"
#include "Dialog.h"
#include "Task.h"
#include "ObjectPool.h"

USING_NS_CC;

class NPC : public cocos2d::Sprite, public IPoolable
{
private:
    Sprite* animate_sprite;
public:
    int relation;
    //����
    ~NPC() {
        moveup->release();
        moveleft->release();
        moveright->release();
        movedown->release();
        movestatic->release();
    }
    //NPC����
    std::string NPCname;

    TMXTiledMap* NPCmap;//��ͼָ��
    //�����ͼָ��
    void setMap(TMXTiledMap* map) { NPCmap = map; }

    //NPC�����С
    static int NPCsize_x;
    static int NPCsize_y;

    //NPC�����Ķ���˳��
    static int NPCorder_up;
    static int NPCorder_left;
    static int NPCorder_right;
    static int NPCorder_down;

    //NPC���� ����·��ʾ����NPC/filename/moveup1.png
    Animate* movestatic;
    Animate* moveup;
    Animate* moveleft;
    Animate* moveright;
    Animate* movedown;

    //NPC����
    Task NPCtask;

    //NPC�Ի�
    Dialog* Dialog_NPC;

    //·�����
    int currentPathIndex;//·������
    float speed;//�ٶ�
    std::vector<Vec2> NPCpath;//·������

    // �����ƶ�·��
    void setPath(const std::vector<Vec2>& path);

    //NPC��ֹͼƬ·��Ϊ NPC/filename/static.png
    static NPC* create(const std::string& filename);

    //��ʼ��
    bool init();

    //NPC�ƶ��ĸ���
    void updatemove(float dt);

    //NPC��ֹ�ĸ���
    void updatestatic(float dt);

    // ���Ŷ���
    void playAnimation(const std::string& direction);

    // ֹͣNPC�Ķ������ƶ�
    void stopMovement();

    // �ָ�NPC�Ķ������ƶ�
    void startMovement();

    //�ж����λ���Ƿ���NPC��
	bool JudgeClickNPC(Vec2 clickPos, int mapscale);

	// IPoolable 接口实现
	virtual void reset() override;
	virtual bool isInUse() const override { return _inUse; }
	virtual void setInUse(bool inUse) override { _inUse = inUse; }

protected:
	bool _inUse;  // 是否正在使用中（对象池状态标记）
};
#endif // __NPC_SCENE_H__;