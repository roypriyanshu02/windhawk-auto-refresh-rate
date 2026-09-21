# Auto Refresh Rate

*Display refresh rate automation for Windows 10 and 11.*

[![Windhawk Mod](https://img.shields.io/badge/windhawk-mod-black?style=flat-square)](https://windhawk.net/mods/auto-refresh-rate)
[![Version: 1.0.0](https://img.shields.io/badge/version-1.0.0-33ab12?style=flat-square)](https://github.com/roypriyanshu02/windhawk-auto-refresh-rate/releases)
![Supported Architectures](https://img.shields.io/badge/arch-x86--64%20%7C%20arm64%20%7C%20x86-555?style=flat-square)

Auto Refresh Rate is a lightweight Windhawk mod that automates display refresh rates based on power state, fullscreen games, foreground apps, and docking.

It runs in-process inside `explorer.exe`. Unplug your charger, and your panel drops to 60 Hz to stretch battery runtime. Plug in or launch a game, and it immediately boosts back to full speed. Because it hooks native Windows power broadcasts (`RegisterPowerSettingNotification`) without background polling loops, idle CPU usage stays at 0%.

## Installation

### From the Windhawk Mod Manager
1. Install directly from the [Windhawk Mod Catalog](https://windhawk.net/mods/auto-refresh-rate) or search for **Auto Refresh Rate** in the Windhawk client.
2. Click **Details** → **Install**.

### Manual / Local Mod
1. Install [Windhawk](https://windhawk.net/) if not already installed.
2. In Windhawk, navigate to **Advanced** → **Create Local Mod**.
3. Copy and paste the source from [`auto-refresh-rate.wh.cpp`](auto-refresh-rate.wh.cpp).
4. Click **Compile and run**.

_Tip: To test immediately without unplugging power, toggle Windows Energy Saver in Quick Settings (Win + A), or enable the cycle hotkey (Win + Ctrl + R) under Settings._

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

## How it works

Auto Refresh Rate runs in-process inside `explorer.exe` and hooks native Windows event notifications without background polling:

* **Power subsystems:** Listens to `GUID_ACDC_POWER_SOURCE` and `GUID_POWER_SAVING_STATUS` via `RegisterPowerSettingNotification`.
* **Window events:** Detects active applications and fullscreen games via `SetWinEventHook` (`EVENT_SYSTEM_FOREGROUND`).
* **Display configuration:** Queries topologies via `QueryDisplayConfig` and adjusts frequencies using `ChangeDisplaySettingsEx`.
* **Docking detection:** Inspects connector types (`DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL`) to differentiate built-in laptop panels from external desktop monitors.

### Evaluation priority

When multiple rules match simultaneously, target refresh rates resolve in order:

1. **Manual hotkey lock:** Overrides automated rules until unlocked or cycled back to auto.
2. **Protected applications:** Pauses switching while capture or presentation tools run (e.g. OBS Studio, PowerPoint).
3. **Windows Energy Saver:** Drops to power-saving rate immediately when battery saver triggers.
4. **Foreground apps & game boost:** Applies matched high/low app rules or fullscreen game boost.
5. **Night schedule:** Applies battery refresh rate during scheduled evening hours.
6. **Power source baseline:** Applies plugged-in rate on AC or on-battery rate on DC power.

## Configuration

All options can be configured interactively from the Windhawk **Settings** tab:

* **Power & battery:** Set target rates on AC, battery (`max`, `60`, `min`, `custom`), and Windows Energy Saver.
* **Gaming & applications:** Automatic boost for borderless/fullscreen games, per-app refresh rules (`cs2; blender; vlc`), and capture inhibition (`obs64; powerpnt`).
* **Display & docking:** Target primary or all displays, enable smart docking (keep external monitors at full rate on battery), and set anti-flicker switch cooldowns.
* **Shortcuts & schedule:** Global cycle hotkey (`Win + Ctrl + R`), on-screen display badge toggle, and scheduled night hours.

<details>
<summary><strong>Advanced: Full settings key reference</strong></summary>

| Setting | Default | Description |
| :--- | :--- | :--- |
| **Power & battery** | | |
| `PowerAndBattery.ChargeSwitchingEnabled` | `true` | Switch refresh rates when connecting or disconnecting power. |
| `PowerAndBattery.PluggedInRate` | `max` | Refresh rate when connected to AC power (`max`, `60`, `custom`). |
| `PowerAndBattery.CustomPluggedInRate` | `144` | Target refresh rate in Hz. |
| `PowerAndBattery.OnBatteryRate` | `60` | Refresh rate while running on battery (`60`, `min`, `match_ac`, `custom`). |
| `PowerAndBattery.CustomBatteryRate` | `60` | Target refresh rate in Hz. |
| `PowerAndBattery.EnergySaverEnabled` | `true` | Lower refresh rate while Windows Energy Saver is active. |
| `PowerAndBattery.EnergySaverRate` | `60` | Refresh rate while Energy Saver is active (`60`, `min`, `custom`). |
| `PowerAndBattery.CustomEnergySaverRate` | `60` | Target refresh rate in Hz. |
| **Gaming & applications** | | |
| `GamingAndApps.AutoGameBoost` | `true` | Boost to highest supported refresh rate in borderless and fullscreen games. |
| `GamingAndApps.AppRulesEnabled` | `false` | Apply custom refresh rates when designated apps are focused. |
| `GamingAndApps.HighRefreshApps` | `cs2; valorant; overwatch; cyberpunk2077; blender` | Apps that boost to maximum refresh rate when focused. |
| `GamingAndApps.LowRefreshApps` | `vlc; mpc-hc64; netflix; acrobat` | Apps locked to battery refresh rate when focused. |
| `GamingAndApps.InhibitAppsEnabled` | `true` | Pause display switching while capture or presentation tools run. |
| `GamingAndApps.InhibitApps` | `obs64; obs; streamlabs; powerpnt` | Apps that block refresh rate changes while running. |
| **Night schedule** | | |
| `Schedule.TimeScheduleEnabled` | `false` | Lower refresh rate during scheduled hours to reduce eye strain. |
| `Schedule.ScheduleStart` | `22:00` | Schedule start time in 24h or 12h format (e.g. `22:00` or `10:00 PM`). |
| `Schedule.ScheduleEnd` | `07:00` | Schedule end time in 24h or 12h format (e.g. `07:00` or `7:00 AM`). |
| **Display & transitions** | | |
| `DisplayAndTransitions.TargetDisplays` | `primary` | Displays to adjust when switching refresh rates (`primary` or `all`). |
| `DisplayAndTransitions.SmartDockingEnabled` | `true` | Keep external monitors at high refresh rate while lowering only the internal laptop screen on battery. |
| `DisplayAndTransitions.QuietSwitchEnabled` | `true` | Wait for keyboard and mouse input to pause before lowering refresh rate. |
| `DisplayAndTransitions.AntiFlickerCooldown` | `3` | Minimum seconds to wait between display switches to avoid rapid panel flashing. |
| **Shortcuts & notifications** | | |
| `ShortcutsAndNotifications.OsdBadgeEnabled` | `true` | Show a temporary on-screen badge when the refresh rate changes. |
| `ShortcutsAndNotifications.GlobalHotkeyEnabled` | `false` | Cycle through supported refresh rates or return to auto mode via hotkey. |
| `ShortcutsAndNotifications.GlobalHotkey` | `Win+Ctrl+R` | Key combination to cycle rates (e.g. `Win+Ctrl+R`, `Ctrl+Alt+R`). |
| `ShortcutsAndNotifications.VerboseLogging` | `true` | Log power transitions and refresh rate events to the Windhawk log. |

</details>

## Compatibility

* **Windows versions:** Windows 10 (1809+) and Windows 11 (21H2 through 24H2).
* **Architectures:** x86-64, ARM64, and x86.
* **Graphics drivers:** WDDM 2.0+ (standard across Intel, AMD, and NVIDIA drivers on Windows 10/11, including hybrid GPU and MUX laptops).
* **Display features:** Preserves HDR color metadata, G-Sync, FreeSync, and Variable Refresh Rate (VRR) ranges.
* **Modern apps:** Tracks UWP and Windows Store applications hosted inside `ApplicationFrameHost.exe`.
* **Sleep and wake:** Re-synchronizes display state after system resume (`PBT_APMRESUME`).

## Troubleshooting & edge cases

* **Hybrid graphics & MUX switches (Advanced Optimus):** Display paths re-enumerate when switching between integrated and discrete GPUs. The mod hooks `WM_DISPLAYCHANGE` to re-synchronize active monitor handles automatically.
* **Custom resolutions & overclocked refresh rates:** Modes created in NVIDIA Control Panel, AMD Software, or CRU (Custom Resolution Utility) are enumerated as valid display targets and can be selected in Settings.
* **Multi-monitor setups:** By default, only the primary monitor is modified (`TargetDisplays: primary`). Switch to `all` to synchronize all monitors, or leave `Smart laptop docking` enabled to preserve external display speeds while on battery.
* **Flicker reduction:** If your display flashes during rate changes, increase `AntiFlickerCooldown` (default: 3 seconds) under **Display & transitions**.
* **Live diagnostics:** Check the **Log** tab in Windhawk to inspect real-time display adjustments, AC/battery transitions, and active process detection.

## Contributing

To report bugs, propose new heuristics, or contribute display engine improvements, see the **[Contributing Guide](CONTRIBUTING.md)**.

## License

[MIT](LICENSE)
