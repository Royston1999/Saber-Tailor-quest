#include "SettingsViewController.hpp"
#include "SettingsFlowCoordinator.hpp"
#include "SaberTailorLeftHand.hpp"
#include "SaberTailorRightHand.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "AxisDisplay.hpp"
#include "SaberTailorHelper.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/RectOffset.hpp"

DEFINE_TYPE(SaberTailor::Views, SettingsViewController);

void SaberTailor::Views::SettingsViewController::DidActivate(
    bool firstActivation,
    bool addedToHierarchy,
    bool screenSystemEnabling
) {
    using namespace GlobalNamespace;
    using namespace UnityEngine;
    using namespace QuestUI::BeatSaberUI;
    using namespace UnityEngine::UI;

    if (firstActivation) {
        VerticalLayoutGroup* mainlayout = CreateVerticalLayoutGroup(get_transform());
        mainlayout->set_spacing(1.5f);

        VerticalLayoutGroup* sabertailorgroup = CreateVerticalLayoutGroup(mainlayout->get_transform());
        sabertailorgroup->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::newcsstr("round-rect-panel"));
        sabertailorgroup->set_padding(UnityEngine::RectOffset::New_ctor(5, 5, 2, 2));
        
        VerticalLayoutGroup* controllersettingshelpergroup = CreateVerticalLayoutGroup(mainlayout->get_transform());
        controllersettingshelpergroup->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::newcsstr("round-rect-panel"));
        controllersettingshelpergroup->set_padding(UnityEngine::RectOffset::New_ctor(5, 5, 2, 2));
        
        TMPro::TextMeshProUGUI* sabertailortext = CreateText(sabertailorgroup->get_transform(), "<size=120%>Saber Tailor</size>");
        sabertailortext->set_alignment(TMPro::TextAlignmentOptions::Center);
        AddHoverHint(CreateToggle(sabertailorgroup->get_transform(), "Enable Saber Tailor", SaberTailorMain::config.isEnabled, 
        [](bool value) {
            setBool(getConfig().config, "isEnabled", value, false); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        } )->get_gameObject(), "Turn on Saber Tailor Override");
        leftButton = CreateUIViewControllerButton(sabertailorgroup->get_transform(), "Left Hand Settings", QuestUI::BeatSaberUI::CreateViewController<SaberTailor::Views::SaberTailorLeftHand*>());
        rightButton = CreateUIViewControllerButton(sabertailorgroup->get_transform(), "Right Hand Settings", QuestUI::BeatSaberUI::CreateViewController<SaberTailor::Views::SaberTailorRightHand*>());
        HorizontalLayoutGroup* somebuttons = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(sabertailorgroup->get_transform());
        CreateUIButton(somebuttons->get_transform(), "Import Base Game to Left", [](){
            TransferHelper::importFromBaseGame(0);
        });
        CreateUIButton(somebuttons->get_transform(), "Import Base Game to Right", [](){
            TransferHelper::importFromBaseGame(1);
        });
        
        TMPro::TextMeshProUGUI* thing = CreateText(controllersettingshelpergroup->get_transform(), "<size=120%>Controller Settings Helper</size>");
        thing->set_alignment(TMPro::TextAlignmentOptions::Center);
        AddHoverHint(CreateToggle(controllersettingshelpergroup->get_transform(), "Enable Axis Arrows in Menu", SaberTailorMain::config.spawnAxisDisplay, 
        [](bool value) {
            setBool(getConfig().config, "axisEnabled", value, false); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
            if(SaberTailorMain::config.spawnAxisDisplay){
                Array<GlobalNamespace::VRController*>* controllers = (UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>());
                for (int i = 0; i<controllers->get_Length(); i++){
                    if ((*controllers)[i]->get_node() == UnityEngine::XR::XRNode::LeftHand)(*controllers)[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
                    if ((*controllers)[i]->get_node() == UnityEngine::XR::XRNode::RightHand)(*controllers)[i]->get_gameObject()->AddComponent<ControllerSettingsHelper::AxisDisplay*>();
                }       
            }
        })->get_gameObject(), "imagine quest getting PC features");
        AddHoverHint(CreateToggle(controllersettingshelpergroup->get_transform(), "Custom Controller Settings Method", SaberTailorMain::config.overrideSettingsMethod, 
        [](bool value) {
            setBool(getConfig().config, "overrideSettingsMethod", value, false); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        } )->get_gameObject(), "does things n stuff");
        AddHoverHint(CreateToggle(controllersettingshelpergroup->get_transform(), "Mirror Z Rotations for Left", SaberTailorMain::config.mirrorZRot, 
        [](bool value) {
            setBool(getConfig().config, "mirrorZRot", value, false); getConfig().Write();
            ConfigHelper::LoadConfig(SaberTailorMain::config, getConfig().config);
        } )->get_gameObject(), "Fixes left hand Z Rotation when using base game settings. INEFFECTIVE WHEN SABER TAILOR IS ENABLED");
    }
}

UnityEngine::UI::Button* SaberTailor::Views::SettingsViewController::CreateUIViewControllerButton(UnityEngine::Transform* parent, std::string title, HMUI::ViewController* viewController) {
    using namespace UnityEngine;
    return QuestUI::BeatSaberUI::CreateUIButton(parent, title, Vector2(), Vector2(30, 5),
        [this, title, viewController]() {
            flowCoordinator->SetTitle(il2cpp_utils::newcsstr(title), ViewController::AnimationType::In);
            flowCoordinator->ReplaceTopViewController(viewController, flowCoordinator, flowCoordinator, nullptr, ViewController::AnimationType::In, ViewController::AnimationDirection::Horizontal);

            reinterpret_cast<SaberTailor::SettingsFlowCoordinator*>(flowCoordinator)->currentViewController = viewController;
        }
    );
}