// Steam Launcher, standalone program.
//
// The launcher's code is shared with the Windhawk mod. These declarations
// stand in for the few Windhawk functions it uses: settings and values come
// from an INI file in %APPDATA%, storage is a folder in %LOCALAPPDATA%, and the
// log is a text file next to it. They are implemented in host.cpp.

#pragma once

#define STEAM_LAUNCHER_STANDALONE 1
#define LAUNCHER_WINDOW_CLASS L"SteamLauncherStandalone"
#define WH_MOD_ID L"steam-launcher"

#include <windows.h>

void Wh_Log_(const wchar_t* format, ...);
#define Wh_Log(...) Wh_Log_(__VA_ARGS__)

int Wh_GetIntSetting(PCWSTR valueName, ...);
PCWSTR Wh_GetStringSetting(PCWSTR valueName, ...);
void Wh_FreeStringSetting(PCWSTR string);
BOOL Wh_GetModStoragePath(PWSTR pathBuffer, size_t bufferChars);
size_t Wh_GetStringValue(PCWSTR valueName, PWSTR stringBuffer, size_t bufferChars);
BOOL Wh_SetStringValue(PCWSTR valueName, PCWSTR value);

namespace WindhawkUtils {
class StringSetting {
public:
    static StringSetting make(PCWSTR name) {
        StringSetting s;
        s.value_ = Wh_GetStringSetting(name);
        return s;
    }
    StringSetting() = default;
    StringSetting(StringSetting&& other) noexcept : value_(other.value_) { other.value_ = nullptr; }
    StringSetting(const StringSetting&) = delete;
    StringSetting& operator=(const StringSetting&) = delete;
    ~StringSetting() {
        if (value_) Wh_FreeStringSetting(value_);
    }
    PCWSTR get() const { return value_ ? value_ : L""; }

private:
    PCWSTR value_ = nullptr;
};
}  // namespace WindhawkUtils
