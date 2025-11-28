#ifndef SEED_FACTORY_H
#define SEED_FACTORY_H

#include "cocos2d.h"
#include "Seeds.h"
#include "Seedstwo.h"
#include "Wheat.h"
#include "Carrot.h"

enum class SeedType {
    SEEDS,
    SEEDSTWO,
    WHEAT,
    CARROT,
};

class SeedFactory {
public:
    static cocos2d::Node* createSeed(SeedType type, cocos2d::TMXTiledMap* map) {
        switch (type) {
        case SeedType::SEEDS:
            return Seeds::create(map);
        case SeedType::SEEDSTWO:
            return Seedstwo::create(map);
        case SeedType::WHEAT:
            return Wheat::create(map);
        case SeedType::CARROT:
            return Carrot::create(map);
        default:
            return nullptr;
        }
    }
};

#endif // SEED_FACTORY_H


