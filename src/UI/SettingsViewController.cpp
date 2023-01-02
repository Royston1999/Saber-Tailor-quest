#include "UI/SettingsViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"

#include "UI/Tabs/MainSettings.hpp"
#include "UI/Tabs/ControllerSettingsHelper.hpp"
#include "UI/Tabs/ProfileSettings.hpp"
#include "UI/Tabs/ScaleSettings.hpp"
#include "UI/Tabs/TrailSettings.hpp"
#include "UI/Tabs/ExperimentalSettings.hpp"

using namespace SaberTailor::Tabs;
using namespace custom_types::Helpers;

DEFINE_TYPE(SaberTailor::Views, SettingsViewController);

void SaberTailor::Views::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(CoroutineHelper::New(CreateTabs()));
    }
}

Coroutine SaberTailor::Views::SettingsViewController::CreateTabs(){
    ArrayW<StringW> tabNames(std::initializer_list<StringW>{"Main Settings", "Helper", "Scale", "Trail", "Profiles", "Experimental"});
    QuestUI::BeatSaberUI::CreateTextSegmentedControl(get_transform(), {75, 7}, tabNames, [&](int index){
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