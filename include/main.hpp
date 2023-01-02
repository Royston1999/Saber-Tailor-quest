#pragma once

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "modloader/shared/modloader.hpp"
#include "SaberTailorConfig.hpp"
#include "UI/SettingsViewController.hpp"
#include "UI/SaberTailorLeftHand.hpp"
#include "UI/SaberTailorRightHand.hpp"
#include "AprilFools.hpp"
#include "HMUI/TimeSlider.hpp"
#include "GlobalNamespace/FormattedFloatListSettingsValueController.hpp"
// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"


class SaberTailorMain {
    public:
        static SaberTailorConfig config;
        static bool configValid;
        static void loadConfig();
        static SaberTailor::Views::SettingsViewController* saberTailorMainSettingsPage;
        static SaberTailor::Views::SaberTailorLeftHand* saberTailorLeftHand;
        static SaberTailor::Views::SaberTailorRightHand* saberTailorRightHand;
};
extern AprilFools::RandomSaber randomSaber;
extern ModInfo modInfo;
// Define these functions here so that we can easily read configuration and log information from other files
Configuration& getConfig();
Logger& getLogger();