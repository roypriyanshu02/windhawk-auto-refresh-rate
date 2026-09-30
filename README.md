<h1 align="center">Auto Refresh Rate</h1>
<p align="center">
  <em>Display refresh rate automation for Windows 10 and 11.</em>
</p>
<p align="center">
  <a href="https://windhawk.net/mods/auto-refresh-rate"><img src="https://img.shields.io/badge/windhawk-mod-black?style=flat-square" alt="Windhawk Mod"></a>
  <a href="https://github.com/roypriyanshu02/windhawk-auto-refresh-rate/releases"><img src="https://img.shields.io/badge/version-1.0.0-33ab12?style=flat-square" alt="Version 1.0.0"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-MIT-0284c7?style=flat-square" alt="License"></a>
  <img src="https://img.shields.io/badge/arch-x86--64%20%7C%20arm64%20%7C%20x86-555?style=flat-square" alt="Architectures">
</p>
<p align="center">
  <a href="#how-it-works">How it works</a> ·
  <a href="#evaluation-priority">Priority</a> ·
  <a href="#installation">Install</a> ·
  <a href="#configuration">Settings</a> ·
  <a href="#compatibility">Compatibility</a> ·
  <a href="#troubleshooting">Troubleshooting</a>
</p>

---

Auto Refresh Rate automates Windows display refresh rates based on AC/battery power, fullscreen games, focused apps, and laptop docking.

Unplug your charger, and your panel steps down to 60 Hz to stretch battery runtime. Launch a game or plug back in, and it immediately restores full panel speed. Because it hooks native Windows power and window events without polling loops, idle CPU usage stays at 0.0%.

## How it works

Auto Refresh Rate runs as an isolated background tool process via `windhawk.exe`. Operating outside `explorer.exe` ensures display configuration API calls cannot hang the Windows shell:

* **Power Events:** Subscribes to `GUID_ACDC_POWER_SOURCE` and `GUID_POWER_SAVING_STATUS` via `RegisterPowerSettingNotification`. Zero polling.
* **Game & App Detection:** Hooks `EVENT_SYSTEM_FOREGROUND` via `SetWinEventHook` to detect active processes and exclusive fullscreen games.
* **Mode Switching:** Queries display topologies via `QueryDisplayConfig` and applies rate changes through `ChangeDisplaySettingsEx`.
* **Docking Logic:** Inspects connector types (`DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL`) to differentiate laptop screens from external desktop monitors.

## Evaluation priority

When multiple rules match at once, display frequency resolves in strict order:

1. **Manual Hotkey Lock** — `Win + Ctrl + R` override active
2. **Protected Apps** — Inhibit tools running (`obs64`, `discord`)
3. **Windows Energy Saver** — Drops to configured battery-saver rate
4. **Foreground App / Game** — App rules (`blender`, `resolve`) or fullscreen 3D boost
5. **Night Schedule** — Evening low-Hz comfort window
6. **Power Baseline** — AC (Panel Max) vs Battery (60 Hz)

## Installation

### From Windhawk Catalog (Recommended)
1. Search for **Auto Refresh Rate** in the [Windhawk](https://windhawk.net/) client or visit the [catalog page](https://windhawk.net/mods/auto-refresh-rate).
2. Click **Details** → **Install**.

### Local Development Mod
1. Open Windhawk → **Advanced** → **Create Local Mod**.
2. Paste the contents of [`auto-refresh-rate.wh.cpp`](auto-refresh-rate.wh.cpp).
3. Click **Compile and run**.

_Quick test: Press `Win + A` and toggle Windows Energy Saver to watch the display frequency change instantly._

## Configuration

All settings can be configured interactively from the Windhawk **Settings** tab:

<details open>
<summary><strong>Full settings reference</strong></summary>

| Setting | Default | Description |
| :--- | :--- | :--- |
| **Power & battery** | | |
| `PowerAndBattery.ChargeSwitchingEnabled` | `true` | Switch refresh rates on AC connect / disconnect. |
| `PowerAndBattery.PluggedInRate` | `max` | Plugged-in rate (`max`, `60`, `custom`). |
| `PowerAndBattery.CustomPluggedInRate` | `144` | Custom AC target in Hz. |
| `PowerAndBattery.OnBatteryRate` | `60` | On-battery rate (`60`, `min`, `match_ac`, `custom`). |
| `PowerAndBattery.CustomBatteryRate` | `60` | Custom battery target in Hz. |
| `PowerAndBattery.EnergySaverEnabled` | `true` | Lower refresh rate when Windows Energy Saver triggers. |
| `PowerAndBattery.EnergySaverRate` | `60` | Energy saver rate (`60`, `min`, `custom`). |
| `PowerAndBattery.CustomEnergySaverRate` | `60` | Custom energy saver target in Hz. |
| **Gaming & applications** | | |
| `GamingAndApps.AutoGameBoost` | `true` | Boost to panel max in borderless and fullscreen games. |
| `GamingAndApps.AppRulesEnabled` | `false` | Apply custom refresh rates for focused apps. |
| `GamingAndApps.HighRefreshApps` | `[blender, resolve, affinity]` | Apps that boost to maximum refresh rate when focused. |
| `GamingAndApps.LowRefreshApps` | `[screenbox, netflix]` | Apps locked to battery rate when focused. |
| `GamingAndApps.InhibitAppsEnabled` | `true` | Freeze rate switching while capture or streaming tools run. |
| `GamingAndApps.InhibitApps` | `[obs64, discord]` | Apps that block rate switches while running. |
| **Night schedule** | | |
| `Schedule.TimeScheduleEnabled` | `false` | Force battery refresh rate during night hours. |
| `Schedule.ScheduleStart` | `22:00` | Start time in 24h or 12h format (`22:00` or `10:00 PM`). |
| `Schedule.ScheduleEnd` | `07:00` | End time in 24h or 12h format (`07:00` or `7:00 AM`). |
| **Display & transitions** | | |
| `DisplayAndTransitions.TargetDisplays` | `primary` | Displays to switch (`primary` or `all`). |
| `DisplayAndTransitions.SmartDockingEnabled` | `true` | Keep external monitors at max rate on battery. |
| `DisplayAndTransitions.QuietSwitchEnabled` | `true` | Wait for user input to pause before switching rates. |
| `DisplayAndTransitions.AntiFlickerCooldown` | `3` | Minimum seconds between switches to prevent panel flicker. |
| **Shortcuts & notifications** | | |
| `ShortcutsAndNotifications.NotificationEnabled` | `true` | Show native Windows notification on rate change. |
| `ShortcutsAndNotifications.GlobalHotkeyEnabled` | `false` | Enable manual cycle hotkey. |
| `ShortcutsAndNotifications.GlobalHotkey` | `Win+Ctrl+R` | Shortcut to cycle through supported refresh rates. |

</details>

## Compatibility

* **OS:** Windows 10 (1809+) and Windows 11 (21H2 through 24H2).
* **Architectures:** x86-64, ARM64, and x86.
* **Drivers:** WDDM 2.0+ across Intel, AMD, and NVIDIA (including MUX / Advanced Optimus laptops).
* **Color & Sync:** Preserves HDR color metadata, G-Sync, FreeSync, and VRR ranges.
* **Sleep / Wake:** Re-synchronizes display state on system resume (`PBT_APMRESUME`).

## Troubleshooting

* **Panel flashes on rate switch:** Increase `DisplayAndTransitions.AntiFlickerCooldown` to `4` or `5` seconds under **Display & transitions**.
* **External monitors drop to 60 Hz on battery:** Verify `DisplayAndTransitions.SmartDockingEnabled` is `true` and `TargetDisplays` is `primary`.
* **Diagnostics:** Check the **Log** tab in Windhawk for real-time AC/DC events and resolution logs.

## Changelog

### Version 1.0.0 (2026-09-30)
* Initial release.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for local setup, testing checklists, and pull request guidelines.

## License

[MIT](LICENSE)
