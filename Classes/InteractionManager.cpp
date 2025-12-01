#include "InteractionManager.h"
#include <algorithm>

InteractionManager* InteractionManager::_instance = nullptr;

InteractionManager::InteractionManager()
    : _interactionPool(nullptr)
    , _bridgePool(nullptr)
    , _chestPool(nullptr)
    , _gobangBoardPool(nullptr)
{
}

InteractionManager::~InteractionManager()
{
    clearAll();
}

InteractionManager* InteractionManager::getInstance()
{
    if (!_instance)
    {
        _instance = new InteractionManager();
    }
    return _instance;
}

void InteractionManager::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void InteractionManager::init(size_t initialPoolSize, size_t maxPoolSize)
{
    // 创建 Interaction 对象池（注意：ObjectPool 的 creator 函数不接受参数）
    // 所以我们需要在 acquire 时设置 map
    _interactionPool = new ObjectPool<Interaction>(
        initialPoolSize,
        maxPoolSize,
        []() -> Interaction* {
            return Interaction::create(nullptr);  // 临时创建，map 会在 acquire 时设置
        }
    );
    
    // 创建 Bridge 对象池
    _bridgePool = new ObjectPool<Bridge>(
        initialPoolSize,
        maxPoolSize,
        []() -> Bridge* {
            return Bridge::create(nullptr);  // 临时创建，map 会在 acquire 时设置
        }
    );
    
    // 创建 Chest 对象池（注意：Chest::create 需要 ID 参数）
    // 这里使用默认 ID=0，实际使用时需要在 acquire 时设置正确的 ID
    _chestPool = new ObjectPool<Chest>(
        initialPoolSize,
        maxPoolSize,
        []() -> Chest* {
            return Chest::create(nullptr, 0);  // 默认 ID，实际使用时需要重新设置
        }
    );
    
    // 创建 GobangBoard 对象池
    _gobangBoardPool = new ObjectPool<GobangBoard>(
        initialPoolSize,
        maxPoolSize,
        []() -> GobangBoard* {
            return GobangBoard::create(nullptr);  // 临时创建，map 会在 acquire 时设置
        }
    );
}

Interaction* InteractionManager::acquireInteraction(TMXTiledMap* map)
{
    if (!_interactionPool || !map)
    {
        return nullptr;
    }
    
    Interaction* interaction = _interactionPool->acquire();
    if (interaction)
    {
        // 设置 map（如果对象是从池中复用的，map 可能已经被 reset() 清空）
        interaction->map_ = map;
        _activeInteractions.push_back(interaction);
    }
    return interaction;
}

Bridge* InteractionManager::acquireBridge(TMXTiledMap* map)
{
    if (!_bridgePool)
    {
        return nullptr;
    }
    
    Bridge* bridge = _bridgePool->acquire();
    if (bridge)
    {
        bridge->map_ = map;
        _activeBridges.push_back(bridge);
    }
    return bridge;
}

Chest* InteractionManager::acquireChest(TMXTiledMap* map, int ID)
{
    if (!_chestPool)
    {
        return nullptr;
    }
    
    Chest* chest = _chestPool->acquire();
    if (chest)
    {
        chest->map_ = map;
        chest->ID_ = ID;  // 设置正确的 ID
        _activeChests.push_back(chest);
    }
    return chest;
}

GobangBoard* InteractionManager::acquireGobangBoard(TMXTiledMap* map)
{
    if (!_gobangBoardPool)
    {
        return nullptr;
    }
    
    GobangBoard* board = _gobangBoardPool->acquire();
    if (board)
    {
        board->map_ = map;
        _activeGobangBoards.push_back(board);
    }
    return board;
}

void InteractionManager::returnInteraction(Interaction* interaction)
{
    if (!interaction)
    {
        return;
    }
    
    // 根据类型归还到对应的池
    if (Bridge* bridge = dynamic_cast<Bridge*>(interaction))
    {
        _activeBridges.erase(
            std::remove(_activeBridges.begin(), _activeBridges.end(), bridge),
            _activeBridges.end()
        );
        if (_bridgePool)
        {
            _bridgePool->returnObject(bridge);
        }
    }
    else if (Chest* chest = dynamic_cast<Chest*>(interaction))
    {
        _activeChests.erase(
            std::remove(_activeChests.begin(), _activeChests.end(), chest),
            _activeChests.end()
        );
        if (_chestPool)
        {
            _chestPool->returnObject(chest);
        }
    }
    else if (GobangBoard* board = dynamic_cast<GobangBoard*>(interaction))
    {
        _activeGobangBoards.erase(
            std::remove(_activeGobangBoards.begin(), _activeGobangBoards.end(), board),
            _activeGobangBoards.end()
        );
        if (_gobangBoardPool)
        {
            _gobangBoardPool->returnObject(board);
        }
    }
    else
    {
        _activeInteractions.erase(
            std::remove(_activeInteractions.begin(), _activeInteractions.end(), interaction),
            _activeInteractions.end()
        );
        if (_interactionPool)
        {
            _interactionPool->returnObject(interaction);
        }
    }
}

void InteractionManager::clearAll()
{
    // 清理活跃对象
    for (Bridge* bridge : _activeBridges)
    {
        if (bridge && _bridgePool)
        {
            _bridgePool->returnObject(bridge);
        }
    }
    _activeBridges.clear();
    
    for (Chest* chest : _activeChests)
    {
        if (chest && _chestPool)
        {
            _chestPool->returnObject(chest);
        }
    }
    _activeChests.clear();
    
    for (GobangBoard* board : _activeGobangBoards)
    {
        if (board && _gobangBoardPool)
        {
            _gobangBoardPool->returnObject(board);
        }
    }
    _activeGobangBoards.clear();
    
    for (Interaction* interaction : _activeInteractions)
    {
        if (interaction && _interactionPool)
        {
            _interactionPool->returnObject(interaction);
        }
    }
    _activeInteractions.clear();
    
    // 清理对象池
    if (_interactionPool)
    {
        _interactionPool->clear();
        delete _interactionPool;
        _interactionPool = nullptr;
    }
    
    if (_bridgePool)
    {
        _bridgePool->clear();
        delete _bridgePool;
        _bridgePool = nullptr;
    }
    
    if (_chestPool)
    {
        _chestPool->clear();
        delete _chestPool;
        _chestPool = nullptr;
    }
    
    if (_gobangBoardPool)
    {
        _gobangBoardPool->clear();
        delete _gobangBoardPool;
        _gobangBoardPool = nullptr;
    }
}

void InteractionManager::printStatistics() const
{
    CCLOG("=== InteractionManager Statistics ===");
    if (_interactionPool)
    {
        CCLOG("Interaction Pool: Available=%zu, Total=%zu, Active=%zu",
              _interactionPool->getAvailableCount(),
              _interactionPool->getTotalCount(),
              _activeInteractions.size());
    }
    if (_bridgePool)
    {
        CCLOG("Bridge Pool: Available=%zu, Total=%zu, Active=%zu",
              _bridgePool->getAvailableCount(),
              _bridgePool->getTotalCount(),
              _activeBridges.size());
    }
    if (_chestPool)
    {
        CCLOG("Chest Pool: Available=%zu, Total=%zu, Active=%zu",
              _chestPool->getAvailableCount(),
              _chestPool->getTotalCount(),
              _activeChests.size());
    }
    if (_gobangBoardPool)
    {
        CCLOG("GobangBoard Pool: Available=%zu, Total=%zu, Active=%zu",
              _gobangBoardPool->getAvailableCount(),
              _gobangBoardPool->getTotalCount(),
              _activeGobangBoards.size());
    }
}

