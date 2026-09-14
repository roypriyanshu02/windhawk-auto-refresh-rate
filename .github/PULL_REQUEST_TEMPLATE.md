## Description

<!-- Provide a concise summary of the changes and the problem being solved. -->

Fixes # <!-- (or Closes #) -->

## Type of Change

- [ ] Bug fix (non-breaking fix for an issue)
- [ ] New feature (non-breaking addition to settings or display logic)
- [ ] Performance / optimization (reduces latency or avoids redundant Win32 API calls)
- [ ] Documentation update (improves README, settings, or guidelines)
- [ ] Breaking change (modifies settings schema or default behavior)

## Technical Checklist

### Architecture & Shell Safety
- [ ] **Zero-polling verification:** No periodic timer loops or busy polling introduced. State changes remain purely event-driven (`WM_POWERBROADCAST`, `WinEventProc`, `WM_DISPLAYCHANGE`).
- [ ] **Idle CPU:** Verified 0.0% CPU usage in Task Manager when the desktop sits idle.
- [ ] **Resource hygiene:** All Win32 handles, registry keys, and GDI objects use RAII wrappers (`ScopedHandle`, `ScopedGdiObject`, `ScopedRegKey`, `ScopedDcState`).
- [ ] **Settings schema backward compatibility:** No existing YAML keys or option values in `// ==WindhawkModSettings==` were removed or renamed.
- [ ] **Memory safety:** No unchecked pointer dereferences or unbounded string buffers.

### Windhawk & Commit Standards
- [ ] Single-source integrity: all changes remain self-contained in `auto-refresh-rate.wh.cpp`.
- [ ] Incremented `@version` in `// ==WindhawkMod==` and updated the in-file changelog.
- [ ] Commits follow [Conventional Commits](https://www.conventionalcommits.org/) (e.g. `feat(display): add fallback for non-standard timings`).

## Testing Performed

- [ ] AC to battery transition (and battery to AC)
- [ ] Windows Energy Saver toggle (`Win + A`)
- [ ] Fullscreen game boost detection
- [ ] Global hotkey cycle (`Win + Ctrl + R`)
- [ ] Multi-monitor / smart docking (if applicable)

**Environment tested:**
- **Windows build:** (e.g. Windows 11 23H2)
- **Windhawk version:** (e.g. 1.5.1)
- **Display setup:** (e.g. 165 Hz internal panel, 144 Hz external via USB-C)
- **GPU & driver:** (e.g. NVIDIA RTX 4060 Laptop Driver 551.86 / Intel Iris Xe)
