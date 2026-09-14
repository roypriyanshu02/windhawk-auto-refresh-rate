# Contributing to Auto Refresh Rate

Guidelines for improving the display engine, maintaining shell stability, and submitting pull requests.

Please review our [Code of Conduct](CODE_OF_CONDUCT.md) before participating.

---

## 1. Golden Rules

Because Auto Refresh Rate runs in-process inside `explorer.exe`, all contributions must follow these four architectural constraints:

### Zero polling loops
Never introduce timer loops or periodic `SetTimer` checks that wake the CPU while idle. All transitions must be purely event-driven:
* **Power transitions:** `WM_POWERBROADCAST` (`RegisterPowerSettingNotification`)
* **Window switches:** `WinEventProc` (`EVENT_SYSTEM_FOREGROUND`)
* **Display topology:** `WM_DISPLAYCHANGE`

Timers are only permitted for one-shot deferred events (such as anti-flicker cooldowns). Idle CPU usage must remain at 0.0%.

### Backward-compatible settings schema
Never rename, remove, or re-order existing YAML keys or option identifiers in `// ==WindhawkModSettings==`. Doing so corrupts saved user configurations and triggers silent resets upon mod updates.

### Shell safety & RAII
Any unhandled exception or leaked resource can destabilize the user's desktop shell. Always manage Win32 handles, registry keys, and GDI objects through existing RAII wrappers (`ScopedHandle`, `ScopedGdiObject`, `ScopedRegKey`, `ScopedDcState`).

### Version bumping & metadata integrity
Windhawk relies on metadata headers to identify updates and render settings:
* Whenever functional logic or settings change, increment `@version` in `// ==WindhawkMod==` (e.g. `1.0.0` → `1.0.1`).
* Add a concise bullet under `### Version X.Y.Z` in the `// ==WindhawkModReadme==` changelog.
* Preserve single-file integrity: metadata, in-app documentation, YAML settings, and code must remain within `auto-refresh-rate.wh.cpp`.

---

## 2. Codebase Orientation

| File | Description |
| :--- | :--- |
| `auto-refresh-rate.wh.cpp` | Single-source engine: metadata, settings schema, in-app readme, and Win32 hooks. |
| `README.md` | Primary project documentation and installation guide. |
| `CONTRIBUTING.md` | Architecture rules, local development setup, and PR guidelines. |
| `SECURITY.md` | Threat model, vulnerability disclosure, and audit scope. |
| `LICENSE` | MIT license terms. |

---

## 3. Development Setup

### Prerequisites
* Windows 10 (1809+) or Windows 11 with a high-refresh-rate panel (e.g. 60 Hz and 120 Hz+).
* [Windhawk](https://windhawk.net/) installed.

### Local Mod Workflow
Auto Refresh Rate is maintained as a single-source C++ mod compiled directly through the Windhawk client:

1. Open Windhawk and navigate to **Advanced** → **Create Local Mod**.
2. Set the Mod ID to `auto-refresh-rate`.
3. Paste the contents of [`auto-refresh-rate.wh.cpp`](auto-refresh-rate.wh.cpp) into the editor.
4. Verify the required linker libraries are declared in the metadata header:
   ```text
   -lole32 -lgdi32 -luuid -luser32 -ladvapi32 -lpowrprof
   ```
5. Click **Compile and run**.
6. Switch to the **Log** tab to inspect live transition diagnostics.

---

## 4. Testing & Verification Checklist

Before opening a pull request, verify each scenario on your hardware:

- [ ] **Idle CPU:** In Task Manager, confirm `explorer.exe` remains at 0.0% CPU usage while the desktop sits idle.
- [ ] **AC/DC transitions:** Unplug the charger to verify the display drops to battery frequency, then reconnect power to confirm it boosts back to maximum rate.
- [ ] **Energy Saver:** Toggle Windows Energy Saver (`Win + A`) and confirm the panel steps down immediately.
- [ ] **Game boost:** Launch a borderless or exclusive fullscreen game to confirm the panel locks to maximum refresh rate.
- [ ] **Protected apps:** Launch a configured tool (`obs64.exe` or `powerpnt.exe`) and confirm display switching is paused.
- [ ] **Cycle hotkey:** Enable the hotkey under Settings, then press `Win + Ctrl + R` to cycle sequentially through supported display rates and wrap back to auto mode.
- [ ] **Version bump:** Incremented `@version` in `// ==WindhawkMod==` and updated the in-file changelog.

---

## 5. Submitting a Pull Request

1. Fork the repository and create a descriptive feature branch:
   ```bash
   git checkout -b feat/custom-timing-fallback
   ```
2. Commit your changes using [Conventional Commits](https://www.conventionalcommits.org/):
   ```bash
   git commit -m "feat(display): add fallback for non-standard timings"
   ```
3. Push to your fork and open a Pull Request against `main` describing your changes, test verification notes, and hardware environment (GPU, monitor model, Windows build).
4. Once merged, updates are synchronized to the official [Windhawk Mods Catalog](https://github.com/ramensoftware/windhawk-mods) (`mods/auto-refresh-rate.wh.cpp`).
