#ifndef __TASK__
#define __TASK__

#include "cocos2d.h"
USING_NS_CC;

class Task {
private:
	std::vector<Task*> child_tasks_;
public:
	~Task();
	std::string name;
	std::string content;
	bool status;
	void add_child_task(Task* child_task);
	Task* get_task_by_name(std::string name);
};

class TaskManager{
private:
	std::vector<Task*> tasks_;
public:
	void add_task(Task* task);
	void remove_task(Task* task);
	Task* get_task_by_name(std::string name);
	Task* get_task_by_index(int i) { return tasks_[i]; }
	int size() { return tasks_.size(); }
};

class TaskLayer : public Layer {
private:
	int visible_flag_ = 0;
	TaskManager* manager_;
public:
	static TaskLayer* create(TaskManager* manager);
	bool init();
	void menuCloseCallback(Ref* sender, int index);
};

#endif // !__TASK__

