#ifndef __TASK_OBSERVER_H__
#define __TASK_OBSERVER_H__

#include <string>

class TaskObserver {
public:
    virtual ~TaskObserver() = default;
    virtual void onTaskAssigned(const std::string& user) = 0;
    virtual void onTaskProgressUpdated(const std::string& user) = 0;
    virtual void onTaskCompleted(const std::string& user) = 0;
};

// Concrete Observers
class QuestLog : public TaskObserver {
public:
    void onTaskAssigned(const std::string& user) override;
    void onTaskProgressUpdated(const std::string& user) override;
    void onTaskCompleted(const std::string& user) override;
};

class RewardSystem : public TaskObserver {
public:
    void onTaskAssigned(const std::string& user) override;
    void onTaskProgressUpdated(const std::string& user) override;
    void onTaskCompleted(const std::string& user) override;
};

class AchievementSystem : public TaskObserver {
public:
    void onTaskAssigned(const std::string& user) override;
    void onTaskProgressUpdated(const std::string& user) override;
    void onTaskCompleted(const std::string& user) override;
};

#endif // __TASK_OBSERVER_H__