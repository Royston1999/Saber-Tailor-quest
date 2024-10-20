#pragma once

#define SABER_TAILOR_EXPORT __attribute__((visibility("default")))
#ifdef __cplusplus
#define SABER_TAILOR_EXPORT_FUNC extern "C" SABER_TAILOR_EXPORT
#else
#define SABER_TAILOR_EXPORT_FUNC SABER_TAILOR_EXPORT
#endif

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "scotland2/shared/loader.hpp"
#include "SaberTailorConfig.hpp"
#include "UI/SettingsViewController.hpp"
#include "AprilFools.hpp"
#include "HMUI/TimeSlider.hpp"
#include "GlobalNamespace/UnityXRHelper.hpp"
#include "GlobalNamespace/IVRPlatformHelper.hpp"

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
};
extern AprilFools::RandomSaber randomSaber;
extern modloader::ModInfo modInfo;
// Define these functions here so that we can easily read configuration and log information from other files
Configuration& getConfig();
const Paper::ConstLoggerContext<12UL>& getLogger();
GlobalNamespace::IVRPlatformHelper* getPlatformHelper();
