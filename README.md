# PassThru-SocketCAN

A **SAE J2534 PassThru adapter that is actually a networked SocketCAN bus.** Point it at a Raspberry
Pi running [socketcand](https://github.com/jakka351/Tester-Engineering-DIY) and *any* Windows J2534
application — Tester Engineering Suite, ForScan, Ford/Mazda IDS, HP Tuners, PCMFlash, … — sees it as
an ordinary PassThru device and talks to the vehicle over Wi-Fi or Ethernet.

```
 J2534 app (TES / ForScan / IDS / HP Tuners …)
   │  loads TesPassThruSocketCAN.dll  (native, stdcall J2534 04.04)
   ▼
 TesPassThruSocketCAN.dll  ── TCP 29536 ──►  Raspberry Pi
   • CAN + ISO15765 (ISO-TP in the DLL)        socketcand → can0 → vehicle
   • socketcand raw-mode client
   • config from the registry
```

It reuses the proven J2534 export skeleton from the Tester Engineering emulator stub and speaks the
**exact same socketcand protocol** as TES's own SocketCAN transport, so the Pi you built with the
[DIY guide](https://github.com/jakka351/Tester-Engineering-DIY) works unchanged.

---

## What's in here

```
PassThru-SocketCAN/
├── dll/
│   ├── TesPassThruSocketCAN.c      ← the J2534 DLL (CAN + ISO15765, ISO-TP engine, socketcand client)
│   ├── TesPassThruSocketCAN.def    ← undecorated exports (14 PassThru functions)
│   └── build.bat                   ← cl /LD build (x86)
├── config-app/
│   ├── TesSocketCANConfig.csproj   ← .NET 4.8.1 WinForms setup app (SDK-style)
│   ├── Program.cs                  ← form: host/port/iface/bitrate/listen, Test, Register/Unregister
│   └── app.manifest                ← requireAdministrator (writes HKLM)
└── README.md
```

---

## 1. Prerequisites

- A **Raspberry Pi running socketcand** on a CAN interface (`can0`), reachable over Wi-Fi or
  Ethernet. Build it with the **[Tester-Engineering-DIY Guide 01](https://github.com/jakka351/Tester-Engineering-DIY/blob/main/01-RaspberryPi-SocketCAN-socketcand.md)**.
- Windows (7–11, 64-bit is fine — the DLL is 32-bit, like virtually all J2534 apps).
- To **build**: Visual Studio 2019 Build Tools (for the C DLL, `cl`) and VS2022 / .NET SDK (for the
  WinForms app). To **just use** a release, you only need the two built files.

## 2. Build

**DLL (x86):**
```bat
cd dll
build.bat
```
Produces `dll\TesPassThruSocketCAN.dll`. (Edit the `vcvarsall.bat` path in `build.bat` if your Build
Tools live elsewhere.)

**Config app (.NET 4.8.1):**
```bat
cd config-app
msbuild TesSocketCANConfig.csproj -t:Restore,Build -p:Configuration=Release
```
Produces `config-app\bin\Release\net481\TesSocketCANConfig.exe`. (Or `dotnet build -c Release`.)

> Tip: put `TesPassThruSocketCAN.dll` next to `TesSocketCANConfig.exe` and the app auto-fills the DLL path.

## 3. Install / register

1. Run **`TesSocketCANConfig.exe`** (it elevates — it writes `HKLM`).
2. Enter the **Pi host/IP** (e.g. `10.42.0.1` for the Wi-Fi AP, or your Ethernet IP), **port** `29536`,
   **interface** `can0`, and the **CAN bus speed** (500 k for OBD-II).
3. Click **Test Connection** — it does the real socketcand handshake (`open can0` → `rawmode`) and
   confirms the Pi is reachable.
4. Click **Register Device**. `Tester Present SocketCAN` is now a J2534 device.

Any J2534 application will list **"Tester Present SocketCAN"** in its device/adapter picker. Select
it and connect as you would any PassThru interface.

> **In TES specifically** you don't need this shim — TES has a native SocketCAN transport. This DLL
> is for every *other* J2534 app, and for using the Pi as a universal shop adapter.

## 4. How it works

- On `PassThruOpen`, the DLL reads its target from the registry, opens a TCP socket to socketcand,
  and enters raw mode (`< open can0 >`, `< rawmode >`). A background thread pumps `< frame … >`
  messages off the socket.
- **CAN (protocol 5):** raw frames in/out, filtered by the app's PASS/BLOCK filters.
- **ISO15765 (protocol 6):** the DLL runs the **ISO 15765-2 (ISO-TP)** state machine itself —
  Single/First/Consecutive frames, flow control (block size + STmin), and the first-frame
  indication — because socketcand only carries raw CAN. Flow-control filters set up by the app map
  the TX↔RX IDs. This is what makes standard UDS/OBD apps work.

## 5. Supported / not supported

| | |
|---|---|
| ✅ CAN (raw), 11- and 29-bit | ✅ ISO15765 (ISO-TP), 11- and 29-bit, physical addressing |
| ✅ OBD-II / UDS diagnostics | ✅ Flow control (block size, STmin) |
| ❌ K-line (ISO9141 / ISO14230) | ❌ J1850 PWM/VPW — *different physical layer SocketCAN can't carry* (returned as `ERR_NOT_SUPPORTED`) |
| ⏳ CAN-FD (ISO15765 05.00) | ⏳ x64 DLL | ⏳ periodic messages — *follow-ons* |

## 6. Limitations — read before flashing

- **CAN only.** No K-line / J1850 — those need a different adapter.
- **Latency.** Network + software ISO-TP is slower than a hardware J2534 box. Fine for diagnostics;
  **timing-sensitive flashing over Wi-Fi is risky.** Use a **wired Ethernet** link for any flashing,
  and expect some flash protocols still won't meet timing. Diagnose first, flash at your own risk.
- **One multi-frame transmit at a time** (v1 ISO-TP TX uses a single flow-control slot).
- **socketcand is unauthenticated.** Anyone who can reach port 29536 can read *and write* the bus.
  Keep it on your own Wi-Fi AP / point-to-point Ethernet, never the internet. (Same discipline as the
  DIY guide's security note.)

## 7. Registry reference

Written in the **32-bit view** (`WOW6432Node` on 64-bit Windows) because the DLL and J2534 apps are 32-bit.

**J2534 registration** — `HKLM\SOFTWARE\WOW6432Node\PassThruSupport.04.04\Tester Present SocketCAN`:

| Value | Type | Example |
|-------|------|---------|
| `Name` | REG_SZ | `Tester Present SocketCAN` |
| `FunctionLibrary` | REG_SZ | `C:\…\TesPassThruSocketCAN.dll` |
| `ConfigApplication` | REG_SZ | `C:\…\TesSocketCANConfig.exe` |
| `CAN`, `ISO15765` | REG_DWORD | `1` |
| `ISO9141`, `ISO14230`, `J1850PWM`, `J1850VPW` | REG_DWORD | `0` |

**DLL target config** — `HKLM\SOFTWARE\WOW6432Node\Tester Present\PassThru-SocketCAN`:

| Value | Type | Example |
|-------|------|---------|
| `Host` | REG_SZ | `10.42.0.1` |
| `Port` | REG_DWORD | `29536` |
| `Interface` | REG_SZ | `can0` |
| `Bitrate` | REG_DWORD | `500000` |
| `ListenOnly` | REG_DWORD | `0` |

---

*Tester Present Specialist Automotive Solutions. Pairs with [Tester-Engineering-DIY](https://github.com/jakka351/Tester-Engineering-DIY).*
