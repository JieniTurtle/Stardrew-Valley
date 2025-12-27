// Refactored with Decorator Pattern
#ifndef __TRADER_DECORATOR_H__
#define __TRADER_DECORATOR_H__

#include "NPCDecorator.h"
#include <map>
#include <string>

// 交易物品结构
struct TradeItem {
    std::string itemName;
    int buyPrice;   // 购买价格
    int sellPrice;  // 出售价格
    int stock;      // 库存
};

// TraderDecorator - 交易装饰器
class TraderDecorator : public NPCDecorator {
private:
    std::map<std::string, TradeItem> tradeItems_;  // 可交易物品列表
    
public:
    TraderDecorator(INPC* npc) : NPCDecorator(npc) {}
    
    virtual void interact() override {
        NPCDecorator::interact();
        // 显示交易界面
        showTradeMenu();
    }
    
    void addTradeItem(const TradeItem& item) {
        tradeItems_[item.itemName] = item;
    }
    
    bool buyItem(const std::string& itemName, int quantity) {
        auto it = tradeItems_.find(itemName);
        if (it != tradeItems_.end() && it->second.stock >= quantity) {
            // 检查玩家金钱（需要访问全局变量money）
            // 执行购买逻辑
            it->second.stock -= quantity;
            return true;
        }
        return false;
    }
    
    bool sellItem(const std::string& itemName, int quantity) {
        auto it = tradeItems_.find(itemName);
        if (it != tradeItems_.end()) {
            // 执行出售逻辑
            return true;
        }
        return false;
    }
    
    const std::map<std::string, TradeItem>& getTradeItems() const {
        return tradeItems_;
    }
    
private:
    void showTradeMenu() {
        // 显示交易UI的逻辑
        // 这里可以添加显示交易界面的代码
    }
};

#endif // __TRADER_DECORATOR_H__

