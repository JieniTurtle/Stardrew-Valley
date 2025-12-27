#include "NPCManager.h"
#include <algorithm>

NPCManager* NPCManager::_instance = nullptr;

NPCManager::NPCManager()
    : _initialPoolSize(10)
    , _maxPoolSize(50)
{
}

NPCManager::~NPCManager()
{
    clearAll();
}

NPCManager* NPCManager::getInstance()
{
    if (!_instance)
    {
        _instance = new NPCManager();
    }
    return _instance;
}

void NPCManager::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void NPCManager::init(size_t initialPoolSize, size_t maxPoolSize)
{
    _initialPoolSize = initialPoolSize;
    _maxPoolSize = maxPoolSize;
    
    // 可以为常用的 NPC 预创建池
    // 例如：Willy, Gus, Jodi, Harvey 等
    std::vector<std::string> commonNPCs = {"Willy", "Gus", "Jodi", "Harvey"};
    
    for (const auto& npcName : commonNPCs)
    {
        if (_npcPools.find(npcName) == _npcPools.end())
        {
            _npcPools[npcName] = new ObjectPool<NPC>(
                initialPoolSize,
                maxPoolSize,
                [npcName]() -> NPC* {
                    return NPC::create(npcName);
                }
            );
        }
    }
}

NPC* NPCManager::acquireNPC(const std::string& filename)
{
    // 如果该类型的池不存在，创建一个
    if (_npcPools.find(filename) == _npcPools.end())
    {
        _npcPools[filename] = new ObjectPool<NPC>(
            _initialPoolSize,
            _maxPoolSize,
            [filename]() -> NPC* {
                return NPC::create(filename);
            }
        );
    }
    
    ObjectPool<NPC>* pool = _npcPools[filename];
    if (!pool)
    {
        return nullptr;
    }
    
    NPC* npc = pool->acquire();
    if (npc)
    {
        // 设置 NPC 名称（如果需要）
        npc->NPCname = filename;
        _activeNPCs.push_back(npc);
    }
    return npc;
}

void NPCManager::returnNPC(NPC* npc)
{
    if (!npc)
    {
        return;
    }
    
    // 从活跃列表中移除
    _activeNPCs.erase(
        std::remove(_activeNPCs.begin(), _activeNPCs.end(), npc),
        _activeNPCs.end()
    );
    
    // 找到对应的池并归还
    std::string npcName = npc->NPCname;
    if (_npcPools.find(npcName) != _npcPools.end())
    {
        _npcPools[npcName]->returnObject(npc);
    }
}

void NPCManager::clearAll()
{
    // 清理活跃对象
    for (NPC* npc : _activeNPCs)
    {
        if (npc)
        {
            std::string npcName = npc->NPCname;
            if (_npcPools.find(npcName) != _npcPools.end())
            {
                _npcPools[npcName]->returnObject(npc);
            }
        }
    }
    _activeNPCs.clear();
    
    // 清理对象池
    for (auto& pair : _npcPools)
    {
        if (pair.second)
        {
            pair.second->clear();
            delete pair.second;
        }
    }
    _npcPools.clear();
}

void NPCManager::printStatistics() const
{
    CCLOG("=== NPCManager Statistics ===");
    for (const auto& pair : _npcPools)
    {
        CCLOG("NPC Pool [%s]: Available=%zu, Total=%zu",
              pair.first.c_str(),
              pair.second->getAvailableCount(),
              pair.second->getTotalCount());
    }
    CCLOG("Active NPCs: %zu", _activeNPCs.size());
}

