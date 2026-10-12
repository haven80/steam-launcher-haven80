////////////////////////////////////////////////////////////////////////////////
// Standalone program host
//
// Runs the shared launcher as a normal Windows program: one instance, an icon
// in the notification area with "Exit", optional start with Windows, and
// settings that the launcher edits itself (saved to an INI file).

#include <cstdarg>
#include <cwchar>

namespace standalone {

std::wstring g_iniPath;      // %APPDATA%\Steam Launcher\settings.ini
std::wstring g_dataPath;     // %LOCALAPPDATA%\Steam Launcher
std::wstring g_logPath;
HANDLE g_quitEvent = nullptr;
SRWLOCK g_logLock = SRWLOCK_INIT;

const wchar_t kRunKey[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const wchar_t kRunValue[] = L"Steam Launcher";
const wchar_t kVersion[] = L"1.0.4";

struct Setting {
    const wchar_t* key;
    const wchar_t* fallback;
    bool isBool;
};
// Same names and defaults as the Windhawk mod's settings.
const Setting kSettings[] = {
    {L"hotkey", L"Ctrl+Alt+G", false},     {L"trayIcon", L"1", true},
    {L"windowSize", L"full", false},       {L"hideOnFocusLoss", L"1", true},
    {L"hideAfterLaunch", L"1", true},      {L"theme", L"auto", false},
    {L"downloadArt", L"1", true},          {L"showRatings", L"1", true},
    {L"igdbClientId", L"", false},         {L"igdbClientSecret", L"", false},
    {L"showNews", L"1", true},             {L"excludedAppIds", L"228980", false},
};

const Setting* FindSetting(const std::wstring& key) {
    for (const auto& s : kSettings) {
        if (key == s.key) return &s;
    }
    return nullptr;
}

std::wstring ReadSetting(PCWSTR key) {
    const Setting* s = FindSetting(key);
    wchar_t buf[2048];
    GetPrivateProfileStringW(L"settings", key, s ? s->fallback : L"", buf, ARRAYSIZE(buf), g_iniPath.c_str());
    return buf;
}

std::wstring ExePath() {
    wchar_t path[MAX_PATH * 2];
    DWORD n = GetModuleFileNameW(nullptr, path, ARRAYSIZE(path));
    return n && n < ARRAYSIZE(path) ? std::wstring(path, n) : std::wstring();
}

std::wstring RunCommand() {
    return L"\"" + ExePath() + L"\" --background";
}

bool StartsWithWindows() {
    wchar_t buf[MAX_PATH * 2 + 32];
    DWORD size = sizeof(buf);
    if (RegGetValueW(HKEY_CURRENT_USER, kRunKey, kRunValue, RRF_RT_REG_SZ, nullptr, buf, &size) != ERROR_SUCCESS) {
        return false;
    }
    // Only counts when it starts this copy of the program.
    return _wcsicmp(buf, RunCommand().c_str()) == 0;
}

void SetStartWithWindows(bool on) {
    HKEY key;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, kRunKey, 0, nullptr, 0, KEY_SET_VALUE, nullptr, &key, nullptr) !=
        ERROR_SUCCESS) {
        return;
    }
    if (on) {
        std::wstring cmd = RunCommand();
        RegSetValueExW(key, kRunValue, 0, REG_SZ, (const BYTE*)cmd.c_str(), (DWORD)((cmd.size() + 1) * sizeof(wchar_t)));
    } else {
        RegDeleteValueW(key, kRunValue);
    }
    RegCloseKey(key);
}

std::wstring KnownFolder(REFKNOWNFOLDERID id) {
    PWSTR p = nullptr;
    std::wstring out;
    if (SUCCEEDED(SHGetKnownFolderPath(id, KF_FLAG_CREATE, nullptr, &p)) && p) out = p;
    CoTaskMemFree(p);
    return out;
}

}  // namespace standalone

// ---------------------------------------------------------------- Windhawk functions

void Wh_Log_(const wchar_t* format, ...) {
    wchar_t msg[2048];
    va_list args;
    va_start(args, format);
    _vsnwprintf_s(msg, _TRUNCATE, format, args);
    va_end(args);
    OutputDebugStringW(msg);
    OutputDebugStringW(L"\n");
    if (standalone::g_logPath.empty()) return;
    AcquireSRWLockExclusive(&standalone::g_logLock);
    // Keep the log small: start over past 512 KB.
    WIN32_FILE_ATTRIBUTE_DATA a;
    if (GetFileAttributesExW(standalone::g_logPath.c_str(), GetFileExInfoStandard, &a) && a.nFileSizeLow > 512 * 1024) {
        DeleteFileW(standalone::g_logPath.c_str());
    }
    HANDLE h = CreateFileW(standalone::g_logPath.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, OPEN_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h != INVALID_HANDLE_VALUE) {
        SYSTEMTIME t;
        GetLocalTime(&t);
        wchar_t stamp[32];
        swprintf_s(stamp, L"%04u-%02u-%02u %02u:%02u:%02u  ", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute,
                   t.wSecond);
        std::string line = WideToUtf8(std::wstring(stamp) + msg) + "\r\n";
        DWORD written;
        WriteFile(h, line.data(), (DWORD)line.size(), &written, nullptr);
        CloseHandle(h);
    }
    ReleaseSRWLockExclusive(&standalone::g_logLock);
}

int Wh_GetIntSetting(PCWSTR valueName, ...) {
    return _wtoi(standalone::ReadSetting(valueName).c_str());
}

PCWSTR Wh_GetStringSetting(PCWSTR valueName, ...) {
    return _wcsdup(standalone::ReadSetting(valueName).c_str());
}

void Wh_FreeStringSetting(PCWSTR string) {
    free((void*)string);
}

BOOL Wh_GetModStoragePath(PWSTR pathBuffer, size_t bufferChars) {
    return wcscpy_s(pathBuffer, bufferChars, standalone::g_dataPath.c_str()) == 0;
}

size_t Wh_GetStringValue(PCWSTR valueName, PWSTR stringBuffer, size_t bufferChars) {
    return GetPrivateProfileStringW(L"state", valueName, L"", stringBuffer, (DWORD)bufferChars,
                                    standalone::g_iniPath.c_str());
}

BOOL Wh_SetStringValue(PCWSTR valueName, PCWSTR value) {
    return WritePrivateProfileStringW(L"state", valueName, value, standalone::g_iniPath.c_str());
}

// ---------------------------------------------------------------- host functions

bool HostCanEditSettings() {
    return true;
}

void HostSetSetting(const std::wstring& key, const std::wstring& value) {
    if (key == L"startWithWindows") {
        standalone::SetStartWithWindows(value == L"1");
    } else {
        const standalone::Setting* s = standalone::FindSetting(key);
        if (!s || value.size() > 1000 || value.find_first_of(L"\r\n") != std::wstring::npos) return;
        std::wstring v = s->isBool ? std::wstring(value == L"1" ? L"1" : L"0") : value;
        WritePrivateProfileStringW(L"settings", key.c_str(), v.c_str(), standalone::g_iniPath.c_str());
    }
    CoreSettingsChanged(false);
}

std::string HostSettingsJson() {
    std::string out = "{\"standalone\":true,\"version\":" + JsonStr(standalone::kVersion) +
                      ",\"startWithWindows\":" + (standalone::StartsWithWindows() ? "true" : "false");
    for (const auto& s : standalone::kSettings) {
        std::wstring v = standalone::ReadSetting(s.key);
        std::string name = WideToUtf8(s.key);
        if (name == "igdbClientSecret") {
            // never sent back to the page, only whether one is saved
            out += ",\"igdbSecretSet\":" + std::string(v.empty() ? "false" : "true");
        } else if (s.isBool) {
            out += ",\"" + name + "\":" + (v == L"0" ? "false" : "true");
        } else {
            out += ",\"" + name + "\":" + JsonStr(v);
        }
    }
    return out + "}";
}

bool HostHasQuit() {
    return true;
}

void HostQuit() {
    if (standalone::g_quitEvent) SetEvent(standalone::g_quitEvent);
}

HICON HostTrayIcon() {
    return (HICON)LoadImageW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(1), IMAGE_ICON,
                             GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
}

void HostReportError(const wchar_t* message) {
    static LONG shown = 0;
    if (InterlockedExchange(&shown, 1)) return;  // once per run
    MessageBoxW(nullptr, message, L"Steam Launcher", MB_OK | MB_ICONWARNING | MB_SETFOREGROUND);
    InterlockedExchange(&shown, 0);
}

// ---------------------------------------------------------------- crashes

// A crash leaves a line in the log and a message, instead of a silent exit.
LONG WINAPI OnCrash(EXCEPTION_POINTERS* info) {
    HMODULE self = GetModuleHandleW(nullptr);
    ULONG_PTR addr = (ULONG_PTR)info->ExceptionRecord->ExceptionAddress;
    HMODULE where = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPCWSTR)addr, &where);
    wchar_t mod[MAX_PATH] = L"?";
    if (where) GetModuleFileNameW(where, mod, MAX_PATH);
    wchar_t msg[1024];
    swprintf_s(msg, L"Steam Launcher stopped because of an error.\n\nCode 0x%08X at %s+0x%llX\n\nPlease send "
                    L"launcher.log from %%LOCALAPPDATA%%\\Steam Launcher.",
               (unsigned)info->ExceptionRecord->ExceptionCode, PathFindFileNameW(mod),
               (unsigned long long)(addr - (ULONG_PTR)(where ? where : self)));
    Wh_Log(L"CRASH: %s", msg);
    MessageBoxW(nullptr, msg, L"Steam Launcher", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
    return EXCEPTION_EXECUTE_HANDLER;
}

// ---------------------------------------------------------------- entry point

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR cmdLine, int) {
    bool background = cmdLine && wcsstr(cmdLine, L"--background");

    // One copy at a time: a second launch just opens the running one.
    HANDLE mutex = CreateMutexW(nullptr, TRUE, L"Local\\SteamLauncherStandalone");
    if (mutex && GetLastError() == ERROR_ALREADY_EXISTS) {
        HWND other = FindWindowW(LAUNCHER_WINDOW_CLASS, nullptr);
        if (other) {
            DWORD pid = 0;
            GetWindowThreadProcessId(other, &pid);
            AllowSetForegroundWindow(pid);
            if (!background) PostMessageW(other, WM_APP_SHOW, 0, 0);
        }
        return 0;
    }

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    std::wstring roaming = standalone::KnownFolder(FOLDERID_RoamingAppData);
    std::wstring local = standalone::KnownFolder(FOLDERID_LocalAppData);
    if (roaming.empty() || local.empty()) return 1;
    std::wstring config = roaming + L"\\Steam Launcher";
    standalone::g_dataPath = local + L"\\Steam Launcher";
    SHCreateDirectoryExW(nullptr, config.c_str(), nullptr);
    SHCreateDirectoryExW(nullptr, standalone::g_dataPath.c_str(), nullptr);
    standalone::g_iniPath = config + L"\\settings.ini";
    standalone::g_logPath = standalone::g_dataPath + L"\\launcher.log";

    SetUnhandledExceptionFilter(OnCrash);
    {
        OSVERSIONINFOW v = {sizeof(v)};
        auto rtlGetVersion = (LONG(WINAPI*)(OSVERSIONINFOW*))(void*)GetProcAddress(GetModuleHandleW(L"ntdll.dll"),
                                                                                 "RtlGetVersion");
        if (rtlGetVersion) rtlGetVersion(&v);
        std::wstring steam = GetSteamPath();
        Wh_Log(L"Steam Launcher %s starting on Windows %lu.%lu build %lu%s; Steam: %s", standalone::kVersion,
               v.dwMajorVersion, v.dwMinorVersion, v.dwBuildNumber, background ? L" (background)" : L"",
               steam.empty() ? L"not found" : steam.c_str());
    }
    standalone::g_quitEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!standalone::g_quitEvent || !WhTool_ModInit()) {
        MessageBoxW(nullptr, L"Steam Launcher couldn't start. Details are in launcher.log in "
                             L"%LOCALAPPDATA%\\Steam Launcher.", L"Steam Launcher", MB_OK | MB_ICONERROR);
        return 1;
    }
    // Opened by hand: show the launcher right away. At sign-in it waits in the tray.
    if (!background && g_hwnd) PostMessageW(g_hwnd, WM_APP_SHOW, 0, 0);

    Wh_Log(L"Started");
    // Wait for "Exit". If the window's thread ends on its own, say so instead of
    // staying in memory without a window.
    HANDLE waits[2] = {standalone::g_quitEvent, g_uiThread};
    if (WaitForMultipleObjects(2, waits, FALSE, INFINITE) == WAIT_OBJECT_0 + 1) {
        DWORD code = 0;
        GetExitCodeThread(g_uiThread, &code);
        Wh_Log(L"The window thread ended unexpectedly (code %lu)", code);
        MessageBoxW(nullptr, L"Steam Launcher's window closed unexpectedly. Details are in launcher.log in "
                             L"%LOCALAPPDATA%\\Steam Launcher.", L"Steam Launcher", MB_OK | MB_ICONERROR);
    }
    Wh_Log(L"Exiting");
    WhTool_ModUninit();
    if (mutex) CloseHandle(mutex);
    return 0;
}
