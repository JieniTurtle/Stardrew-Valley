#include "AnimalManager.h"
#include <iostream>

AnimalManager* AnimalManager::_instance = nullptr;

AnimalManager::AnimalManager()
    : _cowPool(nullptr)
    , _sheepPool(nullptr)
    , _chickenPool(nullptr)
{
}

AnimalManager::~AnimalManager()
{
    clearAll();
}

AnimalManager* AnimalManager::getInstance()
{
    if (!_instance)
    {
        _instance = new AnimalManager();
    }
    return _instance;
}

void AnimalManager::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void AnimalManager::init(size_t initialPoolSize, size_t maxPoolSize)
{
    // 创建 Cow 对象池，使用 lambda 函数来创建对象
    _cowPool = new ObjectPool<Cow>(
        initialPoolSize,
        maxPoolSize,
        []() -> Cow* {
            return Cow::create("cow");
        }
    );
    
    // 创建 Sheep 对象池
    _sheepPool = new ObjectPool<Sheep>(
        initialPoolSize,
        maxPoolSize,
        []() -> Sheep* {
            return Sheep::create("sheep");
        }
    );
    
    // 创建 Chicken 对象池
    _chickenPool = new ObjectPool<Chicken>(
        initialPoolSize,
        maxPoolSize,
        []() -> Chicken* {
            return Chicken::create("chicken");
        }
    );
}

Cow* AnimalManager::acquireCow(const std::string& filename)
{
    if (!_cowPool)
    {
        return nullptr;
    }
    
    Cow* cow = _cowPool->acquire();
    if (cow)
    {
        // 设置动物名称（如果需要）
        cow->animalName = filename;
        _activeCows.push_back(cow);
    }
    return cow;
}

void AnimalManager::returnCow(Cow* cow)
{
    if (!cow || !_cowPool)
    {
        return;
    }
    
    // 从活跃列表中移除
    _activeCows.erase(
        std::remove(_activeCows.begin(), _activeCows.end(), cow),
        _activeCows.end()
    );
    
    // 归还到池中
    _cowPool->returnObject(cow);
}

Sheep* AnimalManager::acquireSheep(const std::string& filename)
{
    if (!_sheepPool)
    {
        return nullptr;
    }
    
    Sheep* sheep = _sheepPool->acquire();
    if (sheep)
    {
        sheep->animalName = filename;
        _activeSheep.push_back(sheep);
    }
    return sheep;
}

void AnimalManager::returnSheep(Sheep* sheep)
{
    if (!sheep || !_sheepPool)
    {
        return;
    }
    
    _activeSheep.erase(
        std::remove(_activeSheep.begin(), _activeSheep.end(), sheep),
        _activeSheep.end()
    );
    
    _sheepPool->returnObject(sheep);
}

Chicken* AnimalManager::acquireChicken(const std::string& filename)
{
    if (!_chickenPool)
    {
        return nullptr;
    }
    
    Chicken* chicken = _chickenPool->acquire();
    if (chicken)
    {
        chicken->animalName = filename;
        _activeChickens.push_back(chicken);
    }
    return chicken;
}

void AnimalManager::returnChicken(Chicken* chicken)
{
    if (!chicken || !_chickenPool)
    {
        return;
    }
    
    _activeChickens.erase(
        std::remove(_activeChickens.begin(), _activeChickens.end(), chicken),
        _activeChickens.end()
    );
    
    _chickenPool->returnObject(chicken);
}

void AnimalManager::clearAll()
{
    // 清理活跃对象
    for (Cow* cow : _activeCows)
    {
        if (cow && _cowPool)
        {
            _cowPool->returnObject(cow);
        }
    }
    _activeCows.clear();
    
    for (Sheep* sheep : _activeSheep)
    {
        if (sheep && _sheepPool)
        {
            _sheepPool->returnObject(sheep);
        }
    }
    _activeSheep.clear();
    
    for (Chicken* chicken : _activeChickens)
    {
        if (chicken && _chickenPool)
        {
            _chickenPool->returnObject(chicken);
        }
    }
    _activeChickens.clear();
    
    // 清理对象池
    if (_cowPool)
    {
        _cowPool->clear();
        delete _cowPool;
        _cowPool = nullptr;
    }
    
    if (_sheepPool)
    {
        _sheepPool->clear();
        delete _sheepPool;
        _sheepPool = nullptr;
    }
    
    if (_chickenPool)
    {
        _chickenPool->clear();
        delete _chickenPool;
        _chickenPool = nullptr;
    }
}

void AnimalManager::printStatistics() const
{
    CCLOG("=== AnimalManager Statistics ===");
    if (_cowPool)
    {
        CCLOG("Cow Pool: Available=%zu, Total=%zu, Active=%zu",
              _cowPool->getAvailableCount(),
              _cowPool->getTotalCount(),
              _activeCows.size());
    }
    if (_sheepPool)
    {
        CCLOG("Sheep Pool: Available=%zu, Total=%zu, Active=%zu",
              _sheepPool->getAvailableCount(),
              _sheepPool->getTotalCount(),
              _activeSheep.size());
    }
    if (_chickenPool)
    {
        CCLOG("Chicken Pool: Available=%zu, Total=%zu, Active=%zu",
              _chickenPool->getAvailableCount(),
              _chickenPool->getTotalCount(),
              _activeChickens.size());
    }
}

