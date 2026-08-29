// ==WindhawkMod==
// @id              auto-refresh-rate
// @name            Auto Refresh Rate
// @description     Automatically switch monitor refresh rates based on AC/battery power, fullscreen games, foreground apps, and docking.
// @version         0.7.0
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

// GUID Definitions
static constexpr GUID GUID_NULL_LOCAL = {
    0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};
static constexpr GUID GUID_ACDC_POWER_SOURCE_LOCAL = {
    0x5D3E9A59, 0xE9D5, 0x4B00, { 0xA6, 0xBD, 0xFF, 0x34, 0xFF, 0x51, 0x65, 0x48 }
};
static constexpr GUID GUID_BATTERY_PERCENTAGE_REMAINING_LOCAL = {
    0xA7AD8041, 0xB45A, 0x4CAE, { 0x87, 0xA3, 0xEE, 0xCB, 0xB4, 0x68, 0xA9, 0xE1 }
};
static constexpr GUID GUID_POWER_SAVING_STATUS_LOCAL = {
    0xE00958C0, 0xC213, 0x4ACE, { 0xAC, 0x77, 0xFE, 0xCC, 0xED, 0x2E, 0xEE, 0xA5 }
};
static constexpr GUID GUID_POWERSCHEME_PERSONALITY_LOCAL = {
    0x245D8541, 0x3943, 0x4422, { 0xB0, 0x25, 0x13, 0xA7, 0x84, 0xF6, 0x79, 0xB7 }
};
static constexpr GUID GUID_MIN_POWER_SAVINGS_LOCAL = {
    0x8C5E7FDA, 0xE8BF, 0x4A96, { 0x9A, 0x85, 0xA6, 0xE2, 0x3A, 0x8C, 0x63, 0x5C }
};
static constexpr GUID GUID_OVERLAY_BEST_PERFORMANCE = {
    0xDED574B5, 0x45A0, 0x4F42, { 0x87, 0x37, 0x46, 0x34, 0x5C, 0x09, 0xC2, 0x38 }
};

#ifndef PBT_POWERSETTINGCHANGE
#define PBT_POWERSETTINGCHANGE 0x8013
#endif
#ifndef CDS_NORESET
#define CDS_NORESET 0x10000000
#endif
#ifndef QDC_ONLY_ACTIVE_PATHS
#define QDC_ONLY_ACTIVE_PATHS 0x00000002
#endif
#ifndef DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL
#define DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL 0x80000000
#endif
#ifndef DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EMBEDDED
#define DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EMBEDDED 11
#endif
#ifndef DISPLAYCONFIG_OUTPUT_TECHNOLOGY_UDI_EMBEDDED
#define DISPLAYCONFIG_OUTPUT_TECHNOLOGY_UDI_EMBEDDED 13
#endif
#ifndef DISPLAYCONFIG_OUTPUT_TECHNOLOGY_LVDS
#define DISPLAYCONFIG_OUTPUT_TECHNOLOGY_LVDS 6
#endif
#ifndef DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME
#define DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME 1
#endif
#ifndef EDS_ROTATEDMODE
#define EDS_ROTATEDMODE 0x00000004
#endif

// Custom window messages and timer identifiers
constexpr UINT WM_APP_REAPPLY_POWER_STATE   = WM_APP + 101;
constexpr UINT WM_APP_FOREGROUND_CHANGED    = WM_APP + 102;
constexpr UINT WM_APP_SETTINGS_CHANGED      = WM_APP + 103;

constexpr UINT_PTR TIMER_ID_POWER_DEBOUNCE      = 1;
constexpr UINT_PTR TIMER_ID_RESUME_SYNC         = 2;
constexpr UINT_PTR TIMER_ID_DISPLAY_CHANGE      = 3;
constexpr UINT_PTR TIMER_ID_TIME_CHECK          = 4;
constexpr UINT_PTR TIMER_ID_FOREGROUND_DEBOUNCE = 6;
constexpr UINT_PTR TIMER_ID_QUIET_SWITCH        = 7;
constexpr UINT_PTR TIMER_ID_COOLDOWN_SWITCH     = 8;

constexpr UINT_PTR TIMER_ID_OSD_HOLD            = 101;
constexpr UINT_PTR TIMER_ID_OSD_FADE            = 102;

constexpr int HOTKEY_ID_CYCLE                   = 0x415A;

constexpr DWORD DEBOUNCE_DELAY_MS               = 350;
constexpr DWORD RESUME_DELAY_MS                 = 1000;
constexpr DWORD DISPLAY_CHANGE_DELAY_MS         = 500;
constexpr DWORD FOREGROUND_DEBOUNCE_MS          = 100;
constexpr DWORD QUIET_SWITCH_TIMEOUT_MS         = 2000;
constexpr DWORD TIME_CHECK_INTERVAL_MS          = 30000;

// ============================================================================
// Mod Configuration & State
// ============================================================================

struct ModSettings {
    bool chargeSwitchingEnabled = true;
    DWORD targetAC = 0; // 0 = automatic maximum supported
    DWORD targetDC = 60; // 1 = lowest, 0 = match AC, or explicit Hz

    bool energySaverEnabled = true;
    std::wstring energySaverAction = L"60"; // "60", "min", or "custom"
    DWORD targetEnergySaver = 60; // if energySaverAction is custom

    bool autoGameBoost = true;
    bool appRulesEnabled = false;
    std::wstring highRefreshApps = L"cs2; valorant; overwatch; cyberpunk2077; blender";
    std::wstring lowRefreshApps = L"vlc; mpc-hc64; netflix; acrobat";
    bool smartDockingEnabled = true;

    bool quietSwitchEnabled = true;
    bool inhibitAppsEnabled = true;
    std::wstring inhibitApps = L"obs64; obs; streamlabs; powerpnt";
    DWORD switchCooldownMs = 3000;

    bool osdEnabled = true;
    DWORD osdDurationMs = 1500;
    bool globalHotkeyEnabled = false;
    std::wstring globalHotkey = L"Win+Ctrl+R";
    UINT hotkeyModifiers = MOD_WIN | MOD_CONTROL;
    UINT hotkeyVk = 'R';

    bool targetDisplayAll = false;
    bool timeScheduleEnabled = false;
    std::wstring scheduleStart = L"22:00";
    std::wstring scheduleEnd = L"07:00";
    bool verboseLogging = true;
};

struct PowerStateSnapshot {
    bool isAC = true;
    BYTE batteryPercent = 100;
    bool isBatterySaverActive = false;
    GUID powerScheme = GUID_NULL_LOCAL;
};

static ModSettings g_settings;
static PowerStateSnapshot g_state;
static std::vector<std::wstring> g_parsedHighRefreshApps;
static std::vector<std::wstring> g_parsedLowRefreshApps;
static std::vector<std::wstring> g_parsedInhibitApps;
static ULONGLONG g_lastSuccessfulSwitchTick = 0;

static HANDLE g_hMutex = nullptr;
static HANDLE g_hThread = nullptr;
static std::atomic<HWND> g_hWnd{nullptr};
static HWND g_hOsdWnd = nullptr;
static HWINEVENTHOOK g_hWinEventHook = nullptr;

static constexpr std::array<const GUID*, 4> g_powerGuids = {
    &GUID_ACDC_POWER_SOURCE_LOCAL,
    &GUID_BATTERY_PERCENTAGE_REMAINING_LOCAL,
    &GUID_POWER_SAVING_STATUS_LOCAL,
    &GUID_POWERSCHEME_PERSONALITY_LOCAL
};
static std::array<HPOWERNOTIFY, 4> g_hPowerNotify = {};

static constexpr WCHAR g_szClassName[] = L"Windhawk_AutoRefreshRate_MsgWnd";
static constexpr WCHAR g_szOsdClassName[] = L"Windhawk_AutoRefreshRate_OsdWnd";
static constexpr WCHAR g_szMutexName[] = L"Local\\Windhawk_AutoRefreshRate_PowerMonitor";

static bool g_manualOverrideActive = false;
static DWORD g_manualOverrideHz = 0;

static DWORD g_osdCurrentHz = 0;
static std::wstring g_osdCurrentReason;
static BYTE g_osdAlpha = 0;

enum class OsdState { Hidden, Holding, Fading };
static OsdState g_osdState = OsdState::Hidden;
static std::atomic<bool> g_foregroundPending{false};

void SynchronizeAndApplyPolicy(bool forceOsd = false, const std::wstring& forcedBrief = L"");
void ShowOsdBadge(DWORD hz, const std::wstring& reasonBrief);
void LoadSettings();


// ============================================================================
// On-screen display badge
// ============================================================================

typedef UINT (WINAPI *PFN_GetDpiForWindow)(HWND);

[[nodiscard]] float GetOsdDpiScale(HWND hWnd) {
    static PFN_GetDpiForWindow pfn = nullptr;
    static bool s_inited = false;
    if (!s_inited) {
        HMODULE hUser = GetModuleHandleW(L"user32.dll");
        if (hUser) {
            pfn = reinterpret_cast<PFN_GetDpiForWindow>(GetProcAddress(hUser, "GetDpiForWindow"));
        }
        s_inited = true;
    }
    if (pfn && hWnd) {
        const UINT dpi = pfn(hWnd);
        if (dpi > 0) return static_cast<float>(dpi) / 96.0f;
    }
    return 1.0f;
}

static HFONT s_hOsdFontHz = nullptr;
static HFONT s_hOsdFontSub = nullptr;
static float s_osdFontScale = 0.0f;

void CleanupOsdFonts() noexcept {
    if (s_hOsdFontHz) {
        DeleteObject(s_hOsdFontHz);
        s_hOsdFontHz = nullptr;
    }
    if (s_hOsdFontSub) {
        DeleteObject(s_hOsdFontSub);
        s_hOsdFontSub = nullptr;
    }
    s_osdFontScale = 0.0f;
}

void EnsureOsdFonts(float scale) noexcept {
    if (s_hOsdFontHz && s_hOsdFontSub && std::abs(s_osdFontScale - scale) < 0.001f) {
        return;
    }
    CleanupOsdFonts();
    s_osdFontScale = scale;
    s_hOsdFontHz = CreateFontW(static_cast<int>(20.0f * scale), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Variable Display");
    s_hOsdFontSub = CreateFontW(static_cast<int>(13.0f * scale), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Variable Text");
}

LRESULT CALLBACK OsdWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps = {};
        HDC hdc = BeginPaint(hWnd, &ps);
        if (!hdc) return 0;

        RECT rc = {};
        GetClientRect(hWnd, &rc);
        const float scale = GetOsdDpiScale(hWnd);

        HDC memDC = CreateCompatibleDC(hdc);
        if (memDC) {
            HBITMAP memBmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            if (memBmp) {
                HBITMAP hOldBmp = static_cast<HBITMAP>(SelectObject(memDC, memBmp));
                {
                    ScopedDcState dcSaver(memDC);

                    {
                        ScopedGdiObject hBgBrush(CreateSolidBrush(RGB(24, 24, 27)));
                        ScopedGdiObject hBorderPen(CreatePen(PS_SOLID, 1, RGB(63, 63, 70)));
                        ScopedDcState bgSaver(memDC);
                        SelectObject(memDC, hBgBrush.get());
                        SelectObject(memDC, hBorderPen.get());

                        const int roundCorner = static_cast<int>(24.0f * scale);
                        RoundRect(memDC, rc.left, rc.top, rc.right, rc.bottom, roundCorner, roundCorner);
                    }

                    {
                        const COLORREF accentColor = (g_osdCurrentHz >= 100) ? RGB(59, 130, 246) : RGB(16, 185, 129);
                        ScopedGdiObject hAccentBrush(CreateSolidBrush(accentColor));
                        ScopedGdiObject hAccentPen(CreatePen(PS_SOLID, 1, accentColor));
                        ScopedDcState dotSaver(memDC);
                        SelectObject(memDC, hAccentBrush.get());
                        SelectObject(memDC, hAccentPen.get());

                        const int dotLeft = static_cast<int>(14.0f * scale);
                        const int dotTop = static_cast<int>(17.0f * scale);
                        const int dotSize = static_cast<int>(12.0f * scale);
                        Ellipse(memDC, dotLeft, dotTop, dotLeft + dotSize, dotTop + dotSize);
                    }

                    SetBkMode(memDC, TRANSPARENT);
                    EnsureOsdFonts(scale);

                    if (s_hOsdFontHz) {
                        ScopedDcState fontSaver(memDC);
                        SelectObject(memDC, s_hOsdFontHz);
                        SetTextColor(memDC, RGB(244, 244, 245));

                        const std::wstring hzText = std::to_wstring(g_osdCurrentHz) + L" Hz";
                        const int hzX = static_cast<int>(34.0f * scale);
                        const int hzY = static_cast<int>(13.0f * scale);
                        TextOutW(memDC, hzX, hzY, hzText.c_str(), static_cast<int>(hzText.length()));

                        SIZE hzSize = {};
                        GetTextExtentPoint32W(memDC, hzText.c_str(), static_cast<int>(hzText.length()), &hzSize);

                        if (s_hOsdFontSub) {
                            ScopedDcState subFontSaver(memDC);
                            SelectObject(memDC, s_hOsdFontSub);
                            SetTextColor(memDC, RGB(161, 161, 170));
                            TextOutW(memDC, hzX + hzSize.cx + static_cast<int>(14.0f * scale), static_cast<int>(17.0f * scale),
                                     g_osdCurrentReason.c_str(), static_cast<int>(g_osdCurrentReason.length()));
                        }
                    }

                    BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);
                }
                SelectObject(memDC, hOldBmp);
                DeleteObject(memBmp);
            }
            DeleteDC(memDC);
        }

        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;

    case WM_MOUSEACTIVATE:
        return MA_NOACTIVATE;

    case WM_NCHITTEST:
        return HTTRANSPARENT;

    case WM_TIMER:
        if (wParam == TIMER_ID_OSD_HOLD) {
            if (g_osdState != OsdState::Holding) return 0;
            KillTimer(hWnd, TIMER_ID_OSD_HOLD);
            g_osdState = OsdState::Fading;
            SetTimer(hWnd, TIMER_ID_OSD_FADE, 20, nullptr);
            return 0;
        } else if (wParam == TIMER_ID_OSD_FADE) {
            if (g_osdState != OsdState::Fading) return 0;
            if (g_osdAlpha > 20) {
                g_osdAlpha -= 20;
                SetLayeredWindowAttributes(hWnd, 0, g_osdAlpha, LWA_ALPHA);
            } else {
                KillTimer(hWnd, TIMER_ID_OSD_FADE);
                g_osdState = OsdState::Hidden;
                g_osdAlpha = 0;
                ShowWindow(hWnd, SW_HIDE);
            }
            return 0;
        }
        break;

    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID_OSD_HOLD);
        KillTimer(hWnd, TIMER_ID_OSD_FADE);
        CleanupOsdFonts();
        g_osdState = OsdState::Hidden;
        g_hOsdWnd = nullptr;
        return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void InitializeOsdWindow(HINSTANCE hInstance) {
    WNDCLASSEXW wc = { sizeof(wc) };
    wc.lpfnWndProc = OsdWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_szOsdClassName;
    RegisterClassExW(&wc);

    HWND hOsd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        g_szOsdClassName, L"AutoRefreshRateOsdWindow", WS_POPUP, 0, 0, 260, 48, nullptr, nullptr, hInstance, nullptr);

    if (hOsd) {
        g_hOsdWnd = hOsd;
        SetLayeredWindowAttributes(hOsd, 0, 0, LWA_ALPHA);
        ShowWindow(hOsd, SW_HIDE);
    }
}

void ShowOsdBadge(DWORD hz, const std::wstring& reasonBrief) {
    if (!g_settings.osdEnabled || !g_hOsdWnd) return;

    g_osdCurrentHz = hz;
    g_osdCurrentReason = reasonBrief;

    float scale = GetOsdDpiScale(g_hOsdWnd);
    int baseWidth = 240;
    int approx = static_cast<int>(reasonBrief.length());
    if (approx > 14) baseWidth += (approx - 14) * 8;

    int width = static_cast<int>(baseWidth * scale);
    int height = static_cast<int>(48 * scale);

    static HWND s_lastOsdHwnd = nullptr;
    static int s_lastWidth = 0;
    static int s_lastHeight = 0;
    if (g_hOsdWnd != s_lastOsdHwnd || width != s_lastWidth || height != s_lastHeight) {
        s_lastOsdHwnd = g_hOsdWnd;
        s_lastWidth = width;
        s_lastHeight = height;
        int roundCorner = static_cast<int>(24 * scale);
        HRGN hRgn = CreateRoundRectRgn(0, 0, width + 1, height + 1, roundCorner, roundCorner);
        if (hRgn) {
            if (!SetWindowRgn(g_hOsdWnd, hRgn, FALSE)) {
                DeleteObject(hRgn);
            }
        }
    }

    RECT rcWork = {};
    POINT ptOrigin = { 0, 0 };
    HMONITOR hMon = MonitorFromPoint(ptOrigin, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mi = { sizeof(mi) };
    if (hMon && GetMonitorInfoW(hMon, &mi)) {
        rcWork = mi.rcWork;
    } else {
        SystemParametersInfoW(SPI_GETWORKAREA, 0, &rcWork, 0);
    }

    int margin = static_cast<int>(24 * scale);
    int x = rcWork.right - width - margin;
    int y = rcWork.top + margin;

    KillTimer(g_hOsdWnd, TIMER_ID_OSD_HOLD);
    KillTimer(g_hOsdWnd, TIMER_ID_OSD_FADE);

    g_osdState = OsdState::Holding;
    g_osdAlpha = 240;
    SetLayeredWindowAttributes(g_hOsdWnd, 0, g_osdAlpha, LWA_ALPHA);
    SetWindowPos(g_hOsdWnd, HWND_TOPMOST, x, y, width, height, SWP_NOACTIVATE | SWP_SHOWWINDOW);
    InvalidateRect(g_hOsdWnd, nullptr, TRUE);
    UpdateWindow(g_hOsdWnd);

    SetTimer(g_hOsdWnd, TIMER_ID_OSD_HOLD, g_settings.osdDurationMs, nullptr);
}


// ============================================================================
// Internal vs external display detection
// ============================================================================

static std::vector<std::pair<std::wstring, bool>> s_internalDisplayCache;
static bool s_internalCacheValid = false;

void InvalidateDisplayDeviceCache() noexcept {
    s_internalCacheValid = false;
    s_internalDisplayCache.clear();
}

[[nodiscard]] bool IsInternalDisplayDevice(const WCHAR* pDeviceName) {
    std::wstring resolvedName;
    if (!pDeviceName || !*pDeviceName) {
        DISPLAY_DEVICEW dd = { sizeof(dd) };
        for (DWORD i = 0; EnumDisplayDevicesW(nullptr, i, &dd, 0); ++i) {
            if ((dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) &&
                (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)) {
                resolvedName = dd.DeviceName;
                pDeviceName = resolvedName.c_str();
                break;
            }
        }
        if (!pDeviceName || !*pDeviceName) return true;
    }

    if (s_internalCacheValid) {
        for (const auto& entry : s_internalDisplayCache) {
            if (_wcsicmp(entry.first.c_str(), pDeviceName) == 0) {
                return entry.second;
            }
        }
    }

    UINT32 pathCount = 0, modeCount = 0;
    std::vector<DISPLAYCONFIG_PATH_INFO> paths;
    std::vector<DISPLAYCONFIG_MODE_INFO> modes;
    LONG result = ERROR_SUCCESS;

    for (int retry = 0; retry < 3; ++retry) {
        if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) != ERROR_SUCCESS || pathCount == 0) {
            return false;
        }
        paths.resize(pathCount);
        modes.resize(modeCount);
        result = QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths.data(), &modeCount, modes.data(), nullptr);
        if (result == ERROR_SUCCESS) break;
        if (result != ERROR_INSUFFICIENT_BUFFER) return false;
    }
    if (result != ERROR_SUCCESS) return false;

    bool isInternal = false;
    for (UINT32 i = 0; i < pathCount; ++i) {
        DISPLAYCONFIG_SOURCE_DEVICE_NAME sourceName = {};
        sourceName.header.type = static_cast<DISPLAYCONFIG_DEVICE_INFO_TYPE>(DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME);
        sourceName.header.size = sizeof(sourceName);
        sourceName.header.adapterId = paths[i].sourceInfo.adapterId;
        sourceName.header.id = paths[i].sourceInfo.id;

        if (DisplayConfigGetDeviceInfo(&sourceName.header) == ERROR_SUCCESS) {
            if (_wcsicmp(sourceName.viewGdiDeviceName, pDeviceName) == 0) {
                UINT32 tech = paths[i].targetInfo.outputTechnology;
                isInternal = (tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL ||
                              tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EMBEDDED ||
                              tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_UDI_EMBEDDED ||
                              tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_LVDS);
                break;
            }
        }
    }

    s_internalCacheValid = true;
    s_internalDisplayCache.emplace_back(pDeviceName, isInternal);
    return isInternal;
}


// ============================================================================
// Target refresh rate evaluation policy
// ============================================================================

[[nodiscard]] DWORD EvaluateTargetRefreshRate(const PowerStateSnapshot& state, std::wstring& outReason, std::wstring& outBrief) {
    // 1. Manual hotkey lock
    if (g_manualOverrideActive && g_manualOverrideHz > 0) {
        outBrief = L"Manual lock";
        outReason = L"Manual hotkey lock active (" + std::to_wstring(g_manualOverrideHz) + L" Hz)";
        return g_manualOverrideHz;
    }

    // 2. Protected applications
    if (g_settings.inhibitAppsEnabled) {
        std::wstring foreProc = GetForegroundProcessName();
        if (!foreProc.empty() && IsAppInList(foreProc, g_parsedInhibitApps)) {
            DWORD currentHz = GetCurrentPrimaryRefreshRate();
            outBrief = L"Protected: " + FormatAppNameForDisplay(foreProc);
            outReason = L"Protected app in focus ('" + foreProc + L"'), maintaining " + std::to_wstring(currentHz) + L" Hz";
            return currentHz;
        }

        auto matchedInhibit = CheckAppInRunningList(g_parsedInhibitApps);
        if (matchedInhibit) {
            DWORD currentHz = GetCurrentPrimaryRefreshRate();
            outBrief = L"Protected: " + FormatAppNameForDisplay(*matchedInhibit);
            outReason = L"Protected app running ('" + *matchedInhibit + L"'), maintaining " + std::to_wstring(currentHz) + L" Hz";
            return currentHz;
        }
    }

    DWORD resolvedAC = (g_settings.targetAC == 0) ? GetMaxRefreshRate() : g_settings.targetAC;
    DWORD resolvedDC = (g_settings.targetDC == 1) ? GetMinRefreshRate() : ((g_settings.targetDC == 0) ? resolvedAC : g_settings.targetDC);

    // 3. Windows Energy Saver
    if (g_settings.energySaverEnabled && state.isBatterySaverActive && g_settings.energySaverAction != L"ignore") {
        DWORD resolvedSaver = 60;
        if (g_settings.energySaverAction == L"min") {
            resolvedSaver = GetMinRefreshRate();
        } else if (g_settings.energySaverAction == L"custom") {
            resolvedSaver = (g_settings.targetEnergySaver >= 30) ? g_settings.targetEnergySaver : 60;
        }
        outBrief = L"Energy saver";
        outReason = L"Windows Energy Saver active (" + std::to_wstring(resolvedSaver) + L" Hz)";
        return resolvedSaver;
    }

    // Windows 11 Power Mode slider (Best Performance boost)
    if (g_settings.chargeSwitchingEnabled && !state.isAC && IsEqualGUID(state.powerScheme, GUID_MIN_POWER_SAVINGS_LOCAL)) {
        outBrief = L"Best performance";
        outReason = L"Windows Power Mode: Best Performance (" + std::to_wstring(resolvedAC) + L" Hz)";
        return resolvedAC;
    }

    // 4. Foreground application rules
    std::wstring foreProc;
    if (g_settings.appRulesEnabled || g_settings.autoGameBoost) {
        foreProc = GetForegroundProcessName();
    }

    if (g_settings.appRulesEnabled && !foreProc.empty()) {
        if (IsAppInList(foreProc, g_parsedLowRefreshApps)) {
            outBrief = L"App saver: " + FormatAppNameForDisplay(foreProc);
            outReason = L"Low-refresh app in focus ('" + foreProc + L"' -> " + std::to_wstring(resolvedDC) + L" Hz)";
            return resolvedDC;
        } else if (IsAppInList(foreProc, g_parsedHighRefreshApps)) {
            outBrief = L"App boost: " + FormatAppNameForDisplay(foreProc);
            outReason = L"High-refresh app in focus ('" + foreProc + L"' -> " + std::to_wstring(resolvedAC) + L" Hz)";
            return resolvedAC;
        }
    }

    // Fullscreen game boost
    if (g_settings.autoGameBoost) {
        auto fsProc = IsForegroundWindowFullscreen(foreProc);
        if (fsProc) {
            std::wstring gameName = fsProc->empty() ? L"Fullscreen" : *fsProc;
            outBrief = L"Game: " + FormatAppNameForDisplay(gameName);
            outReason = L"Fullscreen game detected ('" + gameName + L"' -> " + std::to_wstring(resolvedAC) + L" Hz)";
            return resolvedAC;
        }
    }

    // 5. Night schedule
    if (g_settings.timeScheduleEnabled) {
        if (IsCurrentTimeInSchedule(g_settings.scheduleStart, g_settings.scheduleEnd)) {
            outBrief = L"Night schedule";
            outReason = L"Night schedule active (" + std::to_wstring(resolvedDC) + L" Hz)";
            return resolvedDC;
        }
    }

    // 6. Power source baseline (AC vs battery)
    if (g_settings.chargeSwitchingEnabled) {
        if (state.isAC) {
            outBrief = L"Plugged in";
            outReason = L"AC power connected (" + std::to_wstring(resolvedAC) + L" Hz)";
            return resolvedAC;
        } else {
            outBrief = L"Battery (" + std::to_wstring(state.batteryPercent) + L"%)";
            outReason = L"Battery power (" + std::to_wstring(resolvedDC) + L" Hz, " + std::to_wstring(state.batteryPercent) + L"% remaining)";
            return resolvedDC;
        }
    }

    outBrief = L"Active";
    outReason = L"Power rules inactive, keeping current rate (" + std::to_wstring(GetCurrentPrimaryRefreshRate()) + L" Hz)";
    return GetCurrentPrimaryRefreshRate();
}


// ============================================================================
// Process Tracking & Window Inspection Helpers
// ============================================================================

[[nodiscard]] std::wstring GetProcessNameFromPID(DWORD pid) {
    if (pid <= 4) return L"";
    ScopedHandle hProc(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid));
    if (!hProc) return L"";

    WCHAR path[1024] = {};
    DWORD size = static_cast<DWORD>(std::size(path));
    if (QueryFullProcessImageNameW(hProc.get(), 0, path, &size)) {
        const WCHAR* pSlash = wcsrchr(path, L'\\');
        return pSlash ? (pSlash + 1) : path;
    }
    return L"";
}

[[nodiscard]] std::wstring GetForegroundProcessName() {
    HWND hFore = GetForegroundWindow();
    if (!hFore) return L"";

    DWORD pid = 0;
    GetWindowThreadProcessId(hFore, &pid);
    std::wstring procName = GetProcessNameFromPID(pid);

    if (_wcsicmp(procName.c_str(), L"ApplicationFrameHost.exe") == 0) {
        HWND hChild = FindWindowExW(hFore, nullptr, L"Windows.UI.Core.CoreWindow", nullptr);
        if (hChild) {
            DWORD childPid = 0;
            GetWindowThreadProcessId(hChild, &childPid);
            if (childPid != 0 && childPid != pid) {
                std::wstring childProc = GetProcessNameFromPID(childPid);
                if (!childProc.empty()) return childProc;
            }
        }
    }
    return procName;
}

static ULONGLONG s_lastInhibitCheckTick = 0;
static std::optional<std::wstring> s_cachedInhibitMatch;

[[nodiscard]] std::optional<std::wstring> CheckAppInRunningList(const std::vector<std::wstring>& list) {
    if (list.empty()) return std::nullopt;

    ULONGLONG now = GetTickCount64();
    if (s_lastInhibitCheckTick > 0 && (now - s_lastInhibitCheckTick < 4000)) {
        return s_cachedInhibitMatch;
    }
    s_lastInhibitCheckTick = now;

    ScopedHandle hSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (!hSnapshot) {
        s_cachedInhibitMatch = std::nullopt;
        return std::nullopt;
    }

    PROCESSENTRY32W pe = { sizeof(pe) };
    if (Process32FirstW(hSnapshot.get(), &pe)) {
        do {
            if (IsAppInList(pe.szExeFile, list)) {
                s_cachedInhibitMatch = pe.szExeFile;
                return s_cachedInhibitMatch;
            }
        } while (Process32NextW(hSnapshot.get(), &pe));
    }
    s_cachedInhibitMatch = std::nullopt;
    return std::nullopt;
}

[[nodiscard]] DWORD ResolveRefreshRate(const WCHAR* pDevice, DWORD targetHz, const DEVMODEW& dmCurrent, std::vector<DWORD>& outRates) {
    outRates.clear();
    DEVMODEW dmEnum = {};
    dmEnum.dmSize = sizeof(dmEnum);

    DWORD exactMatch = 0, toleranceMatch = 0, closestMatch = 0;
    int minDiff = 999999;

    for (DWORD i = 0; EnumDisplaySettingsExW(pDevice, i, &dmEnum, EDS_ROTATEDMODE); ++i) {
        if (dmEnum.dmPelsWidth == dmCurrent.dmPelsWidth &&
            dmEnum.dmPelsHeight == dmCurrent.dmPelsHeight &&
            dmEnum.dmBitsPerPel == dmCurrent.dmBitsPerPel) {

            DWORD hz = dmEnum.dmDisplayFrequency;
            if (std::find(outRates.begin(), outRates.end(), hz) == outRates.end()) {
                outRates.push_back(hz);
            }
            if (targetHz > 1) {
                if (hz == targetHz) {
                    exactMatch = hz;
                } else if (std::abs(static_cast<int>(hz) - static_cast<int>(targetHz)) <= 1 && toleranceMatch == 0) {
                    toleranceMatch = hz;
                }
                int diff = std::abs(static_cast<int>(hz) - static_cast<int>(targetHz));
                if (diff < minDiff) {
                    minDiff = diff;
                    closestMatch = hz;
                }
            }
        }
    }

    std::sort(outRates.begin(), outRates.end());
    std::vector<DWORD> dedup;
    for (DWORD r : outRates) {
        if (dedup.empty()) {
            dedup.push_back(r);
        } else {
            DWORD prev = dedup.back();
            if (std::abs(static_cast<int>(r) - static_cast<int>(prev)) <= 1) {
                dedup.back() = std::max(prev, r);
            } else {
                dedup.push_back(r);
            }
        }
    }
    outRates = std::move(dedup);

    if (outRates.empty()) return (targetHz > 1 ? targetHz : 60);

    // targetHz == 0: Highest supported refresh rate
    if (targetHz == 0) return outRates.back();

    // targetHz == 1: Lowest supported refresh rate (usually 60 Hz)
    if (targetHz == 1) return outRates.front();

    if (exactMatch != 0) return exactMatch;
    if (toleranceMatch != 0) return toleranceMatch;
    if (closestMatch != 0) return closestMatch;
    return outRates.back();
}

[[nodiscard]] std::wstring FormatRatesList(const std::vector<DWORD>& rates) {
    std::wstring s;
    for (size_t i = 0; i < rates.size(); ++i) {
        if (i > 0) s += L", ";
        s += std::to_wstring(rates[i]) + L"Hz";
    }
    return s;
}

[[nodiscard]] DWORD GetCurrentPrimaryRefreshRate() {
    DEVMODEW dm = {};
    dm.dmSize = sizeof(dm);
    if (EnumDisplaySettingsExW(nullptr, ENUM_CURRENT_SETTINGS, &dm, EDS_ROTATEDMODE)) {
        return dm.dmDisplayFrequency;
    }
    return 60;
}

[[nodiscard]] DWORD GetMaxRefreshRate(const WCHAR* pDevice = nullptr) {
    DEVMODEW dm = {};
    dm.dmSize = sizeof(dm);
    if (!EnumDisplaySettingsExW(pDevice, ENUM_CURRENT_SETTINGS, &dm, EDS_ROTATEDMODE)) return 144;
    std::vector<DWORD> rates;
    return ResolveRefreshRate(pDevice, 0, dm, rates);
}

[[nodiscard]] DWORD GetMinRefreshRate(const WCHAR* pDevice = nullptr) {
    DEVMODEW dm = {};
    dm.dmSize = sizeof(dm);
    if (!EnumDisplaySettingsExW(pDevice, ENUM_CURRENT_SETTINGS, &dm, EDS_ROTATEDMODE)) return 60;
    std::vector<DWORD> rates;
    return ResolveRefreshRate(pDevice, 1, dm, rates);
}

[[nodiscard]] std::optional<std::wstring> IsForegroundWindowFullscreen(const std::wstring& knownProc = L"") {
    HWND hFore = GetForegroundWindow();
    if (!hFore || !IsWindowVisible(hFore) || IsIconic(hFore)) return std::nullopt;

    WCHAR szClass[128] = {};
    GetClassNameW(hFore, szClass, 127);
    if (_wcsicmp(szClass, L"Progman") == 0 ||
        _wcsicmp(szClass, L"WorkerW") == 0 ||
        _wcsicmp(szClass, L"Shell_TrayWnd") == 0 ||
        _wcsicmp(szClass, L"Shell_SecondaryTrayWnd") == 0 ||
        _wcsicmp(szClass, L"CabinetWClass") == 0 ||
        _wcsicmp(szClass, L"TaskManagerWindow") == 0 ||
        _wcsicmp(szClass, L"Windows.UI.Core.CoreWindow") == 0) {
        return std::nullopt;
    }

    std::wstring proc = !knownProc.empty() ? knownProc : GetForegroundProcessName();
    if (_wcsicmp(proc.c_str(), L"LockApp.exe") == 0 ||
        _wcsicmp(proc.c_str(), L"LogonUI.exe") == 0 ||
        _wcsicmp(proc.c_str(), L"SearchHost.exe") == 0 ||
        _wcsicmp(proc.c_str(), L"explorer.exe") == 0 ||
        _wcsicmp(proc.c_str(), L"StartMenuExperienceHost.exe") == 0) {
        return std::nullopt;
    }

    HMONITOR hMon = MonitorFromWindow(hFore, MONITOR_DEFAULTTONEAREST);
    if (!hMon) return std::nullopt;

    MONITORINFO mi = { sizeof(mi) };
    if (!GetMonitorInfoW(hMon, &mi)) return std::nullopt;

    RECT rcWnd = {};
    if (!GetWindowRect(hFore, &rcWnd)) return std::nullopt;

    if (rcWnd.left <= mi.rcMonitor.left &&
        rcWnd.top <= mi.rcMonitor.top &&
        rcWnd.right >= mi.rcMonitor.right &&
        rcWnd.bottom >= mi.rcMonitor.bottom) {

        LONG style = GetWindowLongW(hFore, GWL_STYLE);
        if ((style & WS_CAPTION) != WS_CAPTION || (style & WS_POPUP)) {
            return proc;
        }
    }
    return std::nullopt;
}



// ============================================================================
// Internal vs external display detection
// ============================================================================

static std::vector<std::pair<std::wstring, bool>> s_internalDisplayCache;
static bool s_internalCacheValid = false;

void InvalidateDisplayDeviceCache() noexcept {
    s_internalCacheValid = false;
    s_internalDisplayCache.clear();
}

[[nodiscard]] bool IsInternalDisplayDevice(const WCHAR* pDeviceName) {
    std::wstring resolvedName;
    if (!pDeviceName || !*pDeviceName) {
        DISPLAY_DEVICEW dd = { sizeof(dd) };
        for (DWORD i = 0; EnumDisplayDevicesW(nullptr, i, &dd, 0); ++i) {
            if ((dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) &&
                (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)) {
                resolvedName = dd.DeviceName;
                pDeviceName = resolvedName.c_str();
                break;
            }
        }
        if (!pDeviceName || !*pDeviceName) return true;
    }

    if (s_internalCacheValid) {
        for (const auto& entry : s_internalDisplayCache) {
            if (_wcsicmp(entry.first.c_str(), pDeviceName) == 0) {
                return entry.second;
            }
        }
    }

    UINT32 pathCount = 0, modeCount = 0;
    std::vector<DISPLAYCONFIG_PATH_INFO> paths;
    std::vector<DISPLAYCONFIG_MODE_INFO> modes;
    LONG result = ERROR_SUCCESS;

    for (int retry = 0; retry < 3; ++retry) {
        if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) != ERROR_SUCCESS || pathCount == 0) {
            return false;
        }
        paths.resize(pathCount);
        modes.resize(modeCount);
        result = QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths.data(), &modeCount, modes.data(), nullptr);
        if (result == ERROR_SUCCESS) break;
        if (result != ERROR_INSUFFICIENT_BUFFER) return false;
    }
    if (result != ERROR_SUCCESS) return false;

    bool isInternal = false;
    for (UINT32 i = 0; i < pathCount; ++i) {
        DISPLAYCONFIG_SOURCE_DEVICE_NAME sourceName = {};
        sourceName.header.type = static_cast<DISPLAYCONFIG_DEVICE_INFO_TYPE>(DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME);
        sourceName.header.size = sizeof(sourceName);
        sourceName.header.adapterId = paths[i].sourceInfo.adapterId;
        sourceName.header.id = paths[i].sourceInfo.id;

        if (DisplayConfigGetDeviceInfo(&sourceName.header) == ERROR_SUCCESS) {
            if (_wcsicmp(sourceName.viewGdiDeviceName, pDeviceName) == 0) {
                UINT32 tech = paths[i].targetInfo.outputTechnology;
                isInternal = (tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL ||
                              tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EMBEDDED ||
                              tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_UDI_EMBEDDED ||
                              tech == DISPLAYCONFIG_OUTPUT_TECHNOLOGY_LVDS);
                break;
            }
        }
    }

    s_internalCacheValid = true;
    s_internalDisplayCache.emplace_back(pDeviceName, isInternal);
    return isInternal;
}


void ApplyRefreshRate(DWORD targetHz) {
    DEVMODEW dm = {};
    dm.dmSize = sizeof(dm);
    if (EnumDisplaySettingsExW(nullptr, ENUM_CURRENT_SETTINGS, &dm, EDS_ROTATEDMODE)) {
        if (dm.dmDisplayFrequency != targetHz) {
            dm.dmDisplayFrequency = targetHz;
            dm.dmFields = DM_DISPLAYFREQUENCY;
            LONG res = ChangeDisplaySettingsExW(nullptr, &dm, nullptr, CDS_UPDATEREGISTRY, nullptr);
            if (res == DISP_CHANGE_SUCCESSFUL) {
                Wh_Log(L"Primary display frequency switched to %u Hz.", targetHz);
            }
        }
    }
}
struct PowerStateSnapshot {
    bool isAC = true;
    BYTE batteryPercent = 100;
    bool isBatterySaverActive = false;
};

static std::atomic<HWND> g_hWnd{nullptr};
static HANDLE g_hThread = nullptr;

LRESULT CALLBACK WorkerWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        Wh_Log(L"Worker window created. Registering power setting notifications.");
        RegisterPowerSettingNotification(hWnd, &GUID_ACDC_POWER_SOURCE_LOCAL, DEVICE_NOTIFY_WINDOW_HANDLE);
        RegisterPowerSettingNotification(hWnd, &GUID_POWERSAVINGMODE_LOCAL, DEVICE_NOTIFY_WINDOW_HANDLE);
        return 0;
    case WM_POWERBROADCAST:
        if (wParam == PBT_POWERSETTINGCHANGE) {
            Wh_Log(L"Power broadcast received.");
        }
        return TRUE;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

DWORD WINAPI WorkerThreadProc(LPVOID lpParam) {
    HANDLE hInitEvent = static_cast<HANDLE>(lpParam);
    WNDCLASSEXW wc = { sizeof(wc) };
    wc.lpfnWndProc = WorkerWndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"AutoHzWorkerWndClass";
    RegisterClassExW(&wc);

    HWND hWnd = CreateWindowExW(0, wc.lpszClassName, L"AutoHzWorker", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, wc.hInstance, nullptr);
    g_hWnd.store(hWnd);
    SetEvent(hInitEvent);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}

BOOL Wh_ModInit() {
    HANDLE hInitEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    g_hThread = CreateThread(nullptr, 0, WorkerThreadProc, hInitEvent, 0, nullptr);
    WaitForSingleObject(hInitEvent, 5000);
    CloseHandle(hInitEvent);
    Wh_Log(L"Auto Refresh Rate worker thread started.");
    return TRUE;
}

void Wh_ModUninit() {
    HWND hWnd = g_hWnd.load();
    if (hWnd) PostMessageW(hWnd, WM_CLOSE, 0, 0);
    if (g_hThread) {
        WaitForSingleObject(g_hThread, 5000);
        CloseHandle(g_hThread);
        g_hThread = nullptr;
    }
    Wh_Log(L"Auto Refresh Rate mod uninitialized.");
}
