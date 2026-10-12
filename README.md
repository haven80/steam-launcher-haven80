# Steam Launcher
![PREVIEW](https://i.imgur.com/f07Sf7h.jpeg)

A minimal, animated launcher for your Steam library: covers on a WebGL strip
or as 3D game cases, search as you type, playtime, Steam reviews, Metacritic,
time to beat and news, and a showroom mode that presents your games on its own.
Not affiliated with Valve.
![PREVIEW2](https://i.imgur.com/xwR6r2W.png)

The same launcher ships two ways, built from one set of sources:

| | Standalone program | Windhawk mod |
|---|---|---|
| File | `SteamLauncher.exe` | `steam-launcher.wh.cpp` |
| Status | 1.0.3 | experimental |
| Settings | inside the launcher (sliders button → Program) | Windhawk's mod settings |
| Starts | with Windows, if you turn it on | with Windhawk |
| Needs | the WebView2 Runtime (preinstalled on Windows 10 and 11) | Windhawk |
![PREVIEW3](https://i.imgur.com/hyMRPCs.jpeg)

Download both from the [Releases](../../releases) page. Run only one of the
two: both use **Ctrl+Alt+G** by default, and the second one can't register it.

## Features

- Installed and not installed games, from Steam's own files: no account or API
  key needed.
- Two styles: a curved strip of covers, or 3D game cases with the title on the
  spine and a reflection on the floor.
- Search as you type, sort by recent, most played or name.
- Game details: playtime, last session, size on disk, Steam reviews, Metacritic,
  time to beat (IGDB, optional), and the latest news from Steam.
- Effects, each one can be turned off: spherical lens, reflection, dust, glow,
  tilt, the game's logo, holographic foil for games played 100+ hours, depth of
  field, light that follows the time of day, the printed back of a case, and a
  disc that slides out of its case.
- Showroom: after a while without input the library scrolls by itself.
- Light and dark theme, Italian and English.

## Using the program

Put `SteamLauncher.exe` in a folder of its own (for example
`%LOCALAPPDATA%\Programs\Steam Launcher`) and double-click it. Press **Esc** or
click outside to hide the launcher, **Ctrl+Alt+G** or the tray icon to bring it
back. Right-click the tray icon to refresh the library or exit. Opening the exe
again shows the copy that is already running.

The exe isn't code-signed, so Windows asks for confirmation the first time.
To stop it asking: right-click the exe → **Properties** → tick **Unblock** →
OK (or `Unblock-File .\SteamLauncher.exe` in PowerShell). Smart App Control, if
it's on, blocks unsigned programs and allows no exceptions.

In the launcher's settings (sliders button, top right), **Program** has: start
with Windows, the hotkey (click it and press the new combination), what closes
the launcher, the tray icon, news, ratings, cover downloads, window size, and
the IGDB key for time to beat.

| What | Where |
|---|---|
| Settings | `%APPDATA%\Steam Launcher\settings.ini` |
| Cache and log | `%LOCALAPPDATA%\Steam Launcher` (`launcher.log`) |
| Start with Windows | `HKCU\Software\Microsoft\Windows\CurrentVersion\Run`, value "Steam Launcher", started with `--background` |

If something goes wrong the program shows a message instead of closing, and
`launcher.log` records every step of the start-up.

### Time to beat

HowLongToBeat has no public API, so the launcher uses IGDB's data. To turn it
on, sign in at [dev.twitch.tv/console](https://dev.twitch.tv/console) (two-factor
authentication must be on), register an application (OAuth redirect
`http://localhost`, category "Application Integration", client type
"Confidential"), and paste its Client ID and a new Client secret in the
launcher's settings.

## Sources

```
steam-launcher.html                the interface: WebGL renderer, animations, settings
mod/steam-launcher.template.cpp    Windhawk metadata, the shared core, the Windhawk host
standalone/wh_shim.h               stand-ins for the few Windhawk functions the core uses
standalone/host.cpp                the program: INI settings, autostart, tray exit, crash report, wWinMain
standalone/resources.rc            icon, manifest (per-monitor DPI, UTF-8), version info
standalone/make_icon.py            draws app.ico
fonts/OFL-BricolageGrotesque.txt   license of the embedded title font
build.py                           builds everything into out/
```

The core sits between `// @@CORE-BEGIN` and `// @@CORE-END` in the template.
What differs between the two hosts goes through the `Host*` functions declared
at the top of the core. The program's version number is `kVersion` in
`standalone/host.cpp`; the build copies it into the exe's properties.

## Building

Needs Python 3 and MinGW-w64 (`x86_64-w64-mingw32-g++` and `windres`), on
Linux, WSL or MSYS2:

```
python3 build.py            # out/steam-launcher.wh.cpp and out/SteamLauncher.exe
python3 build.py --no-exe   # sources only
```

GitHub Actions runs the same build on every push (the files are attached to the
run) and publishes a release when you push a tag such as `v1.0.3`.

`out/SteamLauncher.cpp` is a single file, so it also builds by hand. Keep
`-fno-devirtualize`: the WebView2 interfaces are declared by hand, and older
GCC versions otherwise optimize calls into the runtime away.

```
x86_64-w64-mingw32-windres resources.rc -O coff -o resources.o
x86_64-w64-mingw32-g++ -std=c++20 -O2 -fno-devirtualize -municode -mwindows -DUNICODE -D_UNICODE ^
  SteamLauncher.cpp resources.o -o SteamLauncher.exe -static -s ^
  -lole32 -lshell32 -lshlwapi -luuid -ladvapi32 -luser32 -lgdi32 -lwinhttp -ldwmapi
```

## Network

The interface loads nothing from the internet. The launcher only talks to
Steam (news, reviews, Metacritic, missing artwork) and, if you add a key, to
Twitch/IGDB for time to beat. Each can be turned off.

## Credits

Titles use Bricolage Grotesque by Mathieu Triay, embedded under the SIL Open
Font License 1.1 (see `fonts/`).
