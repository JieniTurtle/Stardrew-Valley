// Refactored with Factory Pattern
#ifndef ANIMAL_FACTORY_H
#define ANIMAL_FACTORY_H

#include "animals.h"

enum class AnimalType {
    COW,
    SHEEP,
    CHICKEN,
};

class AnimalFactory {
public:
    static Animal* createAnimal(AnimalType type, const std::string& name) {
        switch (type) {
        case AnimalType::COW:
            return Cow::create(name);
        case AnimalType::SHEEP:
            return Sheep::create(name);
        case AnimalType::CHICKEN:
            return Chicken::create(name);
        default:
            return nullptr;
        }
    }
};

#endif // ANIMAL_FACTORY_H


