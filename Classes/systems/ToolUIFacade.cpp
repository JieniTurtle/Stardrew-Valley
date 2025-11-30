#include "ToolUIFacade.h"

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "GameScene.h"
#include "global.h"
#include "ToolFactory.h"

#include "ui/CocosGUI.h"

USING_NS_CC;

namespace {

struct CheckboxBinding {
    ui::CheckBox* checkbox = nullptr;
    std::function<void()> onSelect;
    std::function<void()> onDeselect;
};

struct UnlockConfig {
    std::function<void(bool)> toggleEnabled;
    int requiredExperience = 0;
    bool* notificationFlag = nullptr;
    std::string message;
    std::string scheduleKey;
};

std::vector<CheckboxBinding> buildBindings(GameScene* scene) {
    std::vector<CheckboxBinding> bindings;
    if (!scene) {
        return bindings;
    }

    auto addBinding = [&bindings](ui::CheckBox* checkbox, std::function<void()> onSelect, std::function<void()> onDeselect) {
        if (!checkbox) {
            return;
        }
        bindings.push_back({ checkbox, std::move(onSelect), std::move(onDeselect) });
    };

    // 使用工厂模式统一接口访问工具
    addBinding(
        scene->getTool(ToolType::TOOLS) ? scene->getTool(ToolType::TOOLS)->checkbox : nullptr,
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::TOOLS);
            if (tool) {
                tool->isActive = 1;  // 使用统一的基类接口
            }
        },
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::TOOLS);
            if (tool) {
                tool->isActive = 0;  // 使用统一的基类接口
            }
        });

    addBinding(
        scene->getTool(ToolType::PICKAXE) ? scene->getTool(ToolType::PICKAXE)->checkbox : nullptr,
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::PICKAXE);
            if (tool) {
                tool->isActive = 1;
            }
        },
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::PICKAXE);
            if (tool) {
                tool->isActive = 0;
            }
        });

    addBinding(
        scene->fishing ? scene->fishing->fishingcheckbox : nullptr,
        [scene]() {
            if (scene->fishing) scene->fishing->isfishing = 1;
        },
        [scene]() {
            if (scene->fishing) {
                scene->fishing->isfishing = 0;
                scene->fishing->fishingcondition = 0;
            }
        });

    addBinding(
        scene->seeds ? scene->seeds->seedscheckbox : nullptr,
        [scene]() {
            if (scene->seeds) scene->seeds->isseeds = 1;
        },
        [scene]() {
            if (scene->seeds) scene->seeds->isseeds = 0;
        });

    addBinding(
        scene->seedstwo ? scene->seedstwo->seedstwocheckbox : nullptr,
        [scene]() {
            if (scene->seedstwo) scene->seedstwo->isseedstwo = 1;
        },
        [scene]() {
            if (scene->seedstwo) scene->seedstwo->isseedstwo = 0;
        });

    addBinding(
        scene->getTool(ToolType::GLOVES) ? scene->getTool(ToolType::GLOVES)->checkbox : nullptr,
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::GLOVES);
            if (tool) {
                tool->isActive = 1;
            }
        },
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::GLOVES);
            if (tool) {
                tool->isActive = 0;
            }
        });

    addBinding(
        scene->wheat ? scene->wheat->wheatcheckbox : nullptr,
        [scene]() {
            if (scene->wheat) scene->wheat->iswheat = 1;
        },
        [scene]() {
            if (scene->wheat) scene->wheat->iswheat = 0;
        });

    addBinding(
        scene->carrot ? scene->carrot->carrotcheckbox : nullptr,
        [scene]() {
            if (scene->carrot) scene->carrot->iscarrot = 1;
        },
        [scene]() {
            if (scene->carrot) scene->carrot->iscarrot = 0;
        });

    addBinding(
        scene->getTool(ToolType::AXE) ? scene->getTool(ToolType::AXE)->checkbox : nullptr,
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::AXE);
            if (tool) {
                tool->isActive = 1;
            }
        },
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::AXE);
            if (tool) {
                tool->isActive = 0;
            }
        });

    addBinding(
        scene->wood ? scene->wood->woodcheckbox : nullptr,
        [scene]() {
            if (scene->wood) scene->wood->iswood = 1;
        },
        [scene]() {
            if (scene->wood) scene->wood->iswood = 0;
        });

    addBinding(
        scene->getTool(ToolType::KETTLE) ? scene->getTool(ToolType::KETTLE)->checkbox : nullptr,
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::KETTLE);
            if (tool) {
                tool->isActive = 1;
            }
        },
        [scene]() {
            ToolBase* tool = scene->getTool(ToolType::KETTLE);
            if (tool) {
                tool->isActive = 0;
            }
        });

    addBinding(
        scene->fertilizer ? scene->fertilizer->fertilizercheckbox : nullptr,
        [scene]() {
            if (scene->fertilizer) scene->fertilizer->isfertilizer = 1;
        },
        [scene]() {
            if (scene->fertilizer) scene->fertilizer->isfertilizer = 0;
        });

    return bindings;
}

void applyExclusiveSelection(const std::shared_ptr<std::vector<CheckboxBinding>>& bindings, ui::CheckBox* selected) {
    if (!bindings || !selected) {
        return;
    }
    for (auto& binding : *bindings) {
        if (!binding.checkbox || binding.checkbox == selected) {
            continue;
        }
        if (binding.checkbox->isSelected()) {
            binding.checkbox->setSelected(false);
            if (binding.onDeselect) {
                binding.onDeselect();
            }
        }
    }
}

void setupCheckboxes(GameScene* scene) {
    auto bindings = std::make_shared<std::vector<CheckboxBinding>>(buildBindings(scene));
    for (size_t i = 0; bindings && i < bindings->size(); ++i) {
        auto checkbox = (*bindings)[i].checkbox;
        if (!checkbox) {
            continue;
        }
        checkbox->addEventListener([scene, bindings, i](Ref*, ui::CheckBox::EventType type) {
            if (!scene || !bindings || i >= bindings->size()) {
                return;
            }
            auto& binding = (*bindings)[i];
            if (type == ui::CheckBox::EventType::SELECTED) {
                if (binding.onSelect) {
                    binding.onSelect();
                }
                applyExclusiveSelection(bindings, binding.checkbox);
            }
            else if (type == ui::CheckBox::EventType::UNSELECTED) {
                if (binding.onDeselect) {
                    binding.onDeselect();
                }
            }
        });
    }
}

void showUnlockNotification(GameScene* scene, const std::string& message) {
    if (!scene || message.empty()) {
        return;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithSystemFont(message, "Arial", 24);
    auto sprite = Sprite::create("buysellfailscene.png");
    if (!label || !sprite) {
        return;
    }

    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    label->setColor(Color3B::BLUE);
    sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    sprite->setScale(1.2f);

    scene->addChild(label, 6);
    scene->addChild(sprite, 5);

    auto callback = [scene, label, sprite]() {
        scene->removeChild(label);
        scene->removeChild(sprite);
    };
    auto action = Sequence::create(DelayTime::create(ExpTime), CallFunc::create(callback), nullptr);
    scene->runAction(action);
}

void configureUnlock(GameScene* scene, const UnlockConfig& config) {
    if (!scene || !config.toggleEnabled || config.scheduleKey.empty()) {
        return;
    }

    config.toggleEnabled(false);

    scene->schedule([scene, config](float) {
        if (experience >= config.requiredExperience) {
            config.toggleEnabled(true);
            if (config.notificationFlag && !(*config.notificationFlag)) {
                showUnlockNotification(scene, config.message);
                *config.notificationFlag = true;
            }
            scene->unschedule(config.scheduleKey);
        }
    }, 0.2f, config.scheduleKey);
}

void setupUnlocks(GameScene* scene) {
    if (!scene) {
        return;
    }

    configureUnlock(scene, UnlockConfig{
        [scene](bool enabled) {
            if (scene->fishing && scene->fishing->fishingcheckbox) {
                scene->fishing->fishingcheckbox->setEnabled(enabled);
            }
        },
        NewFishingExp,
        &NewFishingListeningbool,
        "You have enough experience.You can fish now.",
        "update_key_fishing"
        });

    configureUnlock(scene, UnlockConfig{
        [scene](bool enabled) {
            ToolBase* pickaxe = scene->getTool(ToolType::PICKAXE);
            if (pickaxe && pickaxe->checkbox) {
                pickaxe->checkbox->setEnabled(enabled);
            }
        },
        NewPickaxeExp,
        &NewPickaxeListeningbool,
        "You have enough experience.You can mine now.",
        "update_key_pickaxe"
        });

    configureUnlock(scene, UnlockConfig{
        [scene](bool enabled) {
            if (scene->cookLayer && scene->cookLayer->listener) {
                scene->cookLayer->listener->setEnabled(enabled);
            }
        },
        NewCookLayerExp,
        &NewCookLayerListeningbool,
        "You have enough experience.You can cook now.",
        "update_key_cook"
        });
}

} // namespace

void ToolUIFacade::setup(GameScene* scene) {
    setupCheckboxes(scene);
    setupUnlocks(scene);
}


