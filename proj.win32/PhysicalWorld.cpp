//#include "cocos2d.h"
//USING_NS_CC;
//
//void set_physical_map(TMXTiledMap* map) {
//	auto layer = map->getLayer("Buildings");
//	const int tile_width = 16, tile_height = 16;
//	const int scale = 2;
//
//
//	for (int x = 0; x < map->getMapSize().width; x++) {
//		for (int y = 0; y < map->getMapSize().height; y++) {
//			if (layer->getTileGIDAt(Vec2(x, y)) != 0) {
//				auto physics_body = PhysicsBody::createBox(Size(tile_width * scale, tile_height * scale), PhysicsMaterial(0.1f, 1.0f, 0.0f));
//				physics_body->setDynamic(false);
//				layer->getTileAt(Vec2(x, y))->addComponent(physics_body);
//			}
//		}
//	}
//
//	//auto physics_body = PhysicsBody::createBox(this->getContentSize() * this->getScale(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
//	//// this->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
//	//this->addComponent(physics_body);
//
//}