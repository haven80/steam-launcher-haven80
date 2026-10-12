// ==WindhawkMod==
// @id              steam-launcher
// @name            Steam Launcher (experimental)
// @description     A minimal, animated launcher for your Steam library: WebGL covers or 3D game cases, search, playtime, studios, your year in games and Steam news, opened with a hotkey, a controller or from the tray
// @version         0.11
// @author          HaVeN80
// @github          https://github.com/haven80
// @include         windhawk.exe
// @compilerOptions -lole32 -lshell32 -lshlwapi -luuid -ladvapi32 -luser32 -lgdi32 -lwinhttp -ldwmapi
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Steam Launcher

> **Experimental.** This mod loads the WebView2 runtime directly (see "How it
> works") and is still being tested. If it doesn't open, the reason is in the
> mod's log. The same launcher is also available as a standalone program that
> starts with Windows and doesn't need Windhawk.

A minimal launcher for your installed **Steam** games. Your covers sit on a
WebGL strip that bends as you drag it; open a game to see its playtime, last
session, install date, size on disk and the latest news from Steam, then press
**Play**.

Open it with the hotkey (**Ctrl+Alt+G** by default) or by clicking its icon in
the notification area. Press **Esc** or click elsewhere to close it.

## Controls
* **Type** anywhere to search; the strip filters as you type.
* **Drag**, mouse wheel or **← →**: move through the library.
* **Click** the centre cover or **Enter**: open the game.
* **Esc**: back to the library, then close the launcher.
* Right-click the tray icon to refresh the library.
* **Controller** (Xbox, PlayStation and other standard pads): stick or d-pad to
  move, **A** to open, **B** to go back, **Y** for "what should I play
  tonight?", **X** to turn a case around, **Menu** to star a game, **View** for
  your year. Hold **View + Menu** to open or close the launcher from anywhere
  (ignored while a full-screen game runs).
* The sliders button (top right) chooses which games to show (installed, all,
  not installed), the showroom (after 30 seconds, 1 or 3 minutes without
  input the library scrolls by itself, one game every few seconds, until you
  move the mouse or press a key), the style (a flat strip of covers, or 3D game cases with
  the title on the spine and a reflection on the floor), the theme (system,
  light, dark) and the effects: curvature of the strip (or angle of the side
  cases), strength of the spherical lens, reflection, dust, glow and
  tilt, plus:
  * **Logo**: the game's own logo over its picture, and as the title.
  * **Holographic**: a rainbow foil on games played for 100+ hours.
  * **Depth of field**: the side covers slightly out of focus.
  * **Time of day**: the glow turns warmer in the evening, bluer at night.
  * **Back** (Cases style): the first click turns the case around to show its
    printed back with the game's numbers; the second click opens it.
  * **Disc** (Cases style): opening a game slides a spinning disc out of its
    case.

  And the features, each one can be switched off too:
  * **What to play?** (dice button): the strip spins like a slot machine and
    lands on a game, more often one that is installed and barely played.
  * **Favorites**: a star in a game's page; starred games come first and have a
    filter of their own, next to your **Steam collections**.
  * **Studios**: next to Recent, Most played and A–Z, the library grouped by
    developer or publisher, each group opened by a divider card like in a
    record shop. The back of each case and the game's page say who developed
    and published it; the names lead to their group. Search also finds studios.
  * **Your year** (chart button): hours per month, days played, streaks, the
    games you played most and the one you picked up again after months.
  * **Trailers**: after a moment on a game's page its short silent Steam
    trailer fades in over the picture.
  * **Parallax**: the picture and the logo drift apart with the mouse.
  * **Sounds** (off by default): a tick as games pass, a rustle when a case
    opens, the click of the disc. Made on the spot, no sound files.
  * **PC ROM logo** (Cases style): a band printed across the top of each case.
    An invented mark, not a real brand.

  Moving effects stop when Windows is set to reduce animations.

## What it shows
Everything is read from the files Steam keeps on disk, so no account or API key
is needed:

* installed games and size on disk from `steamapps\appmanifest_*.acf`
* playtime and last played date from `userdata\<id>\config\localconfig.vdf`
* covers and hero images from Steam's library cache
* save and screenshot folders when they exist
* developers and publishers from `appcache\appinfo.vdf`, and your collections
  (with a fixed list of games) from `userdata\<id>\config\cloudstorage`
* games you own but haven't installed from Steam's local cache (`appinfo.vdf`
  and library artwork): the ones you opened at least once in the Steam library
  show up, games in your "Hidden" collection don't

Covers missing from Steam's cache are downloaded from Steam's store servers
(you can turn this off). When you open a game, its **Steam user reviews** and
**Metacritic** score come from Steam's public store APIs, and its **time to
beat** (rushed, normal, 100%) from **IGDB**, once you add a free IGDB client ID
and secret in the settings. All three are kept for 3 days and can be turned
off. The news comes from Steam's public news API (`ISteamNews/GetNewsForApp`), only
when you open a game. Turn it off in the settings to make the launcher work
fully offline.

**Your year** needs history Steam doesn't keep: the launcher writes each
game's total playtime down whenever it grows (`playtime.tsv` in the mod's
storage folder), so the numbers start from the day you install it.

The interface follows the Windows display language (Italian or English) and the
Windows light/dark theme, unless you choose a theme in the settings.

## How it works
The mod runs in its own Windhawk process and never touches Explorer, Steam or
the game folders. It opens a borderless window with a **WebView2** view (the Edge
engine that ships with Windows 10 and 11) and loads the interface from the mod's
storage folder through a private host name (`steam-launcher.example`, which
never reaches the network). Covers are hard-linked there from Steam's cache, or
copied when Steam is on another drive.

The interface is self-contained: its WebGL renderer and animations are part of
the mod, and it loads nothing from the internet. The only network requests are
the optional ones described above, all to Steam: the news, reviews and
Metacritic score of a game when you open it, covers missing from Steam's cache,
and, if you stay on a game's page, the address of its micro-trailer from
Steam's store API and the video itself from Steam's video servers (Trailers
can be switched off in the launcher's settings).

WebView2 is loaded from the installed WebView2 Runtime (preinstalled on Windows
11 and on up-to-date Windows 10). Windhawk mods can't ship `WebView2Loader.dll`,
so the mod calls the runtime's environment entry point directly, the same way
open-source WebView2 loaders do. If the runtime is missing or changes, the
launcher doesn't open and the reason is written to the mod's log.

## Time to beat
HowLongToBeat has no public API, so the launcher uses IGDB's time-to-beat data,
which comes from players' own reports. To turn it on:
1. Sign in at [dev.twitch.tv/console](https://dev.twitch.tv/console) (any
   Twitch account; two-factor authentication must be on).
2. Register an application: any name, OAuth redirect `http://localhost`,
   category "Application Integration", client type "Confidential".
3. Open it, copy the **Client ID**, create a **Client secret**, and paste both
   in the mod settings.

## Credits
Titles use **Bricolage Grotesque** by Mathieu Triay, embedded in the mod under
the SIL Open Font License 1.1. Other text uses the Windows fonts Segoe UI
Variable and Cascadia Mono.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- hotkey: Ctrl+Alt+G
  $name: Hotkey
  $description: For example Ctrl+Alt+G, Alt+Shift+S or Ctrl+F12. Leave empty to disable.
- trayIcon: true
  $name: Icon in the notification area
- windowSize: full
  $name: Window size
  $options:
  - full: Whole screen (work area)
  - large: Large window, centred
- hideOnFocusLoss: true
  $name: Close when clicking outside the launcher
- hideAfterLaunch: true
  $name: Close after launching a game
- theme: auto
  $name: Theme
  $options:
  - auto: Same as Windows
  - dark: Dark
  - light: Light
- downloadArt: true
  $name: Download missing covers from Steam
  $description: For games whose artwork isn't in Steam's local cache (often games you haven't opened in the library yet)
- showRatings: true
  $name: Show ratings and time to beat
  $description: Steam user reviews and Metacritic score from Steam's store, and time to beat from IGDB when set up below. Fetched when you open a game, kept for 3 days
- igdbClientId: ""
  $name: IGDB client ID (optional, for time to beat)
  $description: Free. Create an application at dev.twitch.tv/console (category "Application Integration", OAuth redirect http://localhost) and paste its client ID here
- igdbClientSecret: ""
  $name: IGDB client secret (optional, for time to beat)
  $description: The client secret of the same Twitch application. It is stored in Windhawk's settings on this PC and only sent to Twitch to get an IGDB access token
- gamepadOpen: true
  $name: Open with a controller
  $description: Hold View and Menu together on an Xbox-style controller to open or close the launcher. Ignored while a full-screen game is running
- showNews: true
  $name: Show Steam news
  $description: Downloads the latest news of a game from Steam when you open it
- excludedAppIds: "228980"
  $name: Steam AppIDs to exclude
  $description: Comma separated. 228980 = Steamworks Common Redistributables
*/
// ==/WindhawkModSettings==

// @@CORE-BEGIN  (shared with the standalone program; see build.py)

#include <windows.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <winhttp.h>
#include <xinput.h>
#ifndef STEAM_LAUNCHER_STANDALONE
#include <windhawk_utils.h>
#endif

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cwctype>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#ifndef WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY
#define WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY 4
#endif
#ifndef RRF_SUBKEY_WOW6464KEY
#define RRF_SUBKEY_WOW6464KEY 0x00010000
#endif
#ifndef RRF_SUBKEY_WOW6432KEY
#define RRF_SUBKEY_WOW6432KEY 0x00020000
#endif
#ifndef DWMWA_WINDOW_CORNER_PREFERENCE
#define DWMWA_WINDOW_CORNER_PREFERENCE 33
#endif
#ifndef LAUNCHER_WINDOW_CLASS
#define LAUNCHER_WINDOW_CLASS L"WindhawkSteamLauncher"
#endif

// What differs between the Windhawk mod and the standalone program. Each one
// implements these after the shared code.
bool HostCanEditSettings();                    // settings live in the launcher itself
void HostSetSetting(const std::wstring& key, const std::wstring& value);
std::string HostSettingsJson();                // current settings for the launcher, or ""
bool HostHasQuit();                            // "Exit" in the tray menu
void HostQuit();
HICON HostTrayIcon();                          // or nullptr for Steam's icon
void HostReportError(const wchar_t* message);  // something the user must know

namespace {

// ============================================================ the page

// The interface: one HTML file, written to the mod's storage folder and
// served to WebView2 from a virtual host name.
const char kIndexHtml[] = R"__wh_html__(@@HTML@@)__wh_html__";

const wchar_t kHostName[] = L"steam-launcher.example";

// The page URL carries a hash of its content, so WebView2 never shows a cached
// copy of an older version after the mod is updated.
std::wstring StartUrl() {
    unsigned long long h = 1469598103934665603ULL;
    for (const char* p = kIndexHtml; *p; p++) {
        h ^= (unsigned char)*p;
        h *= 1099511628211ULL;
    }
    wchar_t buf[96];
    swprintf(buf, 96, L"https://steam-launcher.example/index.html?v=%016llx", h);
    return buf;
}

}  // namespace

// ============================================================ WebView2
//
// WebView2.h is not part of the Windhawk toolchain, so the few interfaces
// the mod uses are declared here, in vtable order (WebView2 SDK 1.0.2903).
// Methods the mod never calls are kept as placeholders.
//
// They live outside the anonymous namespace on purpose: for types local to
// this file, GCC assumes nothing outside implements them and optimizes the
// calls into the WebView2 runtime away (they crash at a bogus address).

#define WV_PAD_CAT2(a, b) a##b
#define WV_PAD_CAT(a, b) WV_PAD_CAT2(a, b)
#define WV_PAD virtual HRESULT STDMETHODCALLTYPE WV_PAD_CAT(unused_, __COUNTER__)() = 0;

struct WvToken {
    INT64 value;
};
struct WvColor {
    BYTE A, R, G, B;
};

struct ICoreWebView2;
struct ICoreWebView2Controller;
struct ICoreWebView2Environment;
struct ICoreWebView2WebMessageReceivedEventArgs;

struct ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler : IUnknown {
    virtual HRESULT STDMETHODCALLTYPE Invoke(HRESULT errorCode,
                                             ICoreWebView2Environment* result) = 0;
};
struct ICoreWebView2CreateCoreWebView2ControllerCompletedHandler : IUnknown {
    virtual HRESULT STDMETHODCALLTYPE Invoke(HRESULT errorCode,
                                             ICoreWebView2Controller* result) = 0;
};
struct ICoreWebView2WebMessageReceivedEventHandler : IUnknown {
    virtual HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender,
                                             ICoreWebView2WebMessageReceivedEventArgs* args) = 0;
};

struct ICoreWebView2Environment : IUnknown {
    virtual HRESULT STDMETHODCALLTYPE CreateCoreWebView2Controller(
        HWND parentWindow, ICoreWebView2CreateCoreWebView2ControllerCompletedHandler* handler) = 0;
};

struct ICoreWebView2Settings : IUnknown {
    WV_PAD  // get_IsScriptEnabled
    WV_PAD  // put_IsScriptEnabled
    WV_PAD  // get_IsWebMessageEnabled
    WV_PAD  // put_IsWebMessageEnabled
    WV_PAD  // get_AreDefaultScriptDialogsEnabled
    virtual HRESULT STDMETHODCALLTYPE put_AreDefaultScriptDialogsEnabled(BOOL value) = 0;
    WV_PAD  // get_IsStatusBarEnabled
    virtual HRESULT STDMETHODCALLTYPE put_IsStatusBarEnabled(BOOL value) = 0;
    WV_PAD  // get_AreDevToolsEnabled
    virtual HRESULT STDMETHODCALLTYPE put_AreDevToolsEnabled(BOOL value) = 0;
    WV_PAD  // get_AreDefaultContextMenusEnabled
    virtual HRESULT STDMETHODCALLTYPE put_AreDefaultContextMenusEnabled(BOOL value) = 0;
    WV_PAD  // get_AreHostObjectsAllowed
    WV_PAD  // put_AreHostObjectsAllowed
    WV_PAD  // get_IsZoomControlEnabled
    virtual HRESULT STDMETHODCALLTYPE put_IsZoomControlEnabled(BOOL value) = 0;
};

struct ICoreWebView2WebMessageReceivedEventArgs : IUnknown {
    WV_PAD  // get_Source
    virtual HRESULT STDMETHODCALLTYPE get_WebMessageAsJson(LPWSTR* value) = 0;
};

struct ICoreWebView2 : IUnknown {
    virtual HRESULT STDMETHODCALLTYPE get_Settings(ICoreWebView2Settings** settings) = 0;  // 1
    WV_PAD                                                                                  // 2 get_Source
    virtual HRESULT STDMETHODCALLTYPE Navigate(LPCWSTR uri) = 0;                            // 3
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                   // 4-13
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                   // 14-23
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                                                      // 24-28
    virtual HRESULT STDMETHODCALLTYPE Reload() = 0;                                         // 29
    virtual HRESULT STDMETHODCALLTYPE PostWebMessageAsJson(LPCWSTR json) = 0;               // 30
    WV_PAD                                                                                  // 31
    virtual HRESULT STDMETHODCALLTYPE add_WebMessageReceived(                               // 32
        ICoreWebView2WebMessageReceivedEventHandler* handler, WvToken* token) = 0;
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                   // 33-42
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                   // 43-52
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                                               // 53-58
};

struct ICoreWebView2_2 : ICoreWebView2 {
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD  // 7 methods
};

struct ICoreWebView2_3 : ICoreWebView2_2 {
    WV_PAD  // TrySuspend
    WV_PAD  // Resume
    WV_PAD  // get_IsSuspended
    virtual HRESULT STDMETHODCALLTYPE SetVirtualHostNameToFolderMapping(LPCWSTR hostName,
                                                                        LPCWSTR folderPath,
                                                                        int accessKind) = 0;
};

struct ICoreWebView2Controller : IUnknown {
    WV_PAD                                                                       // 1 get_IsVisible
    virtual HRESULT STDMETHODCALLTYPE put_IsVisible(BOOL isVisible) = 0;         // 2
    WV_PAD                                                                       // 3 get_Bounds
    virtual HRESULT STDMETHODCALLTYPE put_Bounds(RECT bounds) = 0;               // 4
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                                           // 5-9
    virtual HRESULT STDMETHODCALLTYPE MoveFocus(int reason) = 0;                 // 10
    WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD WV_PAD                      // 11-18
    WV_PAD WV_PAD                                                                // 19-20
    virtual HRESULT STDMETHODCALLTYPE NotifyParentWindowPositionChanged() = 0;   // 21
    virtual HRESULT STDMETHODCALLTYPE Close() = 0;                               // 22
    virtual HRESULT STDMETHODCALLTYPE get_CoreWebView2(ICoreWebView2** wv) = 0;  // 23
};

struct ICoreWebView2Controller2 : ICoreWebView2Controller {
    WV_PAD  // get_DefaultBackgroundColor
    virtual HRESULT STDMETHODCALLTYPE put_DefaultBackgroundColor(WvColor value) = 0;
};

const IID kIidController2 = {0xc979903e, 0xd4ca, 0x4228, {0x92, 0xeb, 0x47, 0xee, 0x3f, 0xa9, 0x6e, 0xab}};
const IID kIidWebView2_3 = {0xa0d6df20, 0x3b92, 0x416d, {0xaa, 0x0c, 0x43, 0x7a, 0x9c, 0x72, 0x78, 0x57}};
const IID kIidEnvCompleted = {0x4e8a3389, 0xc9d8, 0x4bd2, {0xb6, 0xb5, 0x12, 0x4f, 0xee, 0x6c, 0xc1, 0x4d}};
const IID kIidControllerCompleted = {0x6c4819f3, 0xc9b7, 0x4260, {0x81, 0x27, 0xc9, 0xf5, 0xbd, 0xe7, 0xf6, 0x8c}};
const IID kIidMessageReceived = {0x57213f19, 0x00e6, 0x49fa, {0x8e, 0x07, 0x89, 0x8e, 0xa0, 0x1e, 0xcb, 0xd2}};

// COM callback object wrapping a lambda.
template <class I, class... Args>
class Callback : public I {
public:
    Callback(const IID& iid, std::function<HRESULT(Args...)> fn) : iid_(iid), fn_(std::move(fn)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (!ppv) return E_POINTER;
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, iid_)) {
            *ppv = static_cast<I*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return (ULONG)InterlockedIncrement(&ref_); }
    ULONG STDMETHODCALLTYPE Release() override {
        LONG r = InterlockedDecrement(&ref_);
        if (r == 0) delete this;
        return (ULONG)r;
    }
    HRESULT STDMETHODCALLTYPE Invoke(Args... args) override { return fn_(args...); }

private:
    virtual ~Callback() = default;
    IID iid_;
    std::function<HRESULT(Args...)> fn_;
    LONG ref_ = 1;
};

namespace {

using CreateEnvInternal_t = HRESULT(STDMETHODCALLTYPE*)(
    bool checkRunningInstance, int runtimeType, PCWSTR userDataFolder, IUnknown* options,
    ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler* handler);

#if defined(__aarch64__) || defined(_M_ARM64)
const wchar_t kClientDll[] = L"\\EBWebView\\arm64\\EmbeddedBrowserWebView.dll";
#elif defined(__x86_64__) || defined(_M_X64)
const wchar_t kClientDll[] = L"\\EBWebView\\x64\\EmbeddedBrowserWebView.dll";
#else
const wchar_t kClientDll[] = L"\\EBWebView\\x86\\EmbeddedBrowserWebView.dll";
#endif

// ============================================================ settings

enum class WindowSize { Full, Large };
enum class Theme { Auto, Dark, Light };

Theme ParseTheme(const std::wstring& t) {
    return t == L"dark" ? Theme::Dark : t == L"light" ? Theme::Light : Theme::Auto;
}

struct Settings {
    std::wstring hotkey;
    bool trayIcon = true;
    WindowSize size = WindowSize::Full;
    bool hideOnFocusLoss = true;
    bool hideAfterLaunch = true;
    Theme theme = Theme::Auto;
    bool showNews = true;
    bool showRatings = true;
    std::wstring igdbClientId, igdbClientSecret;
    bool downloadArt = true;
    bool gamepadOpen = true;
    std::wstring excluded;
};

Settings g_settings;
SRWLOCK g_settingsLock = SRWLOCK_INIT;

Settings GetSettings() {
    AcquireSRWLockShared(&g_settingsLock);
    Settings s = g_settings;
    ReleaseSRWLockShared(&g_settingsLock);
    return s;
}

std::wstring StringSetting(PCWSTR name) {
    return WindhawkUtils::StringSetting::make(name).get();
}

void LoadSettings() {
    Settings s;
    s.hotkey = StringSetting(L"hotkey");
    s.trayIcon = Wh_GetIntSetting(L"trayIcon") != 0;
    s.size = StringSetting(L"windowSize") == L"large" ? WindowSize::Large : WindowSize::Full;
    s.hideOnFocusLoss = Wh_GetIntSetting(L"hideOnFocusLoss") != 0;
    s.hideAfterLaunch = Wh_GetIntSetting(L"hideAfterLaunch") != 0;
    std::wstring theme = StringSetting(L"theme");
    s.theme = ParseTheme(theme);
    s.showNews = Wh_GetIntSetting(L"showNews") != 0;
    s.showRatings = Wh_GetIntSetting(L"showRatings") != 0;
    s.igdbClientId = StringSetting(L"igdbClientId");
    s.igdbClientSecret = StringSetting(L"igdbClientSecret");
    for (auto* v : {&s.igdbClientId, &s.igdbClientSecret}) {
        while (!v->empty() && iswspace(v->back())) v->pop_back();
        while (!v->empty() && iswspace(v->front())) v->erase(v->begin());
    }
    s.downloadArt = Wh_GetIntSetting(L"downloadArt") != 0;
    s.gamepadOpen = Wh_GetIntSetting(L"gamepadOpen") != 0;
    s.excluded = StringSetting(L"excludedAppIds");
    AcquireSRWLockExclusive(&g_settingsLock);
    g_settings = std::move(s);
    ReleaseSRWLockExclusive(&g_settingsLock);
}

// ============================================================ utilities

HANDLE g_stopEvent = nullptr;
HANDLE g_resyncEvent = nullptr;
volatile LONG g_steamFound = 1;
std::wstring g_storage, g_uiDir;  // mod storage, and the page's folder in it

bool Stopping() {
    return WaitForSingleObject(g_stopEvent, 0) == WAIT_OBJECT_0;
}

std::wstring Lower(std::wstring s) {
    if (!s.empty()) CharLowerBuffW(&s[0], (DWORD)s.size());
    return s;
}

bool IsNumeric(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

std::wstring Utf8ToWide(const std::string& s) {
    if (s.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), &w[0], n);
    return w;
}

std::string WideToUtf8(const std::wstring& w) {
    if (w.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), &s[0], n, nullptr, nullptr);
    return s;
}

std::wstring NormalizePath(std::wstring p) {
    for (auto& c : p) {
        if (c == L'/') c = L'\\';
    }
    while (p.size() > 3 && p.back() == L'\\') p.pop_back();
    return p;
}

bool IsAbsolute(const std::wstring& p) {
    return p.size() > 2 && (p[1] == L':' || (p[0] == L'\\' && p[1] == L'\\'));
}

bool DirExists(const std::wstring& p) {
    DWORD a = GetFileAttributesW(p.c_str());
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileExists(const std::wstring& p) {
    DWORD a = GetFileAttributesW(p.c_str());
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

ULONGLONG FileTimeValue(const FILETIME& ft) {
    return ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
}

long long FileTimeToUnix(const FILETIME& ft) {
    ULONGLONG v = FileTimeValue(ft);
    return v ? (long long)(v / 10000000ULL) - 11644473600LL : 0;
}

ULONGLONG LastWriteTime(const std::wstring& path) {
    WIN32_FILE_ATTRIBUTE_DATA a;
    return GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &a)
               ? FileTimeValue(a.ftLastWriteTime)
               : 0;
}

bool ReadFileBytes(const std::wstring& path, std::string& out) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_READ,
                           FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
                           OPEN_EXISTING, 0, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    LARGE_INTEGER size;
    if (!GetFileSizeEx(h, &size) || size.QuadPart > 64 * 1024 * 1024) {
        CloseHandle(h);
        return false;
    }
    out.resize((size_t)size.QuadPart);
    DWORD read = 0;
    BOOL ok = out.empty() || ReadFile(h, &out[0], (DWORD)out.size(), &read, nullptr);
    CloseHandle(h);
    if (!ok) return false;
    out.resize(read);
    return true;
}

bool WriteFileBytes(const std::wstring& path, const std::string& data) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD written = 0;
    BOOL ok = data.empty() || WriteFile(h, data.data(), (DWORD)data.size(), &written, nullptr);
    CloseHandle(h);
    return ok && written == data.size();
}

std::wstring RegReadStr(HKEY root, const std::wstring& key, const wchar_t* name,
                        DWORD flags = 0) {
    // REG_SZ first; REG_EXPAND_SZ read unexpanded and expanded here (asking
    // for both types at once is refused by some implementations).
    wchar_t buf[2048];
    DWORD size = sizeof(buf);
    if (RegGetValueW(root, key.c_str(), name, RRF_RT_REG_SZ | flags, nullptr, buf, &size) == ERROR_SUCCESS) {
        return buf;
    }
    size = sizeof(buf);
    if (RegGetValueW(root, key.c_str(), name, RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND | flags, nullptr, buf, &size) ==
        ERROR_SUCCESS) {
        wchar_t out[2048];
        DWORD n = ExpandEnvironmentStringsW(buf, out, ARRAYSIZE(out));
        return n && n <= ARRAYSIZE(out) ? out : buf;
    }
    return {};
}

std::wstring FindFileRecursive(const std::wstring& dir, const std::wstring& pattern, int depth) {
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW((dir + L"\\" + pattern).c_str(), &fd);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                FindClose(h);
                return dir + L"\\" + fd.cFileName;
            }
        } while (FindNextFileW(h, &fd));
        FindClose(h);
    }
    if (depth <= 0) return {};
    h = FindFirstFileW((dir + L"\\*").c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return {};
    std::wstring found;
    do {
        std::wstring n = fd.cFileName;
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && n != L"." && n != L"..") {
            found = FindFileRecursive(dir + L"\\" + n, pattern, depth - 1);
            if (!found.empty()) break;
        }
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    return found;
}

// JSON string literal (UTF-8) from a wide string.
std::string JsonStr(const std::wstring& w) {
    std::string s = WideToUtf8(w), out = "\"";
    for (unsigned char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c < 0x20) {
            char buf[8];
            snprintf(buf, sizeof(buf), "\\u%04x", c);
            out += buf;
        } else out += (char)c;
    }
    return out + "\"";
}

// The mod's storage folder, with a subfolder per Windows user.
std::wstring StorageFolder() {
    WCHAR storage[MAX_PATH];
    WCHAR user[256];
    DWORD userLen = ARRAYSIZE(user);
    if (!Wh_GetModStoragePath(storage, ARRAYSIZE(storage)) || !GetUserNameW(user, &userLen)) {
        return {};
    }
    return std::wstring(storage) + L"\\" + user;
}

bool WindowsUsesLightTheme() {
    DWORD v = 0, size = sizeof(v);
    return RegGetValueW(HKEY_CURRENT_USER,
                        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                        L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &v, &size) ==
               ERROR_SUCCESS &&
           v != 0;
}

// Theme picked in the launcher itself (-1: none, use the mod setting). It is
// saved in the mod's storage and dropped when the mod settings change, so the
// most recent choice wins either way.
volatile LONG g_themeOverride = -1;

Theme EffectiveTheme(const Settings& s) {
    LONG o = g_themeOverride;
    return o < 0 ? s.theme : (Theme)o;
}

bool UseLightTheme(const Settings& s) {
    Theme t = EffectiveTheme(s);
    return t == Theme::Light || (t == Theme::Auto && WindowsUsesLightTheme());
}

// ============================================================ parsers

struct VdfToken {
    int type;  // 0 = string, 1 = '{', 2 = '}'
    std::string s;
};

struct VdfKv {
    int depth;
    std::string key, value, parent, grandparent;
};

std::vector<VdfToken> VdfTokenize(const std::string& d) {
    std::vector<VdfToken> t;
    size_t i = 0, n = d.size();
    while (i < n) {
        char c = d[i];
        if (c == '"') {
            std::string s;
            i++;
            while (i < n && d[i] != '"') {
                if (d[i] == '\\' && i + 1 < n) {
                    char e = d[i + 1];
                    s += (e == 'n') ? '\n' : (e == 't') ? '\t' : e;
                    i += 2;
                } else {
                    s += d[i++];
                }
            }
            i++;
            t.push_back({0, std::move(s)});
        } else if (c == '{') {
            t.push_back({1, {}});
            i++;
        } else if (c == '}') {
            t.push_back({2, {}});
            i++;
        } else if (c == '/' && i + 1 < n && d[i + 1] == '/') {
            while (i < n && d[i] != '\n') i++;
        } else {
            i++;
        }
    }
    return t;
}

std::vector<VdfKv> VdfFlatten(const std::vector<VdfToken>& t) {
    std::vector<VdfKv> out;
    std::vector<std::string> stack;
    for (size_t i = 0; i < t.size(); i++) {
        if (t[i].type == 1) {
            stack.push_back({});
        } else if (t[i].type == 2) {
            if (!stack.empty()) stack.pop_back();
        } else if (i + 1 < t.size() && t[i + 1].type == 1) {
            stack.push_back(t[i].s);
            i++;
        } else if (i + 1 < t.size() && t[i + 1].type == 0) {
            VdfKv kv;
            kv.depth = (int)stack.size();
            kv.key = t[i].s;
            kv.value = t[i + 1].s;
            if (stack.size() >= 1) kv.parent = stack[stack.size() - 1];
            if (stack.size() >= 2) kv.grandparent = stack[stack.size() - 2];
            out.push_back(std::move(kv));
            i++;
        }
    }
    return out;
}

// Minimal JSON reader: top-level members of an object as strings
// (the page's messages are flat objects).
class JsonReader {
public:
    explicit JsonReader(const std::string& s) : s_(s) {}

    bool ReadTopLevel(std::map<std::string, std::string>& out) {
        Ws();
        if (!Eat('{')) return false;
        Ws();
        if (Eat('}')) return true;
        for (;;) {
            Ws();
            std::string key;
            if (!String(key)) return false;
            Ws();
            if (!Eat(':')) return false;
            Ws();
            std::string value;
            if (!Value(value, 0)) return false;
            out[key] = value;
            Ws();
            if (Eat(',')) continue;
            return Eat('}');
        }
    }

private:
    void Ws() {
        while (i_ < s_.size() && (unsigned char)s_[i_] <= ' ') i_++;
    }
    bool Eat(char c) {
        if (i_ < s_.size() && s_[i_] == c) {
            i_++;
            return true;
        }
        return false;
    }
    static void AppendUtf8(std::string& out, unsigned cp) {
        if (cp < 0x80) {
            out += (char)cp;
        } else if (cp < 0x800) {
            out += (char)(0xC0 | (cp >> 6));
            out += (char)(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            out += (char)(0xE0 | (cp >> 12));
            out += (char)(0x80 | ((cp >> 6) & 0x3F));
            out += (char)(0x80 | (cp & 0x3F));
        } else {
            out += (char)(0xF0 | (cp >> 18));
            out += (char)(0x80 | ((cp >> 12) & 0x3F));
            out += (char)(0x80 | ((cp >> 6) & 0x3F));
            out += (char)(0x80 | (cp & 0x3F));
        }
    }
    bool Hex4(unsigned& v) {
        if (i_ + 4 > s_.size()) return false;
        v = 0;
        for (int k = 0; k < 4; k++) {
            char c = s_[i_++];
            v <<= 4;
            if (c >= '0' && c <= '9') v |= c - '0';
            else if (c >= 'a' && c <= 'f') v |= c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') v |= c - 'A' + 10;
            else return false;
        }
        return true;
    }
    bool String(std::string& out) {
        if (!Eat('"')) return false;
        while (i_ < s_.size()) {
            char c = s_[i_++];
            if (c == '"') return true;
            if (c != '\\') {
                out += c;
                continue;
            }
            if (i_ >= s_.size()) return false;
            char e = s_[i_++];
            switch (e) {
                case 'n': out += '\n'; break;
                case 't': out += '\t'; break;
                case 'r': out += '\r'; break;
                case 'b': out += '\b'; break;
                case 'f': out += '\f'; break;
                case 'u': {
                    unsigned cp;
                    if (!Hex4(cp)) return false;
                    if (cp >= 0xD800 && cp <= 0xDBFF && i_ + 1 < s_.size() && s_[i_] == '\\' &&
                        s_[i_ + 1] == 'u') {
                        i_ += 2;
                        unsigned lo;
                        if (!Hex4(lo)) return false;
                        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                    }
                    AppendUtf8(out, cp);
                    break;
                }
                default: out += e; break;
            }
        }
        return false;
    }
    bool Value(std::string& out, int depth) {
        if (depth > 32 || i_ >= s_.size()) return false;
        char c = s_[i_];
        if (c == '"') return String(out);
        if (c == '{' || c == '[') {
            char close = c == '{' ? '}' : ']';
            size_t start = i_++;
            auto raw = [&]() {
                out = s_.substr(start, i_ - start);
                return true;
            };
            Ws();
            if (Eat(close)) return raw();
            for (;;) {
                Ws();
                std::string k, v;
                if (c == '{') {
                    if (!String(k)) return false;
                    Ws();
                    if (!Eat(':')) return false;
                    Ws();
                }
                if (!Value(v, depth + 1)) return false;
                Ws();
                if (Eat(',')) continue;
                return Eat(close) && raw();
            }
        }
        while (i_ < s_.size() && s_[i_] != ',' && s_[i_] != '}' && s_[i_] != ']' &&
               (unsigned char)s_[i_] > ' ') {
            out += s_[i_++];
        }
        return true;
    }

    const std::string& s_;
    size_t i_ = 0;
};

// ============================================================ Steam

std::wstring GetSteamPath() {
    std::wstring p = RegReadStr(HKEY_CURRENT_USER, L"Software\\Valve\\Steam", L"SteamPath");
    if (p.empty()) {
        p = RegReadStr(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Valve\\Steam", L"InstallPath");
    }
    if (p.empty()) p = RegReadStr(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Valve\\Steam", L"InstallPath");
    p = NormalizePath(p);
    if (!p.empty() && !DirExists(p)) return {};
    return p;
}

std::vector<std::wstring> GetSteamLibraries(const std::wstring& steam) {
    std::vector<std::wstring> libs;
    std::set<std::wstring> seen;
    auto add = [&](std::wstring p) {
        p = NormalizePath(p);
        if (!p.empty() && seen.insert(Lower(p)).second && DirExists(p + L"\\steamapps")) {
            libs.push_back(p);
        }
    };
    add(steam);
    std::string data;
    if (ReadFileBytes(steam + L"\\steamapps\\libraryfolders.vdf", data)) {
        for (const auto& kv : VdfFlatten(VdfTokenize(data))) {
            if (_stricmp(kv.key.c_str(), "path") == 0 ||
                (IsNumeric(kv.key) && kv.value.find(':') != std::string::npos)) {
                add(Utf8ToWide(kv.value));
            }
        }
    }
    return libs;
}

// userdata\<account> of the account used most recently on this PC.
std::wstring GetSteamUserDir(const std::wstring& steam) {
    std::wstring best;
    ULONGLONG bestTime = 0;
    std::wstring userdata = steam + L"\\userdata\\";
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW((userdata + L"*").c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return {};
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == L'.') continue;
        std::wstring dir = userdata + fd.cFileName;
        ULONGLONG t = LastWriteTime(dir + L"\\config\\localconfig.vdf");
        if (t > bestTime) {
            bestTime = t;
            best = dir;
        }
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    return best;
}

struct SteamStats {
    long long lastPlayed = 0;
    long long playMinutes = 0;
};

// localconfig.vdf can be several MB: parse it again only when it changes.
std::map<std::string, SteamStats> ReadSteamStats(const std::wstring& userDir) {
    static std::wstring cachedDir;
    static ULONGLONG cachedTime = 0;
    static std::map<std::string, SteamStats> cached;
    ULONGLONG mtime = userDir.empty() ? 0 : LastWriteTime(userDir + L"\\config\\localconfig.vdf");
    if (mtime != 0 && mtime == cachedTime && userDir == cachedDir) return cached;
    std::map<std::string, SteamStats> stats;
    std::string data;
    if (mtime && ReadFileBytes(userDir + L"\\config\\localconfig.vdf", data)) {
        for (const auto& kv : VdfFlatten(VdfTokenize(data))) {
            if (_stricmp(kv.grandparent.c_str(), "apps") != 0 || !IsNumeric(kv.parent)) continue;
            if (_stricmp(kv.key.c_str(), "LastPlayed") == 0) {
                stats[kv.parent].lastPlayed = _atoi64(kv.value.c_str());
            } else if (_stricmp(kv.key.c_str(), "Playtime") == 0) {
                stats[kv.parent].playMinutes = _atoi64(kv.value.c_str());
            }
        }
    }
    cached = stats;
    cachedTime = mtime;
    cachedDir = userDir;
    return stats;
}

// Artwork from Steam's library cache (flat or per-app folder layouts).
std::wstring FindSteamArt(const std::wstring& steam, const std::wstring& appId,
                          const wchar_t* file) {
    std::wstring lc = steam + L"\\appcache\\librarycache\\";
    if (FileExists(lc + appId + L"_" + file)) return lc + appId + L"_" + file;
    if (FileExists(lc + appId + L"\\" + file)) return lc + appId + L"\\" + file;
    if (!DirExists(lc + appId)) return {};
    return FindFileRecursive(lc + appId, file, 2);
}

std::set<std::wstring> ParseExcluded(const std::wstring& s) {
    std::set<std::wstring> out;
    std::wstring cur;
    for (wchar_t c : s + L",") {
        if (c >= L'0' && c <= L'9') {
            cur += c;
        } else if (!cur.empty()) {
            out.insert(cur);
            cur.clear();
        }
    }
    return out;
}

struct Game {
    uint32_t id = 0;
    std::wstring name, dir, savesDir, shotsDir, cover, hero, header, logo;
    long long lastPlayed = 0, playMinutes = 0, installTime = 0;
    ULONGLONG size = 0;
    bool updatePending = false;
    bool installed = true;
};

std::vector<Game> ScanSteam(const Settings& s) {
    std::vector<Game> games;
    std::wstring steam = GetSteamPath();
    if (steam.empty()) return games;
    std::wstring userDir = GetSteamUserDir(steam);
    auto stats = ReadSteamStats(userDir);
    auto excluded = ParseExcluded(s.excluded);
    std::set<uint32_t> seen;

    for (const auto& lib : GetSteamLibraries(steam)) {
        std::wstring apps = lib + L"\\steamapps\\";
        WIN32_FIND_DATAW fd;
        HANDLE f = FindFirstFileW((apps + L"appmanifest_*.acf").c_str(), &fd);
        if (f == INVALID_HANDLE_VALUE) continue;
        do {
            std::string data;
            if (!ReadFileBytes(apps + fd.cFileName, data)) continue;
            std::string appId, name, installDir;
            long long flags = 0, lastPlayed = 0, size = 0, lastUpdated = 0;
            for (const auto& kv : VdfFlatten(VdfTokenize(data))) {
                if (kv.depth != 1) continue;
                const char* k = kv.key.c_str();
                if (_stricmp(k, "appid") == 0) appId = kv.value;
                else if (_stricmp(k, "name") == 0) name = kv.value;
                else if (_stricmp(k, "installdir") == 0) installDir = kv.value;
                else if (_stricmp(k, "StateFlags") == 0) flags = _atoi64(kv.value.c_str());
                else if (_stricmp(k, "LastPlayed") == 0) lastPlayed = _atoi64(kv.value.c_str());
                else if (_stricmp(k, "SizeOnDisk") == 0) size = _atoi64(kv.value.c_str());
                else if (_stricmp(k, "LastUpdated") == 0) lastUpdated = _atoi64(kv.value.c_str());
            }
            // StateFlags: 4 = fully installed, 2 = update required.
            if (!IsNumeric(appId) || installDir.empty() || !(flags & 4)) continue;
            Game g;
            g.id = (uint32_t)strtoul(appId.c_str(), nullptr, 10);
            std::wstring idW = Utf8ToWide(appId);
            if (!g.id || excluded.count(idW) || !seen.insert(g.id).second) continue;
            std::wstring dirW = Utf8ToWide(installDir);
            g.dir = IsAbsolute(dirW) ? NormalizePath(dirW) : lib + L"\\steamapps\\common\\" + dirW;
            WIN32_FILE_ATTRIBUTE_DATA a;
            if (!GetFileAttributesExW(g.dir.c_str(), GetFileExInfoStandard, &a) ||
                !(a.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                continue;
            }
            g.installTime = FileTimeToUnix(a.ftCreationTime);
            g.name = Utf8ToWide(name.empty() ? installDir : name);
            g.size = size > 0 ? (ULONGLONG)size : 0;
            g.updatePending = (flags & 2) != 0;
            g.lastPlayed = lastPlayed;
            auto it = stats.find(appId);
            if (it != stats.end()) {
                g.playMinutes = it->second.playMinutes;
                // Steam also writes LastPlayed when a game is installed:
                // ignore it when it matches the install/update time.
                long long accountLast = it->second.lastPlayed;
                bool setByInstall = lastUpdated > 0 && std::llabs(accountLast - lastUpdated) <= 15 * 60;
                if (!setByInstall) g.lastPlayed = std::max(g.lastPlayed, accountLast);
            }
            g.cover = FindSteamArt(steam, idW, L"library_600x900.jpg");
            g.header = FindSteamArt(steam, idW, L"header.jpg");
            g.logo = FindSteamArt(steam, idW, L"logo.png");
            g.hero = FindSteamArt(steam, idW, L"library_hero.jpg");
            if (!userDir.empty()) {
                std::wstring saves = userDir + L"\\" + idW + L"\\remote";
                if (DirExists(saves)) g.savesDir = saves;
                std::wstring shots = userDir + L"\\760\\remote\\" + idW + L"\\screenshots";
                if (DirExists(shots)) g.shotsDir = shots;
            }
            games.push_back(std::move(g));
        } while (FindNextFileW(f, &fd));
        FindClose(f);
    }
    return games;
}

// ---------------------------------------------------------------- not installed games

// Names, developers and publishers of the games in Steam's binary
// appinfo.vdf (formats v27, v28, v29).
struct Credits {
    std::vector<std::string> developers, publishers;  // UTF-8
};
struct AppInfoCache {
    ULONGLONG mtime = 0;
    ULONGLONG parsedAt = 0;  // GetTickCount64
    std::set<uint32_t> known;
    std::map<uint32_t, std::wstring> gameNames;
    std::map<uint32_t, Credits> credits;
};
AppInfoCache g_appInfo;

struct KvCursor {
    const uint8_t* p;
    const uint8_t* end;
    const std::vector<std::string>* strings;  // v29 key table (else null)
};

bool KvCString(KvCursor& c, std::string& out) {
    const uint8_t* z = (const uint8_t*)memchr(c.p, 0, c.end - c.p);
    if (!z) return false;
    out.assign((const char*)c.p, z - c.p);
    c.p = z + 1;
    return true;
}

bool KvKey(KvCursor& c, std::string& key) {
    if (!c.strings) return KvCString(c, key);
    if (c.end - c.p < 4) return false;
    uint32_t index;
    memcpy(&index, c.p, 4);
    c.p += 4;
    if (index >= c.strings->size()) return false;
    key = (*c.strings)[index];
    return true;
}

// What a KeyValues walk picks up: appinfo/common/{name,type}, the developers
// and publishers in common/associations, and extended/{developer,publisher}
// (older apps).
struct AppMeta {
    std::string name, type, assocType, assocName;
    Credits assoc, extended;
};
enum KvSection { kSecOther, kSecCommon, kSecAssociations, kSecAssociation, kSecExtended };

void AddCredit(std::vector<std::string>& list, const std::string& v) {
    if (!v.empty() && list.size() < 4 && std::find(list.begin(), list.end(), v) == list.end()) list.push_back(v);
}

bool KvWalk(KvCursor& c, int depth, int section, AppMeta& m) {
    if (depth > 64) return false;
    while (c.p < c.end) {
        uint8_t t = *c.p++;
        if (t == 8 || t == 11) return true;
        std::string key;
        if (!KvKey(c, key)) return false;
        switch (t) {
            case 0: {
                int sub = kSecOther;
                if (depth == 1 && _stricmp(key.c_str(), "common") == 0) sub = kSecCommon;
                else if (depth == 1 && _stricmp(key.c_str(), "extended") == 0) sub = kSecExtended;
                else if (section == kSecCommon && _stricmp(key.c_str(), "associations") == 0) sub = kSecAssociations;
                else if (section == kSecAssociations) sub = kSecAssociation;
                if (sub == kSecAssociation) {
                    m.assocType.clear();
                    m.assocName.clear();
                }
                if (!KvWalk(c, depth + 1, sub, m)) return false;
                if (sub == kSecAssociation) {
                    if (_stricmp(m.assocType.c_str(), "developer") == 0) AddCredit(m.assoc.developers, m.assocName);
                    if (_stricmp(m.assocType.c_str(), "publisher") == 0) AddCredit(m.assoc.publishers, m.assocName);
                }
                break;
            }
            case 1: {
                std::string value;
                if (!KvCString(c, value)) return false;
                if (section == kSecCommon && _stricmp(key.c_str(), "name") == 0) m.name = value;
                if (section == kSecCommon && _stricmp(key.c_str(), "type") == 0) m.type = value;
                if (section == kSecAssociation && _stricmp(key.c_str(), "type") == 0) m.assocType = value;
                if (section == kSecAssociation && _stricmp(key.c_str(), "name") == 0) m.assocName = value;
                if (section == kSecExtended && _stricmp(key.c_str(), "developer") == 0) AddCredit(m.extended.developers, value);
                if (section == kSecExtended && _stricmp(key.c_str(), "publisher") == 0) AddCredit(m.extended.publishers, value);
                break;
            }
            case 5:  // wide string
                while (c.end - c.p >= 2 && (c.p[0] || c.p[1])) c.p += 2;
                if (c.end - c.p < 2) return false;
                c.p += 2;
                break;
            case 2: case 3: case 4: case 6:
                if (c.end - c.p < 4) return false;
                c.p += 4;
                break;
            case 7: case 10:
                if (c.end - c.p < 8) return false;
                c.p += 8;
                break;
            default:
                return false;
        }
    }
    return false;
}

bool ReadExact(HANDLE h, void* buf, DWORD size) {
    DWORD read = 0;
    return ReadFile(h, buf, size, &read, nullptr) && read == size;
}

void ParseAppInfo(const std::wstring& path, const std::set<uint32_t>& wanted) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_READ,
                           FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
                           OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
    if (h == INVALID_HANDLE_VALUE) return;
    uint32_t magic = 0, universe = 0;
    if (!ReadExact(h, &magic, 4) || !ReadExact(h, &universe, 4) || (magic >> 8) != 0x075644 ||
        (magic & 0xFF) < 0x27 || (magic & 0xFF) > 0x29) {
        CloseHandle(h);
        return;
    }
    int version = magic & 0xFF;
    std::vector<std::string> strings;
    if (version >= 0x29) {
        long long tableOffset = 0;
        LARGE_INTEGER pos = {}, fileSize = {}, here = {};
        if (!ReadExact(h, &tableOffset, 8) || !GetFileSizeEx(h, &fileSize) || tableOffset <= 0 ||
            tableOffset >= fileSize.QuadPart || fileSize.QuadPart - tableOffset > 256 * 1024 * 1024) {
            CloseHandle(h);
            return;
        }
        SetFilePointerEx(h, {}, &here, FILE_CURRENT);
        pos.QuadPart = tableOffset;
        SetFilePointerEx(h, pos, nullptr, FILE_BEGIN);
        std::string table((size_t)(fileSize.QuadPart - tableOffset), '\0');
        if (!ReadExact(h, &table[0], (DWORD)table.size()) || table.size() < 4) {
            CloseHandle(h);
            return;
        }
        uint32_t count;
        memcpy(&count, table.data(), 4);
        size_t i = 4;
        for (uint32_t n = 0; n < count && i < table.size(); n++) {
            size_t z = table.find('\0', i);
            if (z == std::string::npos) break;
            strings.push_back(table.substr(i, z - i));
            i = z + 1;
        }
        SetFilePointerEx(h, here, nullptr, FILE_BEGIN);
    }
    const DWORD headerSize = version >= 0x28 ? 60 : 40;
    std::vector<uint8_t> buf;
    for (;;) {
        if (Stopping()) break;
        uint32_t appId = 0, size = 0;
        if (!ReadExact(h, &appId, 4) || appId == 0 || !ReadExact(h, &size, 4)) break;
        if (!wanted.count(appId) || size < headerSize || size > 64 * 1024 * 1024) {
            LARGE_INTEGER skip;
            skip.QuadPart = size;
            if (!SetFilePointerEx(h, skip, nullptr, FILE_CURRENT)) break;
            continue;
        }
        buf.resize(size);
        if (!ReadExact(h, buf.data(), size)) break;
        g_appInfo.known.insert(appId);
        KvCursor c{buf.data() + headerSize, buf.data() + size, version >= 0x29 ? &strings : nullptr};
        AppMeta m;
        KvWalk(c, 0, kSecOther, m);
        if (!m.name.empty() && _stricmp(m.type.c_str(), "game") == 0) {
            g_appInfo.gameNames[appId] = Utf8ToWide(m.name);
            Credits cr;
            cr.developers = m.assoc.developers.empty() ? m.extended.developers : m.assoc.developers;
            cr.publishers = m.assoc.publishers.empty() ? m.extended.publishers : m.assoc.publishers;
            if (!cr.developers.empty() || !cr.publishers.empty()) g_appInfo.credits[appId] = std::move(cr);
        }
    }
    CloseHandle(h);
}

// App IDs in the user's library: Steam caches artwork for each of them.
std::set<uint32_t> SteamLibraryAppIds(const std::wstring& steam,
                                      const std::map<std::string, SteamStats>& stats) {
    std::set<uint32_t> ids;
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW((steam + L"\\appcache\\librarycache\\*").c_str(), &fd);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            const wchar_t* n = fd.cFileName;
            if (*n < L'0' || *n > L'9') continue;
            uint32_t id = (uint32_t)wcstoul(n, nullptr, 10);
            if (id) ids.insert(id);
        } while (FindNextFileW(h, &fd));
        FindClose(h);
    }
    for (const auto& st : stats) {
        if (st.second.playMinutes > 0) ids.insert((uint32_t)strtoul(st.first.c_str(), nullptr, 10));
    }
    ids.erase(0);
    return ids;
}

// Games the user hid in the Steam library ("Hidden" collection).
std::set<std::wstring> SteamHiddenAppIds(const std::wstring& userDir) {
    std::set<std::wstring> ids;
    std::string data;
    if (userDir.empty() ||
        !ReadFileBytes(userDir + L"\\config\\cloudstorage\\cloud-storage-namespace-1.json", data)) {
        return ids;
    }
    size_t p = data.find("user-collections.hidden");
    if (p == std::string::npos) return ids;
    p = data.find("added", p);
    if (p == std::string::npos) return ids;
    p = data.find('[', p);
    size_t end = p == std::string::npos ? p : data.find(']', p);
    if (end == std::string::npos) return ids;
    std::string cur;
    for (size_t i = p + 1; i <= end; i++) {
        char c = data[i];
        if (c >= '0' && c <= '9') {
            cur += c;
        } else if (!cur.empty()) {
            ids.insert(Utf8ToWide(cur));
            cur.clear();
        }
    }
    return ids;
}

// App ids in a JSON array of numbers ("[1,2,3]").
std::vector<uint32_t> JsonIdArray(const std::string& arr) {
    std::vector<uint32_t> ids;
    std::string cur;
    for (char c : arr + ",") {
        if (c >= '0' && c <= '9') {
            cur += c;
        } else if (!cur.empty()) {
            ids.push_back((uint32_t)strtoul(cur.c_str(), nullptr, 10));
            cur.clear();
        }
    }
    return ids;
}

// The user's Steam collections with a fixed list of games, as JSON for the
// page: [{"id":"uc-…","name":"…","ids":[…]}]. Dynamic collections (built from
// filters) and "Hidden" are left out; "favorite" keeps an empty name.
std::string SteamCollectionsJson(const std::wstring& userDir) {
    std::string data;
    if (userDir.empty() ||
        !ReadFileBytes(userDir + L"\\config\\cloudstorage\\cloud-storage-namespace-1.json", data)) {
        return "[]";
    }
    std::string out;
    int count = 0;
    const std::string needle = "\"user-collections.";
    for (size_t p = data.find(needle); p != std::string::npos && count < 200; p = data.find(needle, p + 1)) {
        // only the value of a "key" member, not the name heading each entry
        size_t before = data.find_last_not_of(" \t\r\n", p ? p - 1 : 0);
        if (p == 0 || before == std::string::npos || data[before] != ':') continue;
        // the object around this key, matched brace by brace outside strings
        size_t open = data.rfind('{', p);
        if (open == std::string::npos) continue;
        int depth = 0;
        bool inStr = false;
        size_t end = std::string::npos;
        for (size_t i = open; i < data.size(); i++) {
            char c = data[i];
            if (inStr) {
                if (c == '\\') i++;
                else if (c == '"') inStr = false;
            } else if (c == '"') {
                inStr = true;
            } else if (c == '{') {
                depth++;
            } else if (c == '}' && --depth == 0) {
                end = i;
                break;
            }
        }
        if (end == std::string::npos) break;
        std::string obj = data.substr(open, end - open + 1);
        std::map<std::string, std::string> entry, value;
        JsonReader entryReader(obj);
        if (!entryReader.ReadTopLevel(entry) || entry["is_deleted"] == "true" || entry["value"].empty()) continue;
        const std::string& valueText = entry["value"];
        JsonReader valueReader(valueText);
        if (!valueReader.ReadTopLevel(value)) continue;
        std::string id = value["id"];
        if (id.empty() || id == "hidden" || !value["filterSpec"].empty()) continue;
        std::vector<uint32_t> ids = JsonIdArray(value["added"]);
        if (ids.empty()) continue;
        std::string arr;
        for (uint32_t a : ids) arr += (arr.empty() ? "" : ",") + std::to_string(a);
        if (!out.empty()) out += ",";
        out += "{\"id\":" + JsonStr(Utf8ToWide(id)) + ",\"name\":" + JsonStr(Utf8ToWide(value["name"])) +
               ",\"ids\":[" + arr + "]}";
        count++;
    }
    return "[" + out + "]";
}

// ---------------------------------------------------------------- playtime history
// Steam only keeps each game's total. The launcher writes it down whenever it
// grows, one line per game and day (playtime.tsv in the storage folder), so the
// page can show playtime per month, streaks and games picked up again.
// The first line holds the day the history began: totals written that day are
// a starting point, not time played.
int TodayYmd() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    return t.wYear * 10000 + t.wMonth * 100 + t.wDay;
}

std::string UpdateHistory(const std::vector<Game>& games) {
    // scanner thread only
    static bool loaded = false;
    static int since = 0;
    static std::map<uint32_t, long long> last;
    static std::string rows;  // "[day,id,min]," entries
    std::wstring path = g_storage + L"\\playtime.tsv";
    if (!loaded) {
        loaded = true;
        std::string data;
        if (ReadFileBytes(path, data)) {
            size_t pos = 0;
            while (pos < data.size()) {
                size_t eol = data.find('\n', pos);
                if (eol == std::string::npos) eol = data.size();
                std::string line = data.substr(pos, eol - pos);
                pos = eol + 1;
                if (line.rfind("since\t", 0) == 0) {
                    since = atoi(line.c_str() + 6);
                    continue;
                }
                int day = atoi(line.c_str());
                size_t t1 = line.find('\t'), t2 = t1 == std::string::npos ? t1 : line.find('\t', t1 + 1);
                if (day < 19700101 || t2 == std::string::npos) continue;
                uint32_t id = (uint32_t)strtoul(line.c_str() + t1 + 1, nullptr, 10);
                long long min = _atoi64(line.c_str() + t2 + 1);
                size_t t3 = line.find('\t', t2 + 1);
                long long played = t3 == std::string::npos ? 0 : _atoi64(line.c_str() + t3 + 1);
                last[id] = min;
                rows += "[" + std::to_string(day) + "," + std::to_string(id) + "," + std::to_string(min) + "," +
                        std::to_string(played) + "],";
            }
        }
    }
    int today = TodayYmd();
    std::string add;
    bool fresh = since == 0;
    if (fresh) since = today;
    for (const auto& g : games) {
        if (g.playMinutes <= 0) continue;
        auto it = last.find(g.id);
        if (it != last.end() && it->second >= g.playMinutes) continue;
        // with the time it was last played: the next line shows how long the pause was
        last[g.id] = g.playMinutes;
        std::string played = std::to_string(g.lastPlayed);
        add += std::to_string(today) + "\t" + std::to_string(g.id) + "\t" + std::to_string(g.playMinutes) + "\t" +
               played + "\n";
        rows += "[" + std::to_string(today) + "," + std::to_string(g.id) + "," + std::to_string(g.playMinutes) + "," +
                played + "],";
    }
    if (fresh || !add.empty()) {
        std::string data;
        if (fresh || !ReadFileBytes(path, data)) data = "since\t" + std::to_string(since) + "\n";
        WriteFileBytes(path, data + add);
    }
    std::string r = rows;
    if (!r.empty()) r.pop_back();
    return "{\"since\":" + std::to_string(since) + ",\"today\":" + std::to_string(today) + ",\"rows\":[" + r + "]}";
}

void ScanSteamLibrary(const Settings& s, std::vector<Game>& games) {
    std::wstring steam = GetSteamPath();
    if (steam.empty()) return;
    std::set<uint32_t> installed;
    for (const auto& g : games) installed.insert(g.id);
    auto excluded = ParseExcluded(s.excluded);
    std::wstring userDir = GetSteamUserDir(steam);
    for (const auto& id : SteamHiddenAppIds(userDir)) excluded.insert(id);
    auto stats = ReadSteamStats(userDir);
    auto ids = SteamLibraryAppIds(steam, stats);
    ids.insert(installed.begin(), installed.end());  // their developers and publishers

    // appinfo.vdf changes often while Steam runs: re-read it at most every
    // 30 minutes, or after 5 minutes when the library has unknown games.
    std::wstring appinfo = steam + L"\\appcache\\appinfo.vdf";
    ULONGLONG mtime = LastWriteTime(appinfo);
    bool missing = false;
    for (uint32_t id : ids) {
        if (!g_appInfo.known.count(id)) {
            missing = true;
            break;
        }
    }
    ULONGLONG now = GetTickCount64();
    ULONGLONG age = now - g_appInfo.parsedAt;
    if (g_appInfo.parsedAt == 0 || (mtime != g_appInfo.mtime && age > 30 * 60 * 1000) ||
        (missing && age > 5 * 60 * 1000)) {
        g_appInfo.known.clear();
        g_appInfo.gameNames.clear();
        g_appInfo.credits.clear();
        ParseAppInfo(appinfo, ids);
        Wh_Log(L"appinfo.vdf: %d games, %d with developer or publisher", (int)g_appInfo.gameNames.size(),
               (int)g_appInfo.credits.size());
        g_appInfo.mtime = mtime;
        g_appInfo.parsedAt = now ? now : 1;
    }

    for (uint32_t id : ids) {
        if (Stopping()) return;
        auto nameIt = g_appInfo.gameNames.find(id);
        if (nameIt == g_appInfo.gameNames.end()) continue;  // not a game
        std::wstring idW = std::to_wstring(id);
        if (installed.count(id) || excluded.count(idW)) continue;
        Game g;
        g.id = id;
        g.installed = false;
        g.name = nameIt->second;
        auto st = stats.find(WideToUtf8(idW));
        if (st != stats.end()) {
            g.playMinutes = st->second.playMinutes;
            g.lastPlayed = st->second.lastPlayed;
        }
        g.cover = FindSteamArt(steam, idW, L"library_600x900.jpg");
        g.header = FindSteamArt(steam, idW, L"header.jpg");
        g.logo = FindSteamArt(steam, idW, L"logo.png");
        g.hero = FindSteamArt(steam, idW, L"library_hero.jpg");
        games.push_back(std::move(g));
    }
}

// ============================================================ library data

struct GameLinks {
    std::wstring dir, saves, shots;
};

struct LibraryData {
    std::string json;  // the games array
    std::string extra;  // ,"collections":[…],"history":{…}
    std::map<uint32_t, GameLinks> links;  // installed games
    std::set<uint32_t> ids;               // every game sent to the page
};

// Copies an image into the page's folder, so the page can use it as a WebGL
// texture (same origin). Returns the page URL, or "" when there's no image.
std::string PublishArt(const std::wstring& src, const std::wstring& uiDir, uint32_t id,
                       const wchar_t* name) {
    if (src.empty()) return {};
    std::wstring dir = uiDir + L"\\art\\" + std::to_wstring(id);
    std::wstring dst = dir + L"\\" + name;
    ULONGLONG srcTime = LastWriteTime(src);
    if (!srcTime) return {};
    // A hard link (or a copy on another drive) keeps the modification time,
    // so equal times mean the file is up to date.
    if (LastWriteTime(dst) != srcTime) {
        SHCreateDirectoryExW(nullptr, dir.c_str(), nullptr);
        DeleteFileW(dst.c_str());
        if (!CreateHardLinkW(dst.c_str(), src.c_str(), nullptr) &&
            !CopyFileW(src.c_str(), dst.c_str(), FALSE)) {
            return {};
        }
    }
    return "art/" + std::to_string(id) + "/" + WideToUtf8(name) + "?v=" +
           std::to_string(srcTime % 1000000007ULL);
}

// Removes art folders of games that are no longer installed.
void CleanArt(const std::wstring& uiDir, const std::set<uint32_t>& keep) {
    std::wstring art = uiDir + L"\\art";
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW((art + L"\\*").c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    std::vector<std::wstring> stale;
    do {
        std::wstring n = fd.cFileName;
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || n.empty() ||
            !IsNumeric(WideToUtf8(n))) {
            continue;
        }
        if (!keep.count((uint32_t)wcstoul(n.c_str(), nullptr, 10))) stale.push_back(art + L"\\" + n);
    } while (FindNextFileW(h, &fd));
    FindClose(h);
    for (const auto& d : stale) {
        for (auto f : {L"\\cover.jpg", L"\\hero.jpg", L"\\logo.png", L"\\dl-cover.jpg", L"\\dl-hero.jpg",
                       L"\\dl-logo.png"}) {
            DeleteFileW((d + f).c_str());
        }
        RemoveDirectoryW(d.c_str());
    }
}

const char* ThemeName(Theme t) {
    return t == Theme::Dark ? "dark" : t == Theme::Light ? "light" : "auto";
}


// ---------------------------------------------------------------- missing artwork

// Steam only caches the artwork of games shown in its library at least once.
// For the others, the art thread asks Steam's store for the image names and
// downloads them into art\<appid>\dl-*.jpg. Failures are remembered for a week.
const int kNeedCover = 1;
const int kNeedHero = 2;
const int kNeedLogo = 4;
SRWLOCK g_artLock = SRWLOCK_INIT;
std::map<uint32_t, int> g_artQueue;
HANDLE g_artEvent = nullptr;
HANDLE g_artThread = nullptr;

bool HttpGet(const wchar_t* host, const std::wstring& path, std::string& out);

std::string UrlEncode(const std::string& s) {
    std::string out;
    for (unsigned char c : s) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            out += (char)c;
        } else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%%%02X", c);
            out += buf;
        }
    }
    return out;
}

// Value of the first "key":"value" string member found in a JSON text.
std::string JsonStringField(const std::string& json, const std::string& key) {
    std::string needle = "\"" + key + "\":\"";
    size_t p = json.find(needle);
    if (p == std::string::npos) return {};
    std::string out;
    for (size_t i = p + needle.size(); i < json.size(); i++) {
        char c = json[i];
        if (c == '"') return out;
        if (c == '\\' && i + 1 < json.size()) {
            c = json[++i];
            if (c == 'u') return {};  // not expected in file names
        }
        out += c;
    }
    return {};
}

bool HttpGetUrl(const std::string& url, std::string& out) {
    const std::string scheme = "https://";
    if (url.compare(0, scheme.size(), scheme) != 0) return false;
    size_t slash = url.find('/', scheme.size());
    if (slash == std::string::npos) return false;
    std::wstring host = Utf8ToWide(url.substr(scheme.size(), slash - scheme.size()));
    return HttpGet(host.c_str(), Utf8ToWide(url.substr(slash)), out);
}

bool LooksLikeImage(const std::string& d) {
    if (d.size() < 2000) return false;
    const unsigned char* b = (const unsigned char*)d.data();
    return (b[0] == 0xFF && b[1] == 0xD8) || (b[0] == 0x89 && b[1] == 'P' && b[2] == 'N' && b[3] == 'G');
}

// Tries each URL in turn and saves the first image that downloads.
bool DownloadFirst(const std::vector<std::string>& urls, const std::wstring& dst) {
    for (const auto& u : urls) {
        if (Stopping()) return false;
        std::string data;
        if (HttpGetUrl(u, data) && LooksLikeImage(data)) {
            std::wstring tmp = dst + L".part";
            if (WriteFileBytes(tmp, data) && MoveFileExW(tmp.c_str(), dst.c_str(), MOVEFILE_REPLACE_EXISTING)) {
                return true;
            }
            DeleteFileW(tmp.c_str());
        }
    }
    return false;
}

// Returns the assets that couldn't be downloaded (kNeed* bits).
int DownloadArt(uint32_t id, int need, const std::wstring& uiDir) {
    std::string ids = std::to_string(id);
    // The store knows the exact (often hashed) path of each image.
    std::string query = "{\"ids\":[{\"appid\":" + ids +
                        "}],\"context\":{\"language\":\"english\",\"country_code\":\"US\"},"
                        "\"data_request\":{\"include_assets\":true}}";
    std::string body, format;
    if (HttpGet(L"api.steampowered.com",
                L"/IStoreBrowseService/GetItems/v1/?input_json=" + Utf8ToWide(UrlEncode(query)), body)) {
        format = JsonStringField(body, "asset_url_format");
    }
    const std::string base = "https://shared.steamstatic.com/store_item_assets/";
    auto assetUrl = [&](const std::string& field) -> std::string {
        std::string file = JsonStringField(body, field);
        size_t at = format.find("${FILENAME}");
        if (file.empty() || at == std::string::npos) return {};
        std::string path = format;
        path.replace(at, 11, file);
        return base + path;
    };
    std::wstring dir = uiDir + L"\\art\\" + std::to_wstring(id);
    SHCreateDirectoryExW(nullptr, dir.c_str(), nullptr);
    int failed = 0;
    if (need & kNeedCover) {
        std::vector<std::string> urls;
        for (auto f : {"library_capsule_2x", "library_capsule"}) {
            std::string u = assetUrl(f);
            if (!u.empty()) urls.push_back(u);
        }
        urls.push_back(base + "steam/apps/" + ids + "/library_600x900_2x.jpg");
        urls.push_back("https://cdn.cloudflare.steamstatic.com/steam/apps/" + ids + "/library_600x900_2x.jpg");
        if (!DownloadFirst(urls, dir + L"\\dl-cover.jpg")) failed |= kNeedCover;
    }
    if (need & kNeedHero) {
        std::vector<std::string> urls;
        std::string u = assetUrl("library_hero");
        if (!u.empty()) urls.push_back(u);
        urls.push_back(base + "steam/apps/" + ids + "/library_hero.jpg");
        if (!DownloadFirst(urls, dir + L"\\dl-hero.jpg")) failed |= kNeedHero;
    }
    if (need & kNeedLogo) {
        std::vector<std::string> urls = {base + "steam/apps/" + ids + "/logo.png",
                                         "https://cdn.cloudflare.steamstatic.com/steam/apps/" + ids + "/logo.png"};
        if (!DownloadFirst(urls, dir + L"\\dl-logo.png")) failed |= kNeedLogo;
    }
    return failed;
}

DWORD WINAPI ArtThread(LPVOID) {
    // id -> (when, which assets failed); retried after a week
    std::wstring failedFile = g_storage + L"\\art-failed.txt";
    std::map<uint32_t, std::pair<long long, int>> failed;
    {
        std::string data;
        if (ReadFileBytes(failedFile, data)) {
            size_t pos = 0;
            while (pos < data.size()) {
                size_t eol = data.find('\n', pos);
                if (eol == std::string::npos) eol = data.size();
                std::string line = data.substr(pos, eol - pos);
                size_t tab = line.find('\t');
                if (tab != std::string::npos) {
                    size_t tab2 = line.find('\t', tab + 1);
                    int mask = tab2 == std::string::npos ? (kNeedCover | kNeedHero | kNeedLogo)
                                                         : atoi(line.c_str() + tab2 + 1);
                    failed[(uint32_t)strtoul(line.c_str(), nullptr, 10)] = {_atoi64(line.c_str() + tab + 1), mask};
                }
                pos = eol + 1;
            }
        }
    }
    auto saveFailed = [&]() {
        std::string data;
        for (const auto& f : failed) {
            data += std::to_string(f.first) + "\t" + std::to_string(f.second.first) + "\t" +
                    std::to_string(f.second.second) + "\n";
        }
        WriteFileBytes(failedFile, data);
    };
    HANDLE handles[2] = {g_stopEvent, g_artEvent};
    while (WaitForMultipleObjects(2, handles, FALSE, INFINITE) == WAIT_OBJECT_0 + 1) {
        int done = 0;
        for (;;) {
            if (Stopping()) return 0;
            uint32_t id = 0;
            int need = 0;
            AcquireSRWLockExclusive(&g_artLock);
            if (!g_artQueue.empty()) {
                id = g_artQueue.begin()->first;
                need = g_artQueue.begin()->second;
                g_artQueue.erase(g_artQueue.begin());
            }
            ReleaseSRWLockExclusive(&g_artLock);
            if (!id) break;
            long long now = (long long)time(nullptr);
            auto f = failed.find(id);
            bool recent = f != failed.end() && now - f->second.first < 7 * 86400;
            if (recent) need &= ~f->second.second;
            if (!need) continue;
            int miss = DownloadArt(id, need, g_uiDir);
            if (miss != need) {
                // Show new art in batches rather than rebuilding for each image.
                if (++done % 12 == 0) SetEvent(g_resyncEvent);
            }
            bool had = f != failed.end();
            long long since = had ? f->second.first : now;
            int keep = recent ? f->second.second : 0;
            int mask = (keep | miss) & ~(need & ~miss);  // what is still missing
            if (mask) failed[id] = {miss ? now : since, mask};
            else if (had) failed.erase(id);
            if (miss || had) saveFailed();
            WaitForSingleObject(g_stopEvent, 120);
        }
        if (done % 12) SetEvent(g_resyncEvent);
    }
    return 0;
}

void QueueArt(uint32_t id, int need) {
    AcquireSRWLockExclusive(&g_artLock);
    g_artQueue[id] |= need;
    ReleaseSRWLockExclusive(&g_artLock);
}

// URL of a downloaded image, or "".
std::string DownloadedArt(const std::wstring& uiDir, uint32_t id, const wchar_t* name) {
    std::wstring path = uiDir + L"\\art\\" + std::to_wstring(id) + L"\\" + name;
    ULONGLONG t = LastWriteTime(path);
    if (!t) return {};
    return "art/" + std::to_string(id) + "/" + WideToUtf8(name) + "?v=" + std::to_string(t % 1000000007ULL);
}

LibraryData BuildLibrary(const Settings& s, const std::wstring& uiDir) {
    LibraryData lib;
    std::vector<Game> games = ScanSteam(s);
    InterlockedExchange(&g_steamFound, GetSteamPath().empty() ? 0 : 1);
    ScanSteamLibrary(s, games);
    std::string arr;
    for (const auto& g : games) {
        lib.ids.insert(g.id);
        if (g.installed) lib.links[g.id] = {g.dir, g.savesDir, g.shotsDir};
        // Cover: Steam's cache, then a downloaded one, then the wide header.
        std::string cover = PublishArt(g.cover, uiDir, g.id, L"cover.jpg");
        std::string hero = PublishArt(g.hero, uiDir, g.id, L"hero.jpg");
        int need = 0;
        if (cover.empty()) {
            cover = DownloadedArt(uiDir, g.id, L"dl-cover.jpg");
            if (cover.empty()) need |= kNeedCover;
        }
        if (hero.empty()) {
            hero = DownloadedArt(uiDir, g.id, L"dl-hero.jpg");
            if (hero.empty()) need |= kNeedHero;
        }
        std::string logo = PublishArt(g.logo, uiDir, g.id, L"logo.png");
        if (logo.empty()) {
            logo = DownloadedArt(uiDir, g.id, L"dl-logo.png");
            if (logo.empty()) need |= kNeedLogo;
        }
        if (cover.empty()) cover = PublishArt(g.header, uiDir, g.id, L"cover.jpg");
        if (need && s.downloadArt) QueueArt(g.id, need);
        if (!arr.empty()) arr += ",";
        arr += "{\"id\":" + std::to_string(g.id) + ",\"name\":" + JsonStr(g.name) +
               ",\"min\":" + std::to_string(g.playMinutes) +
               ",\"last\":" + std::to_string(g.lastPlayed) +
               ",\"inst\":" + std::to_string(g.installTime) +
               ",\"size\":" + std::to_string(g.size) +
               ",\"update\":" + (g.updatePending ? "true" : "false") +
               ",\"installed\":" + (g.installed ? "true" : "false") +
               ",\"saves\":" + (g.savesDir.empty() ? "false" : "true") +
               ",\"shots\":" + (g.shotsDir.empty() ? "false" : "true") +
               ",\"cover\":" + JsonStr(Utf8ToWide(cover)) + ",\"hero\":" + JsonStr(Utf8ToWide(hero)) +
               ",\"logo\":" + JsonStr(Utf8ToWide(logo));
        auto cr = g_appInfo.credits.find(g.id);
        if (cr != g_appInfo.credits.end()) {
            auto list = [](const std::vector<std::string>& v) {
                std::string a;
                for (const auto& x : v) a += (a.empty() ? "" : ",") + JsonStr(Utf8ToWide(x));
                return "[" + a + "]";
            };
            arr += ",\"dev\":" + list(cr->second.developers) + ",\"pub\":" + list(cr->second.publishers);
        }
        arr += "}";
    }
    CleanArt(uiDir, lib.ids);
    lib.json = "[" + arr + "]";
    std::wstring steam = GetSteamPath();
    lib.extra = ",\"collections\":" + SteamCollectionsJson(steam.empty() ? std::wstring() : GetSteamUserDir(steam)) +
                ",\"history\":" + UpdateHistory(games);
    return lib;
}

// ============================================================ threads and messages

const UINT WM_APP_LIBRARY = WM_APP + 1;   // lParam: LibraryData*
const UINT WM_APP_NEWS = WM_APP + 2;      // lParam: NewsResult*
const UINT WM_APP_TRAY = WM_APP + 3;
const UINT WM_APP_STOP = WM_APP + 4;
const UINT WM_APP_SETTINGS = WM_APP + 5;
const UINT WM_APP_SHOW = WM_APP + 6;
const UINT_PTR kHotkeyId = 1;
const UINT_PTR kTimerPreload = 1;
const UINT_PTR kTimerHide = 2;
const UINT_PTR kTimerWatchdog = 3;
const UINT kTrayId = 1;
const UINT kMenuOpen = 1;
const UINT kMenuRefresh = 2;
const UINT kMenuQuit = 3;

HANDLE g_scanThread = nullptr;
HANDLE g_padThread = nullptr;
HANDLE g_uiThread = nullptr;
HANDLE g_uiReady = nullptr;
HWND g_hwnd = nullptr;

// ---------------------------------------------------------------- scanner

DWORD WINAPI ScanThread(LPVOID) {
    std::string lastJson;
    while (!Stopping()) {
        Settings s = GetSettings();
        LibraryData* lib = new LibraryData(BuildLibrary(s, g_uiDir));
        if (s.downloadArt && g_artEvent) SetEvent(g_artEvent);
        if (lib->json + lib->extra != lastJson && g_hwnd) {
            lastJson = lib->json + lib->extra;
            Wh_Log(L"Library: %d games, %d bytes%s", (int)lib->ids.size(), (int)lastJson.size(),
                   lastJson.find("\"dev\":") != std::string::npos ? L", with credits" : L"");
#ifdef SL_DUMP_LIBRARY  // test builds only
            WriteFileBytes(g_storage + L"\\library-dump.json", lastJson);
#endif
            if (!PostMessageW(g_hwnd, WM_APP_LIBRARY, 0, (LPARAM)lib)) delete lib;
        } else {
            delete lib;
        }

        // Wait for Steam to change its files, for a refresh request, or 15 minutes.
        std::vector<HANDLE> handles = {g_stopEvent, g_resyncEvent};
        std::wstring steam = GetSteamPath();
        if (!steam.empty()) {
            std::vector<std::wstring> dirs;
            for (const auto& l : GetSteamLibraries(steam)) dirs.push_back(l + L"\\steamapps");
            std::wstring user = GetSteamUserDir(steam);
            if (!user.empty()) dirs.push_back(user + L"\\config");
            for (const auto& d : dirs) {
                if (handles.size() >= MAXIMUM_WAIT_OBJECTS) break;
                HANDLE c = FindFirstChangeNotificationW(
                    d.c_str(), FALSE,
                    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
                        FILE_NOTIFY_CHANGE_LAST_WRITE);
                if (c != INVALID_HANDLE_VALUE) handles.push_back(c);
            }
        }
        DWORD count = (DWORD)handles.size();
        DWORD r = WaitForMultipleObjects(count, handles.data(), FALSE, 15 * 60 * 1000);
        if (r >= WAIT_OBJECT_0 + 2 && r < WAIT_OBJECT_0 + count) {
            // Debounce: Steam rewrites these files several times in a row.
            DWORD start = GetTickCount();
            for (;;) {
                FindNextChangeNotification(handles[r - WAIT_OBJECT_0]);
                r = WaitForMultipleObjects(count, handles.data(), FALSE, 3000);
                if (r < WAIT_OBJECT_0 + 2 || r >= WAIT_OBJECT_0 + count) break;
                if (GetTickCount() - start > 30000) break;
            }
        }
        for (DWORD i = 2; i < count; i++) FindCloseChangeNotification(handles[i]);
    }
    return 0;
}

// ---------------------------------------------------------------- news

struct NewsResult {
    uint32_t id;
    bool ok;
    std::string body;
};

// HTTPS request. Returns true on HTTP 200; the status code goes to *status.
bool HttpRequest(const wchar_t* method, const wchar_t* host, const std::wstring& path,
                 const std::wstring& headers, const std::string& body, std::string& out,
                 DWORD* statusOut = nullptr) {
    bool ok = false;
    if (statusOut) *statusOut = 0;
    HINTERNET session = WinHttpOpen(L"Windhawk-SteamLauncher/0.1", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) {
        session = WinHttpOpen(L"Windhawk-SteamLauncher/0.1", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                              WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    }
    if (!session) return false;
    WinHttpSetTimeouts(session, 5000, 5000, 8000, 8000);
    HINTERNET conn = WinHttpConnect(session, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
    HINTERNET req = conn ? WinHttpOpenRequest(conn, method, path.c_str(), nullptr, WINHTTP_NO_REFERER,
                                              WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE)
                         : nullptr;
    if (req && WinHttpSendRequest(req, headers.empty() ? WINHTTP_NO_ADDITIONAL_HEADERS : headers.c_str(),
                                  headers.empty() ? 0 : (DWORD)-1L,
                                  body.empty() ? WINHTTP_NO_REQUEST_DATA : (LPVOID)body.data(),
                                  (DWORD)body.size(), (DWORD)body.size(), 0) &&
        WinHttpReceiveResponse(req, nullptr)) {
        DWORD status = 0, size = sizeof(status);
        WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                            WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX);
        if (statusOut) *statusOut = status;
        if (status == 200) {
            ok = true;
            for (;;) {
                DWORD avail = 0;
                if (!WinHttpQueryDataAvailable(req, &avail)) {
                    ok = false;
                    break;
                }
                if (!avail) break;
                size_t at = out.size();
                out.resize(at + avail);
                DWORD read = 0;
                if (!WinHttpReadData(req, &out[at], avail, &read)) {
                    ok = false;
                    break;
                }
                out.resize(at + read);
                if (out.size() > 4 * 1024 * 1024) {
                    ok = false;
                    break;
                }
            }
        }
    }
    if (req) WinHttpCloseHandle(req);
    if (conn) WinHttpCloseHandle(conn);
    WinHttpCloseHandle(session);
    return ok;
}

bool HttpGet(const wchar_t* host, const std::wstring& path, std::string& out) {
    return HttpRequest(L"GET", host, path, {}, {}, out);
}


// ---------------------------------------------------------------- ratings and time to beat

struct InfoResult {
    uint32_t id;
    std::string json;  // {"reviews":…,"metacritic":…,"ttb":…}
};

// The number after the first "key": at or after `from`, or "" when missing.
std::string JsonNumberField(const std::string& json, const std::string& key, size_t from = 0) {
    std::string needle = "\"" + key + "\":";
    size_t p = json.find(needle, from);
    if (p == std::string::npos) return {};
    p += needle.size();
    while (p < json.size() && json[p] == ' ') p++;
    size_t e = p;
    if (e < json.size() && json[e] == '-') e++;
    while (e < json.size() && (isdigit((unsigned char)json[e]) || json[e] == '.')) e++;
    std::string n = json.substr(p, e - p);
    if (n.empty() || n == "-" || n == ".") return {};
    return n;
}

// Steam user reviews, all languages and purchase types, like the store page.
std::string SteamReviews(uint32_t id) {
    std::string q = "{\"appid\":" + std::to_string(id) +
                    ",\"num_per_page\":1,\"languages\":[\"all\"],\"purchase_type\":1}";
    std::string body;
    if (!HttpGet(L"api.steampowered.com",
                 L"/IUserReviewsService/GetAppReviews/v1/?input_json=" + Utf8ToWide(UrlEncode(q)), body)) {
        return {};
    }
    size_t qs = body.find("\"query_summary\":{");
    if (qs == std::string::npos) return {};
    std::string score = JsonNumberField(body, "review_score", qs);
    std::string positive = JsonNumberField(body, "total_positive", qs);
    std::string total = JsonNumberField(body, "total_reviews", qs);
    if (total.empty()) return {};
    return "{\"score\":" + (score.empty() ? "0" : score) + ",\"positive\":" +
           (positive.empty() ? "0" : positive) + ",\"total\":" + total + "}";
}

// Metacritic score as the Steam store lists it ("null" when the game has none).
std::string Metacritic(uint32_t id, bool& ok) {
    std::string body;
    ok = HttpGet(L"store.steampowered.com",
                 L"/api/appdetails?appids=" + std::to_wstring(id) + L"&filters=metacritic", body);
    if (!ok) return {};
    size_t m = body.find("\"metacritic\":{");
    if (m == std::string::npos) return "null";
    std::string score = JsonNumberField(body, "score", m);
    std::string url = JsonStringField(body.substr(m), "url");
    if (score.empty()) return "null";
    if (url.rfind("https://www.metacritic.com/", 0) != 0) url.clear();
    return "{\"score\":" + score + ",\"url\":" + JsonStr(Utf8ToWide(url)) + "}";
}

// IGDB: an app access token from Twitch, kept until it expires.
SRWLOCK g_igdbLock = SRWLOCK_INIT;
std::wstring g_igdbToken, g_igdbTokenFor;
ULONGLONG g_igdbTokenUntil = 0;

std::wstring IgdbToken(const Settings& s, bool renew) {
    AcquireSRWLockExclusive(&g_igdbLock);
    std::wstring token;
    if (!renew && g_igdbTokenFor == s.igdbClientId + L"|" + s.igdbClientSecret &&
        GetTickCount64() < g_igdbTokenUntil) {
        token = g_igdbToken;
    } else {
        std::string body;
        std::wstring path = L"/oauth2/token?client_id=" + Utf8ToWide(UrlEncode(WideToUtf8(s.igdbClientId))) +
                            L"&client_secret=" + Utf8ToWide(UrlEncode(WideToUtf8(s.igdbClientSecret))) +
                            L"&grant_type=client_credentials";
        if (HttpRequest(L"POST", L"id.twitch.tv", path, {}, {}, body)) {
            std::string t = JsonStringField(body, "access_token");
            long long ttl = _atoi64(JsonNumberField(body, "expires_in").c_str());
            bool safe = !t.empty() && t.find_first_not_of(
                "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.") == std::string::npos;
            if (safe) {
                g_igdbToken = Utf8ToWide(t);
                g_igdbTokenFor = s.igdbClientId + L"|" + s.igdbClientSecret;
                g_igdbTokenUntil = GetTickCount64() + (ULONGLONG)std::max(60LL, ttl - 600) * 1000;
                token = g_igdbToken;
            }
        } else {
            Wh_Log(L"IGDB: Twitch refused the client ID or secret");
        }
    }
    ReleaseSRWLockExclusive(&g_igdbLock);
    return token;
}

bool IgdbQuery(const Settings& s, const wchar_t* endpoint, const std::string& query, std::string& out) {
    for (int attempt = 0; attempt < 2; attempt++) {
        std::wstring token = IgdbToken(s, attempt > 0);
        if (token.empty()) return false;
        std::wstring headers = L"Client-ID: " + s.igdbClientId + L"\r\nAuthorization: Bearer " + token +
                               L"\r\nAccept: application/json\r\nContent-Type: text/plain\r\n";
        DWORD status = 0;
        out.clear();
        if (HttpRequest(L"POST", L"api.igdb.com", std::wstring(L"/v4/") + endpoint, headers, query, out, &status)) {
            return true;
        }
        if (status != 401) return false;  // 401: the token expired, get a new one
    }
    return false;
}

// Time to beat in seconds ("null" when IGDB doesn't know the game).
std::string TimeToBeat(const Settings& s, uint32_t id, bool& ok) {
    ok = false;
    std::string body, ids = std::to_string(id);
    // Steam is external source 1; older IGDB data used the field "category"
    if (!IgdbQuery(s, L"external_games",
                   "fields game; where uid = \"" + ids + "\" & external_game_source = 1; limit 1;", body) &&
        !IgdbQuery(s, L"external_games", "fields game; where uid = \"" + ids + "\" & category = 1; limit 1;",
                   body)) {
        return {};
    }
    ok = true;
    std::string game = JsonNumberField(body, "game");
    if (game.empty()) return "null";
    if (!IgdbQuery(s, L"game_time_to_beats",
                   "fields hastily,normally,completely,count; where game_id = " + game + "; limit 1;", body)) {
        ok = false;
        return {};
    }
    std::string h = JsonNumberField(body, "hastily"), n = JsonNumberField(body, "normally"),
                c = JsonNumberField(body, "completely"), count = JsonNumberField(body, "count");
    if (h.empty() && n.empty() && c.empty()) return "null";
    return "{\"hastily\":" + (h.empty() ? "0" : h) + ",\"normally\":" + (n.empty() ? "0" : n) +
           ",\"completely\":" + (c.empty() ? "0" : c) + ",\"count\":" + (count.empty() ? "0" : count) + "}";
}

const UINT WM_APP_INFO_MSG = WM_APP + 7;  // lParam: InfoResult*

DWORD WINAPI InfoThread(LPVOID param) {
    uint32_t id = (uint32_t)(UINT_PTR)param;
    Settings s = GetSettings();
    bool igdb = !s.igdbClientId.empty() && !s.igdbClientSecret.empty();
    std::wstring dir = g_storage + L"\\info";
    std::wstring path = dir + L"\\" + std::to_wstring(id) + (igdb ? L"-igdb.json" : L".json");
    long long now = (long long)time(nullptr);
    std::string json, cached;
    if (ReadFileBytes(path, cached)) {
        size_t nl = cached.find('\n');
        if (nl != std::string::npos && now - _atoi64(cached.c_str()) < 3 * 86400 && cached[nl + 1] == '{') {
            json = cached.substr(nl + 1);
        }
    }
    if (json.empty()) {
        bool mcOk = false, tbOk = !igdb;
        std::string rv = SteamReviews(id);
        std::string mc = Metacritic(id, mcOk);
        std::string tb = igdb ? TimeToBeat(s, id, tbOk) : "null";
        json = "{\"reviews\":" + (rv.empty() ? std::string("null") : rv) +
               ",\"metacritic\":" + (mc.empty() ? std::string("null") : mc) +
               ",\"ttb\":" + (tb.empty() ? std::string("null") : tb) + "}";
        // only complete answers are kept, so an offline moment isn't remembered
        if (!rv.empty() && mcOk && tbOk) {
            SHCreateDirectoryExW(nullptr, dir.c_str(), nullptr);
            WriteFileBytes(path, std::to_string(now) + "\n" + json);
        }
    }
    InfoResult* r = new InfoResult{id, json};
    if (!g_hwnd || !PostMessageW(g_hwnd, WM_APP_INFO_MSG, 0, (LPARAM)r)) delete r;
    return 0;
}

DWORD WINAPI NewsThread(LPVOID param) {
    uint32_t id = (uint32_t)(UINT_PTR)param;
    NewsResult* r = new NewsResult{id, false, {}};
    // The developers' own announcements first: the unfiltered list also has
    // third-party press feeds, often in other languages.
    std::wstring base = L"/ISteamNews/GetNewsForApp/v2/?appid=" + std::to_wstring(id) +
                        L"&count=10&maxlength=400&format=json";
    r->ok = HttpGet(L"api.steampowered.com", base + L"&feeds=steam_community_announcements", r->body);
    if (!r->ok || r->body.find("\"newsitems\":[]") != std::string::npos) {
        r->body.clear();
        r->ok = HttpGet(L"api.steampowered.com", base, r->body);
    }
    // Only a JSON object is forwarded to the page.
    size_t first = r->body.find_first_not_of(" \t\r\n");
    if (r->ok && (first == std::string::npos || r->body[first] != '{')) r->ok = false;
    if (!g_hwnd || !PostMessageW(g_hwnd, WM_APP_NEWS, 0, (LPARAM)r)) delete r;
    return 0;
}

// A game's micro-trailer: the few silent seconds Steam plays when you hover
// a game in the store. Only its address is looked up here; the page streams
// the video from Steam's servers when it shows it.
const UINT WM_APP_TRAILER = WM_APP + 8;  // lParam: InfoResult* (id, message)
const UINT WM_APP_PAD = WM_APP + 9;

DWORD WINAPI TrailerThread(LPVOID param) {
    uint32_t id = (uint32_t)(UINT_PTR)param;
    std::string query = "{\"ids\":[{\"appid\":" + std::to_string(id) +
                        "}],\"context\":{\"language\":\"english\",\"country_code\":\"US\"},"
                        "\"data_request\":{\"include_trailers\":true}}";
    std::string body, webm, mp4;
    bool ok = HttpGet(L"api.steampowered.com",
                      L"/IStoreBrowseService/GetItems/v1/?input_json=" + Utf8ToWide(UrlEncode(query)), body);
    size_t p = ok ? body.find("\"microtrailer\":[") : std::string::npos;
    size_t end = p == std::string::npos ? p : body.find(']', p);
    while (p != std::string::npos && end != std::string::npos) {
        p = body.find("\"filename\":\"", p);
        if (p == std::string::npos || p > end) break;
        p += 12;
        size_t q = body.find('"', p);
        if (q == std::string::npos) break;
        std::string file = body.substr(p, q - p);
        bool safe = file.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/._-") ==
                    std::string::npos;
        if (safe && file.size() > 5 && file.compare(file.size() - 5, 5, ".webm") == 0 && webm.empty()) webm = file;
        if (safe && file.size() > 4 && file.compare(file.size() - 4, 4, ".mp4") == 0 && mp4.empty()) mp4 = file;
        p = q;
    }
    const std::string base = "https://video.akamai.steamstatic.com/store_trailers/";
    std::string msg = "{\"type\":\"trailer\",\"id\":" + std::to_string(id);
    if (!webm.empty() || !mp4.empty()) {
        msg += ",\"webm\":" + JsonStr(Utf8ToWide(webm.empty() ? "" : base + webm)) +
               ",\"mp4\":" + JsonStr(Utf8ToWide(mp4.empty() ? "" : base + mp4));
    } else {
        msg += ok ? ",\"none\":true" : ",\"error\":true";
    }
    msg += "}";
    InfoResult* r = new InfoResult{id, ok ? msg : msg + "!"};  // "!": don't cache a failure
    if (!g_hwnd || !PostMessageW(g_hwnd, WM_APP_TRAILER, 0, (LPARAM)r)) delete r;
    return 0;
}

// ---------------------------------------------------------------- controller
// View + Menu held together on an Xbox-style controller opens or closes the
// launcher. Ignored while a full-screen game has the screen.
using XInputGetState_t = DWORD(WINAPI*)(DWORD, XINPUT_STATE*);

DWORD WINAPI PadThread(LPVOID) {
    HMODULE xi = LoadLibraryW(L"xinput1_4.dll");
    if (!xi) xi = LoadLibraryW(L"xinput9_1_0.dll");
    auto getState = xi ? (XInputGetState_t)(void*)GetProcAddress(xi, "XInputGetState") : nullptr;
    if (!getState) return 0;
    bool connected[4] = {true, true, true, true};
    ULONGLONG nextProbe = 0, heldSince = 0;
    bool fired = false;
    for (;;) {
        bool on = GetSettings().gamepadOpen;
        if (WaitForSingleObject(g_stopEvent, on ? 60 : 1000) != WAIT_TIMEOUT) break;
        if (!on) continue;
        ULONGLONG now = GetTickCount64();
        // asking an empty slot is slow, so those are only checked every few seconds
        bool probe = now >= nextProbe;
        if (probe) nextProbe = now + 3000;
        bool combo = false;
        for (DWORD i = 0; i < 4; i++) {
            if (!connected[i] && !probe) continue;
            XINPUT_STATE st = {};
            connected[i] = getState(i, &st) == ERROR_SUCCESS;
            WORD b = st.Gamepad.wButtons;
            if (connected[i] && (b & XINPUT_GAMEPAD_BACK) && (b & XINPUT_GAMEPAD_START)) combo = true;
        }
        if (!combo) {
            heldSince = 0;
            fired = false;
            continue;
        }
        if (!heldSince) heldSince = now;
        if (fired || now - heldSince < 500) continue;
        fired = true;
        QUERY_USER_NOTIFICATION_STATE qs = QUNS_ACCEPTS_NOTIFICATIONS;
        HWND fg = GetForegroundWindow();
        bool ours = fg && fg == g_hwnd;
        if (!ours && SUCCEEDED(SHQueryUserNotificationState(&qs)) &&
            (qs == QUNS_RUNNING_D3D_FULL_SCREEN || qs == QUNS_BUSY || qs == QUNS_PRESENTATION_MODE)) {
            continue;
        }
        if (g_hwnd) PostMessageW(g_hwnd, WM_APP_PAD, 0, 0);
    }
    FreeLibrary(xi);
    return 0;
}

// ---------------------------------------------------------------- UI state (UI thread only)

HMODULE g_clientDll = nullptr;
ICoreWebView2Controller* g_controller = nullptr;
ICoreWebView2* g_webview = nullptr;
bool g_creating = false;
bool g_pageReady = false;
bool g_pendingShow = false;
std::wstring g_loadError;    // painted in the window when WebView2 can't start
bool g_loadErrorReported = false;
ULONGLONG g_navTick = 0;
bool g_visible = false;
std::string g_gamesJson;
std::string g_libraryExtra;
std::map<uint32_t, std::string> g_trailerCache;  // id -> message for the page
std::set<uint32_t> g_trailerLoading;
std::map<uint32_t, GameLinks> g_links;
std::set<uint32_t> g_ids;
std::map<uint32_t, std::pair<ULONGLONG, std::string>> g_newsCache;
std::set<uint32_t> g_newsLoading;
std::set<uint32_t> g_infoLoading;
UINT g_taskbarCreatedMsg = 0;
bool g_trayAdded = false;
bool g_hotkeyRegistered = false;
HICON g_trayIcon = nullptr;

void PostJson(const std::string& json) {
    if (g_webview && g_pageReady) g_webview->PostWebMessageAsJson(Utf8ToWide(json).c_str());
}

// Before the first scan finishes there is nothing to send yet; the scanner's
// result reaches the page as soon as it arrives.
void PostLibrary() {
    if (g_gamesJson.empty()) return;
    std::string app = HostSettingsJson();
    PostJson(std::string("{\"type\":\"library\",\"theme\":\"") +
             ThemeName(EffectiveTheme(GetSettings())) + "\",\"games\":" + g_gamesJson + g_libraryExtra +
             (app.empty() ? std::string() : ",\"app\":" + app) +
             ",\"steam\":" + (g_steamFound ? "true" : "false") + "}");
}

WvColor BackgroundColor() {
    return UseLightTheme(GetSettings()) ? WvColor{255, 0xed, 0xec, 0xea} : WvColor{255, 0x0c, 0x0d, 0x0f};
}

void ApplyBackground() {
    ICoreWebView2Controller2* c2 = nullptr;
    if (g_controller && SUCCEEDED(g_controller->QueryInterface(kIidController2, (void**)&c2)) && c2) {
        c2->put_DefaultBackgroundColor(BackgroundColor());
        c2->Release();
    }
    InvalidateRect(g_hwnd, nullptr, TRUE);
}

// Path of the WebView2 runtime's client DLL (installed runtime, any scope).
std::wstring FindWebView2ClientDll() {
    const wchar_t kKey[] =
        L"Software\\Microsoft\\EdgeUpdate\\ClientState\\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}";
    for (HKEY root : {HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER}) {
        for (DWORD wow : {(DWORD)RRF_SUBKEY_WOW6432KEY, (DWORD)RRF_SUBKEY_WOW6464KEY}) {
            std::wstring path = RegReadStr(root, kKey, L"EBWebView", wow);
            if (path.empty()) continue;
            std::wstring dll = NormalizePath(path) + kClientDll;
            if (FileExists(dll)) return dll;
        }
    }
    return {};
}

void OnMessageFromPage(const std::string& json);
void ShowLauncher();
void HideLauncher();

bool UiItalian() {
    return PRIMARYLANGID(GetUserDefaultUILanguage()) == LANG_ITALIAN;
}

void WebViewFailed(const std::wstring& what, HRESULT hr) {
    wchar_t code[32] = L"";
    if (hr) swprintf(code, 32, L" (0x%08X)", (unsigned)hr);
    Wh_Log(L"%s%s", what.c_str(), code);
    g_creating = false;
    g_loadError = what + code;
    if (g_hwnd) InvalidateRect(g_hwnd, nullptr, TRUE);
    if (!g_loadErrorReported) {
        g_loadErrorReported = true;
        std::wstring msg = (UiItalian() ? L"Steam Launcher non riesce ad avviare WebView2, il motore che disegna "
                                          L"la sua finestra.\n\n"
                                        : L"Steam Launcher can't start WebView2, the engine that draws its "
                                          L"window.\n\n") +
                           g_loadError +
                           (UiItalian() ? L"\n\nIl registro launcher.log nella cartella del programma ha i dettagli."
                                        : L"\n\nlauncher.log in the program's folder has the details.");
        HostReportError(msg.c_str());
    }
}

void CreateWebView() {
    if (g_controller || g_creating) return;
    std::wstring dll = FindWebView2ClientDll();
    if (dll.empty()) {
        WebViewFailed(UiItalian() ? L"WebView2 Runtime non installato. Scaricalo dal sito Microsoft (cerca "
                                    L"\"WebView2 Runtime\") e riapri il programma."
                                  : L"WebView2 Runtime isn't installed. Get it from Microsoft's website (search "
                                    L"for \"WebView2 Runtime\") and open the program again.",
                      0);
        return;
    }
    Wh_Log(L"WebView2: %s", dll.c_str());
    if (!g_clientDll) g_clientDll = LoadLibraryW(dll.c_str());
    if (!g_clientDll) {
        WebViewFailed(L"LoadLibrary " + dll, HRESULT_FROM_WIN32(GetLastError()));
        return;
    }
    auto create = g_clientDll ? (CreateEnvInternal_t)(void*)GetProcAddress(
                                    g_clientDll, "CreateWebViewEnvironmentWithOptionsInternal")
                              : nullptr;
    if (!create) {
        WebViewFailed(L"WebView2 entry point not found in " + dll, 0);
        return;
    }
    g_creating = true;
    Wh_Log(L"WebView2: creating the environment");
    std::wstring userData = g_storage + L"\\WebView2";
    SHCreateDirectoryExW(nullptr, userData.c_str(), nullptr);

    auto onController = new Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler, HRESULT,
                                     ICoreWebView2Controller*>(
        kIidControllerCompleted, [](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT {
            g_creating = false;
            if (FAILED(hr) || !controller || !g_hwnd) {
                WebViewFailed(L"WebView2 controller", FAILED(hr) ? hr : E_FAIL);
                return S_OK;
            }
            Wh_Log(L"WebView2: view ready");
            controller->AddRef();
            g_controller = controller;
            controller->get_CoreWebView2(&g_webview);
            if (!g_webview) return S_OK;
            ApplyBackground();

            ICoreWebView2Settings* settings = nullptr;
            if (SUCCEEDED(g_webview->get_Settings(&settings)) && settings) {
                settings->put_AreDefaultScriptDialogsEnabled(FALSE);
                settings->put_IsStatusBarEnabled(FALSE);
                settings->put_AreDevToolsEnabled(FALSE);
                settings->put_AreDefaultContextMenusEnabled(FALSE);
                settings->put_IsZoomControlEnabled(FALSE);
                settings->Release();
            }
            ICoreWebView2_3* wv3 = nullptr;
            if (SUCCEEDED(g_webview->QueryInterface(kIidWebView2_3, (void**)&wv3)) && wv3) {
                wv3->SetVirtualHostNameToFolderMapping(kHostName, g_uiDir.c_str(), 1 /* allow */);
                wv3->Release();
            }
            auto onMessage = new Callback<ICoreWebView2WebMessageReceivedEventHandler, ICoreWebView2*,
                                          ICoreWebView2WebMessageReceivedEventArgs*>(
                kIidMessageReceived,
                [](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                    LPWSTR json = nullptr;
                    if (args && SUCCEEDED(args->get_WebMessageAsJson(&json)) && json) {
                        OnMessageFromPage(WideToUtf8(json));
                        CoTaskMemFree(json);
                    }
                    return S_OK;
                });
            WvToken token;
            g_webview->add_WebMessageReceived(onMessage, &token);
            onMessage->Release();

            RECT rc;
            GetClientRect(g_hwnd, &rc);
            controller->put_Bounds(rc);
            controller->put_IsVisible(g_visible);
            g_pageReady = false;
            g_navTick = GetTickCount64();
            g_webview->Navigate(StartUrl().c_str());
            if (g_pendingShow) {
                g_pendingShow = false;
                ShowLauncher();
            }
            return S_OK;
        });

    auto onEnv = new Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler, HRESULT,
                              ICoreWebView2Environment*>(
        kIidEnvCompleted, [onController](HRESULT hr, ICoreWebView2Environment* env) -> HRESULT {
            Wh_Log(L"WebView2: environment answered (0x%08X)", (unsigned)hr);
            if (FAILED(hr) || !env || !g_hwnd ||
                FAILED(env->CreateCoreWebView2Controller(g_hwnd, onController))) {
                WebViewFailed(L"WebView2 environment", FAILED(hr) ? hr : E_FAIL);
            }
            onController->Release();
            return S_OK;
        });
    HRESULT hr = create(true, 0 /* installed runtime */, userData.c_str(), nullptr, onEnv);
    onEnv->Release();
    if (FAILED(hr)) {
        WebViewFailed(L"WebView2 creation", hr);
    }
}

// ---------------------------------------------------------------- window

void PlaceWindow() {
    POINT pt;
    GetCursorPos(&pt);
    HMONITOR mon = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = {sizeof(mi)};
    GetMonitorInfoW(mon, &mi);
    RECT r = mi.rcWork;
    DWORD corner = 1;  // don't round
    if (GetSettings().size == WindowSize::Large) {
        int w = r.right - r.left, h = r.bottom - r.top;
        int nw = w * 86 / 100, nh = h * 86 / 100;
        r.left += (w - nw) / 2;
        r.top += (h - nh) / 2;
        r.right = r.left + nw;
        r.bottom = r.top + nh;
        corner = 2;  // round
    }
    DwmSetWindowAttribute(g_hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
    SetWindowPos(g_hwnd, HWND_TOP, r.left, r.top, r.right - r.left, r.bottom - r.top,
                 SWP_NOACTIVATE);
}

// Windows only lets the foreground app hand over the focus; the hotkey and the
// tray click usually allow it, otherwise borrow the foreground thread's input.
void ForceForeground(HWND hwnd) {
    if (SetForegroundWindow(hwnd) && GetForegroundWindow() == hwnd) return;
    HWND fg = GetForegroundWindow();
    DWORD fgThread = fg ? GetWindowThreadProcessId(fg, nullptr) : 0;
    DWORD me = GetCurrentThreadId();
    if (fgThread && fgThread != me && AttachThreadInput(me, fgThread, TRUE)) {
        BringWindowToTop(hwnd);
        SetForegroundWindow(hwnd);
        AttachThreadInput(me, fgThread, FALSE);
    }
}

void ShowLauncher() {
    if (!g_hwnd) return;
    if (!g_controller) {
        // The view isn't ready yet: show the window now with a loading message,
        // so starting the program always puts something on screen.
        g_pendingShow = true;
        PlaceWindow();
        g_visible = true;
        ShowWindow(g_hwnd, SW_SHOW);
        ForceForeground(g_hwnd);
        InvalidateRect(g_hwnd, nullptr, TRUE);
        CreateWebView();
        SetTimer(g_hwnd, kTimerWatchdog, 20000, nullptr);
        return;
    }
    KillTimer(g_hwnd, kTimerWatchdog);
    // The page never answered (renderer crash, no network for the libraries
    // on the very first start): load it again.
    if (!g_pageReady && g_webview && GetTickCount64() - g_navTick > 15000) {
        g_navTick = GetTickCount64();
        g_webview->Reload();
    }
    KillTimer(g_hwnd, kTimerHide);
    ApplyBackground();
    PlaceWindow();
    g_visible = true;
    ShowWindow(g_hwnd, SW_SHOW);
    ForceForeground(g_hwnd);
    RECT rc;
    GetClientRect(g_hwnd, &rc);
    g_controller->put_Bounds(rc);
    g_controller->put_IsVisible(TRUE);
    g_controller->MoveFocus(0 /* programmatic */);
    PostJson("{\"type\":\"show\"}");
    SetEvent(g_resyncEvent);  // make sure the data is fresh
}

void HideLauncher() {
    g_pendingShow = false;  // closed while loading: stay closed when the view is ready
    if (!g_visible) return;
    g_visible = false;
    KillTimer(g_hwnd, kTimerHide);
    ShowWindow(g_hwnd, SW_HIDE);
    // A hidden view stops rendering, so the page uses no CPU in the background.
    if (g_controller) g_controller->put_IsVisible(FALSE);
}

void Open(const std::wstring& target) {
    ShellExecuteW(nullptr, L"open", target.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

void OnMessageFromPage(const std::string& json) {
    std::map<std::string, std::string> m;
    JsonReader reader(json);
    if (!reader.ReadTopLevel(m)) return;
    const std::string& type = m["type"];
    uint32_t id = IsNumeric(m["id"]) ? (uint32_t)strtoul(m["id"].c_str(), nullptr, 10) : 0;
    Settings s = GetSettings();

    if (type == "ready") {
        Wh_Log(L"Page ready");
        g_pageReady = true;
        PostLibrary();
        if (g_visible) PostJson("{\"type\":\"show\"}");
    } else if (type == "close") {
        HideLauncher();
    } else if (type == "launch" && g_links.count(id)) {
        Open(L"steam://rungameid/" + std::to_wstring(id));
        if (s.hideAfterLaunch) SetTimer(g_hwnd, kTimerHide, 700, nullptr);
    } else if (type == "install" && g_ids.count(id) && !g_links.count(id)) {
        Open(L"steam://install/" + std::to_wstring(id));
        HideLauncher();
    } else if (type == "theme") {
        Theme t = ParseTheme(Utf8ToWide(m["value"]));
        g_themeOverride = (LONG)t;
        Wh_SetStringValue(L"uiTheme", Utf8ToWide(ThemeName(t)).c_str());
        ApplyBackground();
    } else if (type == "store" && id) {
        Open(L"steam://store/" + std::to_wstring(id));
        HideLauncher();
    } else if (type == "folder" && g_links.count(id)) {
        const GameLinks& l = g_links[id];
        const std::string& which = m["which"];
        const std::wstring& dir = which == "saves" ? l.saves : which == "shots" ? l.shots : l.dir;
        if (!dir.empty() && DirExists(dir)) {
            Open(dir);
            HideLauncher();
        }
    } else if (type == "url") {
        // Only web pages (news articles) are opened from the page.
        std::wstring url = Utf8ToWide(m["url"]);
        if (url.rfind(L"https://", 0) == 0 && url.find_first_of(L" \"<>") == std::wstring::npos) {
            Open(url);
            HideLauncher();
        }
    } else if (type == "setting" && HostCanEditSettings()) {
        HostSetSetting(Utf8ToWide(m["key"]), Utf8ToWide(m["value"]));
    } else if (type == "quit" && HostHasQuit()) {
        HostQuit();
    } else if (type == "info" && id) {
        if (!s.showRatings) {
            PostJson("{\"type\":\"info\",\"id\":" + std::to_string(id) + ",\"disabled\":true}");
            return;
        }
        if (g_infoLoading.insert(id).second) {
            HANDLE t = CreateThread(nullptr, 0, InfoThread, (LPVOID)(UINT_PTR)id, 0, nullptr);
            if (t) CloseHandle(t);
            else g_infoLoading.erase(id);
        }
    } else if (type == "trailer" && id && g_ids.count(id)) {
        auto it = g_trailerCache.find(id);
        if (it != g_trailerCache.end()) {
            PostJson(it->second);
        } else if (g_trailerLoading.insert(id).second) {
            HANDLE t = CreateThread(nullptr, 0, TrailerThread, (LPVOID)(UINT_PTR)id, 0, nullptr);
            if (t) CloseHandle(t);
            else g_trailerLoading.erase(id);
        }
    } else if (type == "news" && id) {
        if (!s.showNews) {
            PostJson("{\"type\":\"news\",\"id\":" + std::to_string(id) + ",\"disabled\":true}");
            return;
        }
        auto it = g_newsCache.find(id);
        if (it != g_newsCache.end() && GetTickCount64() - it->second.first < 30 * 60 * 1000) {
            PostJson("{\"type\":\"news\",\"id\":" + std::to_string(id) + ",\"data\":" + it->second.second + "}");
            return;
        }
        if (g_newsLoading.insert(id).second) {
            HANDLE t = CreateThread(nullptr, 0, NewsThread, (LPVOID)(UINT_PTR)id, 0, nullptr);
            if (t) CloseHandle(t);
            else g_newsLoading.erase(id);
        }
    }
}

// ---------------------------------------------------------------- hotkey and tray

bool ParseHotkey(const std::wstring& text, UINT& mods, UINT& vk) {
    mods = 0;
    vk = 0;
    std::wstring t = Lower(text), part;
    std::vector<std::wstring> parts;
    for (wchar_t c : t + L"+") {
        if (c == L'+') {
            while (!part.empty() && part.back() == L' ') part.pop_back();
            size_t b = part.find_first_not_of(L' ');
            if (b != std::wstring::npos) parts.push_back(part.substr(b));
            part.clear();
        } else {
            part += c;
        }
    }
    for (const auto& p : parts) {
        if (p == L"ctrl" || p == L"control") mods |= MOD_CONTROL;
        else if (p == L"alt") mods |= MOD_ALT;
        else if (p == L"shift") mods |= MOD_SHIFT;
        else if (p == L"win" || p == L"windows") mods |= MOD_WIN;
        else if (p.size() == 1 && ((p[0] >= L'a' && p[0] <= L'z') || (p[0] >= L'0' && p[0] <= L'9')))
            vk = towupper(p[0]);
        else if (p.size() >= 2 && p[0] == L'f' && iswdigit(p[1])) {
            int n = _wtoi(p.c_str() + 1);
            if (n >= 1 && n <= 24) vk = VK_F1 + n - 1;
        } else if (p == L"space") vk = VK_SPACE;
        else if (p == L"enter") vk = VK_RETURN;
        else if (p == L"home") vk = VK_HOME;
        else if (p == L"end") vk = VK_END;
        else if (p == L"insert" || p == L"ins") vk = VK_INSERT;
        else if (p == L"pause") vk = VK_PAUSE;
        else return false;
    }
    return vk != 0;
}

void ApplyHotkey() {
    if (g_hotkeyRegistered) {
        UnregisterHotKey(g_hwnd, kHotkeyId);
        g_hotkeyRegistered = false;
    }
    std::wstring text = GetSettings().hotkey;
    if (text.empty()) return;
    UINT mods, vk;
    if (!ParseHotkey(text, mods, vk)) {
        Wh_Log(L"Invalid hotkey: %s", text.c_str());
        return;
    }
    g_hotkeyRegistered = RegisterHotKey(g_hwnd, kHotkeyId, mods | MOD_NOREPEAT, vk) != FALSE;
    if (!g_hotkeyRegistered) Wh_Log(L"Hotkey %s is used by another program", text.c_str());
}

void ApplyTray() {
    NOTIFYICONDATAW nid = {sizeof(nid)};
    nid.hWnd = g_hwnd;
    nid.uID = kTrayId;
    bool want = GetSettings().trayIcon;
    if (!want) {
        if (g_trayAdded) Shell_NotifyIconW(NIM_DELETE, &nid);
        g_trayAdded = false;
        return;
    }
    if (!g_trayIcon) g_trayIcon = HostTrayIcon();
    if (!g_trayIcon) {
        std::wstring steam = GetSteamPath();
        if (!steam.empty()) ExtractIconExW((steam + L"\\steam.exe").c_str(), 0, nullptr, &g_trayIcon, 1);
        if (!g_trayIcon) g_trayIcon = LoadIconW(nullptr, IDI_APPLICATION);
    }
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_APP_TRAY;
    nid.hIcon = g_trayIcon;
    wcscpy_s(nid.szTip, L"Steam Launcher");
    g_trayAdded = Shell_NotifyIconW(g_trayAdded ? NIM_MODIFY : NIM_ADD, &nid) != FALSE;
}

void ShowTrayMenu() {
    bool it = PRIMARYLANGID(GetUserDefaultUILanguage()) == LANG_ITALIAN;
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_STRING, kMenuOpen, it ? L"Apri il launcher" : L"Open launcher");
    AppendMenuW(menu, MF_STRING, kMenuRefresh, it ? L"Aggiorna la libreria" : L"Refresh library");
    if (HostHasQuit()) {
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(menu, MF_STRING, kMenuQuit, it ? L"Esci" : L"Exit");
    }
    SetMenuDefaultItem(menu, kMenuOpen, FALSE);
    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(g_hwnd);
    UINT cmd = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_NONOTIFY, pt.x, pt.y, 0,
                              g_hwnd, nullptr);
    DestroyMenu(menu);
    if (cmd == kMenuOpen) ShowLauncher();
    else if (cmd == kMenuQuit) HostQuit();
    else if (cmd == kMenuRefresh) {
        if (g_webview && g_pageReady) g_webview->Reload();
        SetEvent(g_resyncEvent);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (g_taskbarCreatedMsg && msg == g_taskbarCreatedMsg) {
        g_trayAdded = false;  // Explorer restarted: add the icon again
        ApplyTray();
        return 0;
    }
    switch (msg) {
        case WM_HOTKEY:
            if (wp == kHotkeyId) {
                if (g_visible && GetForegroundWindow() == hwnd) HideLauncher();
                else ShowLauncher();
            }
            return 0;
        case WM_APP_TRAY:
            if (lp == WM_LBUTTONUP) {
                if (g_visible) HideLauncher();
                else ShowLauncher();
            } else if (lp == WM_RBUTTONUP) {
                ShowTrayMenu();
            }
            return 0;
        case WM_APP_SHOW:
            ShowLauncher();
            return 0;
        case WM_APP_LIBRARY: {
            LibraryData* lib = (LibraryData*)lp;
            g_gamesJson = std::move(lib->json);
            g_libraryExtra = std::move(lib->extra);
            g_links = std::move(lib->links);
            g_ids = std::move(lib->ids);
            delete lib;
            PostLibrary();
            return 0;
        }
        case WM_APP_INFO_MSG: {
            InfoResult* r = (InfoResult*)lp;
            g_infoLoading.erase(r->id);
            PostJson("{\"type\":\"info\",\"id\":" + std::to_string(r->id) + "," + r->json.substr(1));
            delete r;
            return 0;
        }
        case WM_APP_TRAILER: {
            InfoResult* r = (InfoResult*)lp;
            g_trailerLoading.erase(r->id);
            std::string msg = r->json;
            if (!msg.empty() && msg.back() == '!') msg.pop_back();
            else g_trailerCache[r->id] = msg;
            PostJson(msg);
            delete r;
            return 0;
        }
        case WM_APP_PAD:
            if (g_visible && GetForegroundWindow() == hwnd) HideLauncher();
            else ShowLauncher();
            return 0;
        case WM_APP_NEWS: {
            NewsResult* r = (NewsResult*)lp;
            g_newsLoading.erase(r->id);
            std::string idStr = std::to_string(r->id);
            if (r->ok) {
                g_newsCache[r->id] = {GetTickCount64(), r->body};
                PostJson("{\"type\":\"news\",\"id\":" + idStr + ",\"data\":" + r->body + "}");
            } else {
                PostJson("{\"type\":\"news\",\"id\":" + idStr + ",\"error\":true}");
            }
            delete r;
            return 0;
        }
        case WM_APP_SETTINGS:
            ApplyHotkey();
            ApplyTray();
            ApplyBackground();
            PostLibrary();  // the theme may have changed
            if (g_visible) PlaceWindow();
            return 0;
        case WM_TIMER:
            if (wp == kTimerPreload) {
                KillTimer(hwnd, kTimerPreload);
                CreateWebView();
            } else if (wp == kTimerHide) {
                HideLauncher();
            } else if (wp == kTimerWatchdog) {
                KillTimer(hwnd, kTimerWatchdog);
                if (!g_controller) {
                    WebViewFailed(UiItalian() ? L"WebView2 non ha risposto entro 20 secondi."
                                              : L"WebView2 didn't answer within 20 seconds.",
                                  0);
                }
            }
            return 0;
        case WM_KEYDOWN:
            if (wp == VK_ESCAPE) HideLauncher();  // while the loading message is up
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hwnd, &ps);
            if (!g_controller || !g_pageReady) {
                RECT rc;
                GetClientRect(hwnd, &rc);
                bool light = UseLightTheme(GetSettings());
                SetBkMode(dc, TRANSPARENT);
                int dpi = GetDpiForWindow(hwnd);
                HFONT big = CreateFontW(-MulDiv(26, dpi, 96), 0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
                                        CLEARTYPE_QUALITY, 0, L"Segoe UI Variable Display");
                HFONT small = CreateFontW(-MulDiv(13, dpi, 96), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
                                          CLEARTYPE_QUALITY, 0, L"Segoe UI");
                RECT t = rc;
                t.bottom = (rc.top + rc.bottom) / 2;
                HGDIOBJ old = SelectObject(dc, big);
                SetTextColor(dc, light ? RGB(19, 20, 22) : RGB(236, 231, 223));
                DrawTextW(dc, L"Steam Launcher", -1, &t, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
                SelectObject(dc, small);
                RECT b = rc;
                b.top = t.bottom + MulDiv(12, dpi, 96);
                b.left += MulDiv(40, dpi, 96);
                b.right -= MulDiv(40, dpi, 96);
                SetTextColor(dc, g_loadError.empty() ? RGB(140, 138, 134) : RGB(228, 106, 85));
                std::wstring line = !g_loadError.empty() ? g_loadError
                                    : UiItalian()       ? L"Caricamento…"
                                                        : L"Loading…";
                DrawTextW(dc, line.c_str(), -1, &b, DT_CENTER | DT_TOP | DT_WORDBREAK);
                SelectObject(dc, old);
                DeleteObject(big);
                DeleteObject(small);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_ACTIVATE:
            if (LOWORD(wp) == WA_INACTIVE && g_visible && GetSettings().hideOnFocusLoss) HideLauncher();
            return 0;
        case WM_SIZE:
            if (g_controller) {
                RECT rc;
                GetClientRect(hwnd, &rc);
                g_controller->put_Bounds(rc);
            }
            return 0;
        case WM_MOVE:
            if (g_controller) g_controller->NotifyParentWindowPositionChanged();
            return 0;
        case WM_SETTINGCHANGE:
            if (lp && wcscmp((LPCWSTR)lp, L"ImmersiveColorSet") == 0) ApplyBackground();
            break;
        case WM_ERASEBKGND: {
            RECT rc;
            GetClientRect(hwnd, &rc);
            WvColor c = BackgroundColor();
            HBRUSH b = CreateSolidBrush(RGB(c.R, c.G, c.B));
            FillRect((HDC)wp, &rc, b);
            DeleteObject(b);
            return 1;
        }
        case WM_CLOSE:
            HideLauncher();
            return 0;
        case WM_APP_STOP:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

// Writes the page to the storage folder when it changed.
void WritePage() {
    SHCreateDirectoryExW(nullptr, g_uiDir.c_str(), nullptr);
    std::string html(kIndexHtml);
    std::string current;
    std::wstring path = g_uiDir + L"\\index.html";
    if (!ReadFileBytes(path, current) || current != html) WriteFileBytes(path, html);
}

DWORD WINAPI UiThread(LPVOID) {
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    HRESULT hrCo = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    WritePage();

    WNDCLASSEXW wc = {sizeof(wc)};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = LAUNCHER_WINDOW_CLASS;
    RegisterClassExW(&wc);
    g_hwnd = CreateWindowExW(WS_EX_TOOLWINDOW, wc.lpszClassName, L"Steam Launcher", WS_POPUP, 0, 0,
                             800, 600, nullptr, nullptr, wc.hInstance, nullptr);
    SetEvent(g_uiReady);
    if (!g_hwnd) {
        if (SUCCEEDED(hrCo)) CoUninitialize();
        return 1;
    }
    g_taskbarCreatedMsg = RegisterWindowMessageW(L"TaskbarCreated");
    ApplyHotkey();
    ApplyTray();
    // Load the view in the background a little after sign-in, so the first
    // open is instant.
    SetTimer(g_hwnd, kTimerPreload, 8000, nullptr);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (g_controller) {
        g_controller->Close();
        g_controller->Release();
        g_controller = nullptr;
    }
    if (g_webview) {
        g_webview->Release();
        g_webview = nullptr;
    }
    if (g_trayAdded) {
        NOTIFYICONDATAW nid = {sizeof(nid)};
        nid.hWnd = g_hwnd;
        nid.uID = kTrayId;
        Shell_NotifyIconW(NIM_DELETE, &nid);
    }
    g_hwnd = nullptr;
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    if (SUCCEEDED(hrCo)) CoUninitialize();
    return 0;
}

}  // namespace

BOOL WhTool_ModInit() {
    LoadSettings();
    g_storage = StorageFolder();
    if (g_storage.empty()) {
        Wh_Log(L"No storage folder");
        return FALSE;
    }
    g_uiDir = g_storage + L"\\ui";
    WCHAR saved[16] = {};
    if (Wh_GetStringValue(L"uiTheme", saved, ARRAYSIZE(saved)) && saved[0]) {
        g_themeOverride = (LONG)ParseTheme(saved);
    }
    g_stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    g_resyncEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_uiReady = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!g_stopEvent || !g_resyncEvent || !g_uiReady) return FALSE;
    g_uiThread = CreateThread(nullptr, 0, UiThread, nullptr, 0, nullptr);
    if (!g_uiThread) return FALSE;
    WaitForSingleObject(g_uiReady, 10000);
    g_artEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if (g_artEvent) g_artThread = CreateThread(nullptr, 0, ArtThread, nullptr, 0, nullptr);
    g_scanThread = CreateThread(nullptr, 0, ScanThread, nullptr, 0, nullptr);
    g_padThread = CreateThread(nullptr, 0, PadThread, nullptr, 0, nullptr);
    return g_scanThread != nullptr;
}

void WhTool_ModUninit() {
    if (g_stopEvent) SetEvent(g_stopEvent);
    if (g_scanThread) {
        WaitForSingleObject(g_scanThread, 10000);
        CloseHandle(g_scanThread);
        g_scanThread = nullptr;
    }
    if (g_padThread) {
        WaitForSingleObject(g_padThread, 5000);
        CloseHandle(g_padThread);
        g_padThread = nullptr;
    }
    if (g_artThread) {
        WaitForSingleObject(g_artThread, 10000);
        CloseHandle(g_artThread);
        g_artThread = nullptr;
    }
    if (g_uiThread) {
        if (g_hwnd) PostMessageW(g_hwnd, WM_APP_STOP, 0, 0);
        WaitForSingleObject(g_uiThread, 10000);
        CloseHandle(g_uiThread);
        g_uiThread = nullptr;
    }
}

void CoreSettingsChanged(bool resetTheme) {
    LoadSettings();
    if (resetTheme) {
        // A theme chosen in the mod settings replaces the one picked in the launcher.
        g_themeOverride = -1;
        Wh_SetStringValue(L"uiTheme", L"");
    }
    if (g_hwnd) PostMessageW(g_hwnd, WM_APP_SETTINGS, 0, 0);
    SetEvent(g_resyncEvent);
}

void WhTool_ModSettingsChanged() {
    CoreSettingsChanged(true);
}
// @@CORE-END

////////////////////////////////////////////////////////////////////////////////
// Windhawk mod host

bool HostCanEditSettings() { return false; }
void HostSetSetting(const std::wstring&, const std::wstring&) {}
std::string HostSettingsJson() { return {}; }
bool HostHasQuit() { return false; }
void HostQuit() {}
HICON HostTrayIcon() { return nullptr; }
void HostReportError(const wchar_t*) {}

////////////////////////////////////////////////////////////////////////////////
// Windhawk tool mod implementation for mods which don't need to inject to other
// processes or hook other functions. Context:
// https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process
//
// The mod will load and run in a dedicated windhawk.exe process.

bool g_isToolModProcessLauncher;
HANDLE g_toolModProcessMutex;

void WINAPI EntryPoint_Hook() {
    Wh_Log(L">");
    ExitThread(0);
}

BOOL Wh_ModInit() {
    DWORD sessionId;
    if (ProcessIdToSessionId(GetCurrentProcessId(), &sessionId) && sessionId == 0) {
        return FALSE;
    }

    bool isExcluded = false;
    bool isToolModProcess = false;
    bool isCurrentToolModProcess = false;
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
    if (!argv) {
        Wh_Log(L"CommandLineToArgvW failed");
        return FALSE;
    }

    for (int i = 1; i < argc; i++) {
        if (wcscmp(argv[i], L"-service") == 0 || wcscmp(argv[i], L"-service-start") == 0 ||
            wcscmp(argv[i], L"-service-stop") == 0) {
            isExcluded = true;
            break;
        }
    }

    for (int i = 1; i < argc - 1; i++) {
        if (wcscmp(argv[i], L"-tool-mod") == 0) {
            isToolModProcess = true;
            if (wcscmp(argv[i + 1], WH_MOD_ID) == 0) {
                isCurrentToolModProcess = true;
            }
            break;
        }
    }

    LocalFree(argv);

    if (isExcluded) {
        return FALSE;
    }

    if (isCurrentToolModProcess) {
        g_toolModProcessMutex = CreateMutex(nullptr, TRUE, L"windhawk-tool-mod_" WH_MOD_ID);
        if (!g_toolModProcessMutex) {
            Wh_Log(L"CreateMutex failed");
            ExitProcess(1);
        }

        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            Wh_Log(L"Tool mod already running (%s)", WH_MOD_ID);
            ExitProcess(1);
        }

        if (!WhTool_ModInit()) {
            ExitProcess(1);
        }

        IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)GetModuleHandle(nullptr);
        IMAGE_NT_HEADERS* ntHeaders = (IMAGE_NT_HEADERS*)((BYTE*)dosHeader + dosHeader->e_lfanew);

        DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;
        void* entryPoint = (BYTE*)dosHeader + entryPointRVA;

        Wh_SetFunctionHook(entryPoint, (void*)EntryPoint_Hook, nullptr);
        return TRUE;
    }

    if (isToolModProcess) {
        return FALSE;
    }

    g_isToolModProcessLauncher = true;
    return TRUE;
}

void Wh_ModAfterInit() {
    if (!g_isToolModProcessLauncher) {
        return;
    }

    WCHAR currentProcessPath[MAX_PATH];
    switch (GetModuleFileName(nullptr, currentProcessPath, ARRAYSIZE(currentProcessPath))) {
        case 0:
        case ARRAYSIZE(currentProcessPath):
            Wh_Log(L"GetModuleFileName failed");
            return;
    }

    WCHAR commandLine[MAX_PATH + 2 + (sizeof(L" -tool-mod \"" WH_MOD_ID "\"") / sizeof(WCHAR)) - 1];
    swprintf_s(commandLine, L"\"%s\" -tool-mod \"%s\"", currentProcessPath, WH_MOD_ID);

    HMODULE kernelModule = GetModuleHandle(L"kernelbase.dll");
    if (!kernelModule) {
        kernelModule = GetModuleHandle(L"kernel32.dll");
        if (!kernelModule) {
            Wh_Log(L"No kernelbase.dll/kernel32.dll");
            return;
        }
    }

    using CreateProcessInternalW_t = BOOL(WINAPI*)(
        HANDLE hUserToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
        WINBOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment,
        LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation, PHANDLE hRestrictedUserToken);
    CreateProcessInternalW_t pCreateProcessInternalW =
        (CreateProcessInternalW_t)GetProcAddress(kernelModule, "CreateProcessInternalW");
    if (!pCreateProcessInternalW) {
        Wh_Log(L"No CreateProcessInternalW");
        return;
    }

    STARTUPINFO si{
        .cb = sizeof(STARTUPINFO),
        .dwFlags = STARTF_FORCEOFFFEEDBACK,
    };
    PROCESS_INFORMATION pi;
    if (!pCreateProcessInternalW(nullptr, currentProcessPath, commandLine, nullptr, nullptr, FALSE,
                                 NORMAL_PRIORITY_CLASS, nullptr, nullptr, &si, &pi, nullptr)) {
        Wh_Log(L"CreateProcess failed");
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void Wh_ModSettingsChanged() {
    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModSettingsChanged();
}

void Wh_ModUninit() {
    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModUninit();
    ExitProcess(0);
}
