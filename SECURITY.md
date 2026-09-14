# Security policy

Vulnerability disclosure procedures, threat model, and privacy guarantees.

---

## Supported versions

I only patch and maintain the current major version (v1.x). Security patches are pushed to the `main` branch and published as updates. Older releases are not maintained or backported, so please keep Auto Refresh Rate up to date.

---

## Reporting a vulnerability

If you spot a security vulnerability (such as a memory corruption bug, thread deadlock, or an `explorer.exe` crash vector), please do not open a public issue. Instead, report it privately by opening a [draft security advisory](https://github.com/roypriyanshu02/windhawk-auto-refresh-rate/security/advisories/new) under the Security tab.

I will reply within 72 hours. Please give me time to patch the issue before you share it publicly. I will credit you in the release notes unless you prefer to remain anonymous.

---

## Threat model and scope

Auto Refresh Rate runs in-process inside `explorer.exe`. Because it operates within the Windows shell process, stability, isolation, and user privacy are critical.

### In-scope vulnerabilities

I track these issues as security vulnerabilities:

* **Shell crash vectors:** Malformed display strings, unhandled exceptions, or memory corruption that crashes or destabilizes `explorer.exe`.
* **Memory safety flaws:** Buffer overflows, out-of-bounds reads during app list parsing (`HighRefreshApps`, `InhibitApps`), or use-after-free bugs in display event hooks.
* **Input and screen capture violations:** Any regression or change that inspects, records, or logs screen pixel contents, window buffers, or arbitrary user keystrokes beyond the registered global hotkey.
* **Resource deadlocks:** Thread locks, unbounded message loops, or unreleased GDI handles that freeze the desktop window manager or UI thread.

### Out-of-scope issues

I do not track these issues as security vulnerabilities:

* Vulnerabilities in Windhawk itself or the Windows kernel/WDDM graphics drivers.
* Hardware-specific panel flicker during resolution changes (use `AntiFlickerCooldown` to mitigate).
* Standard functional bugs, config typos, or non-security display switching edge cases (please report these via [GitHub Issues](https://github.com/roypriyanshu02/windhawk-auto-refresh-rate/issues)).

---

## Privacy and local execution

Auto Refresh Rate operates strictly on your local machine with zero telemetry:

* **Zero network activity:** The mod does not link against network libraries (`wininet.dll`, `ws2_32.dll`) and performs zero outbound network requests.
* **Zero telemetry:** No analytics, diagnostic tracking, or user telemetry is collected or transmitted.
* **In-memory state:** Configuration is stored solely via native Windhawk settings, and runtime state is maintained purely in process memory.
