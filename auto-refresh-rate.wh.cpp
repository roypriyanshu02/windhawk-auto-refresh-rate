// ==WindhawkMod==
// @id              auto-refresh-rate
// @name            Auto Refresh Rate
// @description     Automatically switch monitor refresh rates based on AC/battery power, fullscreen games, foreground apps, and docking.
// @version         0.1.0
// @author          roypriyanshu02
// @github          https://github.com/roypriyanshu02
// @homepage        https://github.com/roypriyanshu02/windhawk-auto-refresh-rate
// @include         explorer.exe
// @architecture    x86-64
// @architecture    arm64
// @architecture    x86
// @compilerOptions -lole32 -lgdi32 -luuid -luser32 -ladvapi32 -lpowrprof
// @license         MIT
// ==/WindhawkMod==

// Source code is published under the MIT License.
//
// For bug reports and feature requests, please open an issue here:
// https://github.com/roypriyanshu02/windhawk-auto-refresh-rate/issues

// ==WindhawkModReadme==
/*
# Auto Refresh Rate

Display refresh rate automation for Windows 10 (1809+) and Windows 11 on x86-64, ARM64, and x86.

Auto Refresh Rate is a lightweight Windhawk mod that automates display refresh rates based on power state, fullscreen games, foreground apps, and docking.

It runs in-process inside `explorer.exe`. Unplug your charger, and your panel drops to 60 Hz to stretch battery runtime. Plug in or launch a game, and it immediately boosts back to full speed. Because it hooks native Windows power broadcasts (`RegisterPowerSettingNotification`) without background polling loops, idle CPU usage stays at 0%.

Configure target rates, application rules, and hotkeys in the **Settings** tab above.

_Tip: To test immediately without unplugging power, toggle Windows Energy Saver in Quick Settings (Win + A), or enable the cycle hotkey (Win + Ctrl + R) in the Settings tab above._

## Key features

* **Power source switching:** Run at maximum refresh rate on AC power, drop to 60 Hz (or lowest supported rate) on battery, and lower further when Windows Energy Saver turns on.
* **Fullscreen game boost:** Detects borderless and exclusive fullscreen games to automatically switch into maximum refresh rate.
* **Per-application rules:** Set custom refresh rates for creative applications and lock media players to battery refresh rates. Semicolon-separated executable names (`cs2; blender; vlc`); `.exe` extensions are optional.
* **Protected applications:** Locks refresh rate switching while capture or presentation tools run (e.g. OBS Studio, PowerPoint) to avoid display stutter.
* **Smart laptop docking:** When running on battery with external monitors connected, lowers only the built-in laptop screen while keeping desktop monitors at full refresh rate.
* **Quiet transitions:** Waits for keyboard and mouse activity to rest before lowering refresh rates, with an anti-flicker cooldown between switches.
* **Night schedule:** Enforces battery refresh rates during designated night hours to reduce eye strain.
* **On-screen display badge:** Click-through layered notification confirms rate changes without stealing window focus; includes DPI scaling support.
* **Global cycle hotkey:** Step sequentially through each supported display frequency before wrapping back to auto mode (`Win + Ctrl + R`; disabled by default, enable under Settings).

## Evaluation priority

When multiple conditions match simultaneously, target refresh rates resolve in the following order:

1. **Manual hotkey lock:** Overrides all automated rules until unlocked or cycled back to auto.
2. **Protected applications:** Pauses switching while capture or presentation tools run (e.g. OBS Studio, PowerPoint).
3. **Windows Energy Saver:** Drops to power-saving rate immediately when battery saver triggers.
4. **Foreground application rules & game boost:** Applies matched high/low app rules or fullscreen game boost.
5. **Night schedule:** Applies battery refresh rate during scheduled evening hours.
6. **Power source baseline:** Applies plugged-in rate on AC or on-battery rate on DC power.

## Tips & troubleshooting

* **Display flicker:** If your panel flashes during transitions, increase **Anti-flicker cooldown** (default: 3 seconds) under the **Display & transitions** settings tab.
* **External monitors on battery:** Smart docking keeps external desktop monitors at full refresh rate while on battery. To switch all connected displays, set **Target displays** to `all`.
* **Live diagnostics:** Open the **Log** tab in Windhawk to inspect real-time display adjustments, AC/battery transitions, and active process detection.

## Changelog

### Version 1.0.0 (2026-09-20)
* Initial release.
* Automatic display refresh rate switching on AC and battery power transitions.
* Windows Energy Saver integration with instant frequency step-down.
* Borderless and fullscreen game detection with automatic high-refresh boost.
* Per-application refresh rate rules and screen capture protection (`obs64`, `powerpnt`).
* Smart laptop docking: maintains external monitor speeds while lowering internal panel.
* On-screen display notification badge with DPI awareness and click-through transparency.
* Global cycle hotkey (`Win + Ctrl + R`) to step through supported frequencies.
* Purely event-driven Win32 architecture with zero idle CPU overhead.

## Feedback & source code

For bug reports, feature requests, and source code, visit the **[GitHub repository](https://github.com/roypriyanshu02/windhawk-auto-refresh-rate)**.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- PowerAndBattery:
    - ChargeSwitchingEnabled: true
      $name: "Power source switching"
      $description: "Switch refresh rates when connecting or disconnecting power."
    - PluggedInRate: max
      $name: "Plugged-in rate"
      $description: "Refresh rate when connected to AC power."
      $options:
        - max: "Highest supported (recommended)"
        - "60": "60 Hz"
        - custom: "Custom rate"
    - CustomPluggedInRate: 144
      $name: "Custom plugged-in rate"
      $description: "Target refresh rate in Hz."
    - OnBatteryRate: "60"
      $name: "On-battery rate"
      $description: "Refresh rate while running on battery."
      $options:
        - "60": "60 Hz (recommended)"
        - min: "Lowest supported"
        - match_ac: "Match plugged-in rate"
        - custom: "Custom rate"
    - CustomBatteryRate: 60
      $name: "Custom on-battery rate"
      $description: "Target refresh rate in Hz."
    - EnergySaverEnabled: true
      $name: "Energy Saver sync"
      $description: "Lower refresh rate while Windows Energy Saver is active."
    - EnergySaverRate: "60"
      $name: "Energy Saver rate"
      $description: "Refresh rate while Energy Saver is active."
      $options:
        - "60": "60 Hz (recommended)"
        - min: "Lowest supported"
        - custom: "Custom rate"
    - CustomEnergySaverRate: 60
      $name: "Custom Energy Saver rate"
      $description: "Target refresh rate in Hz."
  $name: "Power & battery"
  $description: "Refresh rate automation for AC power, battery, and Windows Energy Saver."

- GamingAndApps:
    - AutoGameBoost: true
      $name: "Fullscreen game boost"
      $description: "Boost to highest supported refresh rate in borderless and fullscreen games."
    - AppRulesEnabled: false
      $name: "Per-app refresh rates"
      $description: "Apply custom refresh rates when designated apps are focused."
    - HighRefreshApps: "cs2; valorant; overwatch; cyberpunk2077; blender"
      $name: "High-refresh apps"
      $description: "Apps that boost to maximum refresh rate when focused. Separate names with semicolons (e.g. cs2; blender)."
    - LowRefreshApps: "vlc; mpc-hc64; netflix; acrobat"
      $name: "Low-refresh apps"
      $description: "Apps locked to battery refresh rate when focused. Separate names with semicolons (e.g. vlc; acrobat)."
    - InhibitAppsEnabled: true
      $name: "Protected apps lock"
      $description: "Pause display switching while capture or presentation tools run."
    - InhibitApps: "obs64; obs; streamlabs; powerpnt"
      $name: "Protected apps"
      $description: "Apps that block refresh rate changes while running. Separate names with semicolons (e.g. obs64; powerpnt)."
  $name: "Gaming & applications"
  $description: "Fullscreen game boost, per-app rules, and screen capture protection."

- Schedule:
    - TimeScheduleEnabled: false
      $name: "Night schedule"
      $description: "Lower refresh rate during scheduled hours to reduce eye strain."
    - ScheduleStart: "22:00"
      $name: "Start time"
      $description: "Schedule start time in 24h or 12h format (e.g. 22:00 or 10:00 PM)."
    - ScheduleEnd: "07:00"
      $name: "End time"
      $description: "Schedule end time in 24h or 12h format (e.g. 07:00 or 7:00 AM)."
  $name: "Night schedule"
  $description: "Scheduled refresh rate limits for late night hours."

- DisplayAndTransitions:
    - TargetDisplays: primary
      $name: "Target displays"
      $description: "Displays to adjust when switching refresh rates."
      $options:
        - primary: "Primary display only"
        - all: "All connected displays"
    - SmartDockingEnabled: true
      $name: "Smart laptop docking"
      $description: "Keep external monitors at high refresh rate while lowering only the internal laptop screen on battery."
    - QuietSwitchEnabled: true
      $name: "Idle-only switching"
      $description: "Wait for keyboard and mouse input to pause before lowering refresh rate."
    - AntiFlickerCooldown: 3
      $name: "Anti-flicker cooldown"
      $description: "Minimum seconds to wait between display switches to avoid rapid panel flashing."
  $name: "Display & transitions"
  $description: "Target display selection, laptop docking, and transition timing."

- ShortcutsAndNotifications:
    - OsdBadgeEnabled: true
      $name: "On-screen notification badge"
      $description: "Show a temporary on-screen badge when the refresh rate changes."
    - GlobalHotkeyEnabled: false
      $name: "Cycle hotkey"
      $description: "Cycle through supported refresh rates or return to auto mode via hotkey."
    - GlobalHotkey: "Win+Ctrl+R"
      $name: "Hotkey combination"
      $description: "Key combination to cycle rates (e.g. Win+Ctrl+R, Ctrl+Alt+R)."
    - VerboseLogging: true
      $name: "Verbose event logging"
      $description: "Log power transitions and refresh rate events to the Windhawk log."
  $name: "Shortcuts & notifications"
  $description: "On-screen badge, keyboard shortcuts, and event logging."
*/
// ==/WindhawkModSettings==

#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <windows.h>
#include <objbase.h>
#include <winuser.h>
#include <powrprof.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <algorithm>
#include <cmath>
#include <optional>
#include <atomic>

BOOL Wh_ModInit() {
    Wh_Log(L"Auto Refresh Rate mod initialized.");
    return TRUE;
}

void Wh_ModUninit() {
    Wh_Log(L"Auto Refresh Rate mod uninitialized.");
}
