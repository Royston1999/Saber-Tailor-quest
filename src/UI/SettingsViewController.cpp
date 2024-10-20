#include "UI/SettingsViewController.hpp"

#include "bsml/shared/BSML-Lite.hpp"
// #include "GlobalNamespace/SharedCoroutineStarter.hpp"

#include "UI/Tabs/MainSettings.hpp"
#include "UI/Tabs/ControllerSettingsHelper.hpp"
#include "UI/Tabs/ProfileSettings.hpp"
#include "UI/Tabs/ScaleSettings.hpp"
#include "UI/Tabs/TrailSettings.hpp"
#include "UI/Tabs/ExperimentalSettings.hpp"
#include "UnityEngine/Resources.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "bsml/shared/Helpers/getters.hpp"
#include "Utils/EasyDelegate.hpp"
#include "main.hpp"

using namespace SaberTailor::Tabs;
using namespace custom_types::Helpers;

DEFINE_TYPE(SaberTailor::Views, SettingsViewController);

void SaberTailor::Views::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        StartCoroutine(CoroutineHelper::New(CreateTabs()));
    }
}

UnityEngine::GameObject* CreateTextSegmentedControlObject(UnityEngine::Transform* parent) {
    auto textSegmentedControlTemplate = UnityEngine::Resources::FindObjectsOfTypeAll<HMUI::TextSegmentedControl*>()->FirstOrDefault([](HMUI::TextSegmentedControl* x) {
        auto parent = x->get_transform()->get_parent();
        if (!parent) return false;
        return parent->get_name() == "HelpMenu";
    });
    auto gameObject = BSML::Helpers::GetDiContainer()->InstantiatePrefab(textSegmentedControlTemplate, parent);
    auto textSegmentedControl = gameObject->GetComponent<HMUI::TextSegmentedControl*>();
    textSegmentedControl->____dataSource = nullptr;

    auto transform = gameObject->get_transform().cast<UnityEngine::RectTransform>();
    transform->set_anchoredPosition({0, 0});
    int childCount = transform->get_childCount();
    for (int i = 1; i <= childCount; i++) {
        UnityEngine::Object::DestroyImmediate(transform->GetChild(childCount - i)->get_gameObject());
    }
    gameObject->SetActive(true);
    return gameObject;
}

HMUI::TextSegmentedControl* CreateSaberTailorTextSegmentedControl(UnityEngine::Transform* parent, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::span<std::string_view> values, std::function<void(int)> onCellWithIdxClicked) {
    auto go = CreateTextSegmentedControlObject(parent);
    auto textSegmentedControl = go->GetComponent<HMUI::TextSegmentedControl*>();
    go->set_name("SaberTailorTextSegmentedControl");
    auto rect = go->get_transform().cast<UnityEngine::RectTransform>();
    rect->set_anchoredPosition(anchoredPosition);
    rect->set_sizeDelta(sizeDelta);

    auto texts = ListW<StringW>::New();
    texts->EnsureCapacity(values.size());
    for (const auto& text : values) texts->Add(text);
    textSegmentedControl->SetTexts(*texts);

    textSegmentedControl->add_didSelectCellEvent(custom_types::MakeDelegate<System::Action_2<UnityW<HMUI::SegmentedControl>, int>*>(
        std::function<void(HMUI::SegmentedControl*, int)>([onCellWithIdxClicked](HMUI::SegmentedControl* _, int idx) { onCellWithIdxClicked(idx); })
    ));
    return textSegmentedControl;
}

static std::vector<std::string_view> tabNames = {"Main Settings", "Helper", "Scale", "Trail", "Profiles", "Experimental"};

Coroutine SaberTailor::Views::SettingsViewController::CreateTabs(){
    
    CreateSaberTailorTextSegmentedControl(get_transform(), {0, 0}, {-10, 7}, tabNames, [&](int index){
        tabs[selectedTab]->SetActive(false);
        tabs[index]->SetActive(true);
        selectedTab = index;
    });
    co_yield nullptr;
    tabs.push_back(MainSettings::CreateMainSettings(get_transform())); // tab 1
    co_yield nullptr;
    tabs.push_back(CSH::CreateCSHSettings(get_transform())); // tab 2
    co_yield nullptr;
    tabs.push_back(Scale::CreateScaleSettings(get_transform())); // tab 3
    co_yield nullptr;
    tabs.push_back(Trail::CreateTrailSettings(get_transform())); // tab 4
    co_yield nullptr;
    tabs.push_back(Profile::CreateProfileSettings(get_transform())); // tab 5
    co_yield nullptr;
    tabs.push_back(Experimental::CreateExperimentalSettings(get_transform())); // tab 6
    co_return;
}