#include "Task.h"
#include "ui/CocosGUI.h"

Task::~Task()
{
	for (int i = 0; i < child_tasks_.size(); i++) {
		delete child_tasks_[i];
	}
}

void Task::add_child_task(Task* child_task)
{
	child_tasks_.push_back(child_task);
}

Task* Task::get_task_by_name(std::string name)
{
	for (int i = 0; i < child_tasks_.size(); i++) {
		if (child_tasks_[i]->name == name) {
			return child_tasks_[i];
		}
	}
	return nullptr;
}

Task* TaskManager::get_task_by_name(std::string name)
{
	for (int i = 0; i < tasks_.size(); i++) {
		if (tasks_[i]->name == name) {
			return tasks_[i];
		}
	}
	return nullptr;
}

void TaskManager::add_task(Task* task)
{
	tasks_.push_back(task);
}

void TaskManager::remove_task(Task* task)
{
	for (auto it = tasks_.begin(); it != tasks_.end(); ) {
		if (*it == task) {
			it = tasks_.erase(it); // erase 返回下一个有效迭代器
		}
		else {
			++it;
		}
	}
}

/****************************************************************
****************************************************************/

TaskLayer* TaskLayer::create(TaskManager* manager)
{
	TaskLayer* task_layer = new TaskLayer;
	if (task_layer) {
		task_layer->manager_ = manager;
		if (task_layer->init()) {
			return task_layer;
		}
	}
	return nullptr;
}

bool TaskLayer::init()
{
	this->setVisible(visible_flag_);

	auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
	
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		// flash task board every time you open it
		if (keyCode == EventKeyboard::KeyCode::KEY_F) {
			visible_flag_ = !visible_flag_;
			this->setVisible(visible_flag_);
			if (visible_flag_ == 1) {
				auto back_layer = Sprite::create("Menu/help_wanted.png");	// brown-yellow
				back_layer->setScale(2);

				auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
				back_layer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				this->addChild(back_layer, 1, "back1");
				// creating a Menu from a Vector of items
				Vector<MenuItem*> MenuItems;
				for (int i = 0; i < manager_->size() && i < 4; i++) {
					auto closeItem = MenuItemImage::create("Menu/unpressed_buttom.png", "Menu/unpressed_buttom.png", 
						[this, index = i](Ref* sender) {
							menuCloseCallback(sender, index);
						});
					auto label = Label::createWithTTF(manager_->get_task_by_index(i)->name, "Fonts/Marker Felt.ttf", 24);
					label->setTextColor(Color4B(0, 0, 0, 255));
					label->setPosition(closeItem->getContentSize() / 2);
					closeItem->addChild(label); 
					closeItem->setPosition(Vec2(100 * (0.5f - (i /2)), -20 + 60 * (- i % 2)));
					MenuItems.pushBack(closeItem);
				}

				auto exit_button = ui::Button::create("CloseNormal.png", "CloseSelected.png");
				exit_button->addClickEventListener([&](Ref* sender) {
					auto back = this->getChildByName<Sprite*>("back1");
					if (back) {
						visible_flag_ = !visible_flag_;
						this->setVisible(visible_flag_);
					}
					});
				exit_button->setPosition(Vec2(10, 10));
				back_layer->addChild(exit_button);


				/* repeat for as many menu items as needed */
				auto menu = Menu::createWithArray(MenuItems);
				menu->setPosition(Vec2(visibleSize.width / 6, visibleSize.height / 6));
				back_layer->addChild(menu, 2);
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return 1;
}

void TaskLayer::menuCloseCallback(Ref* sender, int index)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
	//const int content_tag = 114514;

	auto back = Sprite::create("Menu/task_content.png");
	back->setPosition(visibleSize / 2);
	this->addChild(back, 10, "back");
	log("content showed");
	auto description = Label::createWithTTF(manager_->get_task_by_index(index)->content, "Fonts/Marker Felt.ttf", 24);
	description->setTextColor(Color4B(0, 0, 0, 255));
	description->setPosition(back->getContentSize() / 2);
	back->addChild(description);

	auto exit_button = ui::Button::create("CloseNormal.png", "CloseSelected.png");
	exit_button->addClickEventListener([&](Ref* sender) {
		auto back = this->getChildByName<Sprite*>("back");
		if (back) {
			this->removeChild(back);
		}
		});
	exit_button->setPosition(Vec2(10, 10));
	back->addChild(exit_button);
}