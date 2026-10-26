# WAR MACHINE: ZERO HOUR

**A Combined-Arms Tactical FPS / Real-Time Command / Military Simulation built on Unreal Engine 5 + C++**

*All factions, weapons, vehicles and callsigns in this project are entirely fictional. No real-world military, nation, weapon system, or organization is depicted or modeled.*

---

## Table of Contents
- [English](#english)
- [فارسی (Persian)](#فارسی)
- [中文 (Chinese)](#中文)

---
<a name="english"></a>
## 🇬🇧 English

### 1. Overview

**WAR MACHINE: ZERO HOUR** is a large-scale project skeleton for a combined-arms military simulation that blends first-person combat, real-time squad command, vehicle operations, logistics, reconnaissance, and a persistent, reactive campaign. The gameplay core (AI, mission generation, war state, vehicles, logistics, save/load, squads, UI plumbing) is written in **C++**; Animation, VFX, level scripting and detailed UI composition are intended to be authored in **Blueprint** on top of the provided C++ base classes and subsystems, as is standard Unreal Engine workflow.

> **Scope note:** This repository ships a genuine, compiling-style C++ architecture covering the core systems below with real gameplay logic (not empty stubs). It is a **foundation to build the full game on**, not a finished, asset-complete AAA title — a project of this stated scope (Nanite/Lumen open world content, full multiplayer replication, anti-cheat, a complete mission/level roster, finished UMG art, and QA-approved shipping builds) requires a production team and engine-side asset authoring that cannot be produced as plain text files. Everything below is real, working, extensible code you can open directly in Unreal Engine 5.4+.

### 2. Core Systems Included

| Domain | What's implemented |
|---|---|
| Squad Command | `UWMSquadComponent`, `UWMSquadMember` — fireteams, formations, rules of engagement, cohesion model, casualty reporting |
| AI | `AWMAIController`, `UWMUtilityAIComponent` — perception-driven (Behavior Tree + Utility AI), morale/fatigue/confidence/discipline gauges |
| Command & Tactical Map | `UWMCommandComponent`, `UWMTacticalMapSubsystem` — command resources/cooldowns, Fog-of-War contact confidence decay |
| Missions | `UWMMissionDirectorSubsystem`, `UWMObjective` — dynamic objective graph, optional objectives, substitution instead of hard failure |
| War / World State | `UWMWarStateSubsystem`, `UWMFrontlineManager`, `UWMWorldEventEngine`, `UWMWeatherSubsystem` — persistent war indices, contested sectors, context-aware world events, weather gameplay modifiers |
| Campaign Clock | `UWMCampaignClockSubsystem` — **fully user-configurable** operation-window length, gap between windows, and time scale; the HUD can always query whether a window is open and how many minutes remain until the next transition |
| Weapons | `UWMWeaponDataAsset`, `AWMWeaponBase` — fictional, data-driven weapon stats, attachments with real trade-offs, abstracted gameplay damage falloff |
| Vehicles | `AWMVehicleBase`, `UWMVehicleDamageComponent` — component-based damage (Engine/Wheels/Power/Sensors/Comms/Armor/Crew), fuel/energy |
| Logistics | `AWMSupplyDepot`, `AWMConvoy`, `UWMLogisticsSubsystem` — simulated, movable, damageable/reroutable convoys |
| Recon | `UWMDroneComponent` — battery, range, signal quality, damage state, thermal sensor flag |
| Medical | `UWMMedicalComponent` — non-graphic Health/Injury/Downed/Recovery state machine with bleed-out timer |
| Equipment | `UWMInventoryComponent` — slot-based loadouts, saved presets, weight-driven handling penalty |
| Dialogue | `UWMDialogueSubsystem` — branching graph gated by decisions/evidence tags and a persistent per-character Trust value |
| Save System | `UWMSaveGame`, `UWMSaveSubsystem` — multi-slot, versioned, integrity-validated, with a crash-recovery slot |
| Radio | `UWMRadioSubsystem` — event-driven chatter bus with signal-quality gating |
| Replay | `UWMReplaySubsystem` — lightweight event timeline for post-mission review |
| UI plumbing | `AWMHUD` — widget lifetime management for HUD / Tactical Map / Command Wheel (visuals authored in UMG) |
| Dev Tools | `UWMDeveloperConsole` — Development-build-only console commands, stripped from Shipping via `UE_BUILD_SHIPPING` |

### 3. Project Structure

```
WarMachineZeroHour/
├── WarMachineZeroHour.uproject
├── Source/
│   ├── WarMachine/        (module entry point, game mode)
│   ├── Core/              (shared enums/structs, faction data, game instance)
│   ├── Squads/            (squad + fireteam + member)
│   ├── Command/           (command component, tactical map subsystem)
│   ├── AI/                (AI controller, utility AI)
│   ├── Missions/          (objective, mission director)
│   ├── World/             (war state, frontline, world events, weather, campaign clock)
│   ├── Weapons/           (weapon data + base actor)
│   ├── Vehicles/          (vehicle base + component damage)
│   ├── Logistics/         (depot, convoy, logistics subsystem)
│   ├── Recon/             (drone component)
│   ├── Medical/           (medical component)
│   ├── Equipment/         (inventory component)
│   ├── Dialogue/          (dialogue subsystem)
│   ├── Save/              (save game + save subsystem)
│   ├── Radio/             (radio subsystem)
│   ├── Replay/            (replay subsystem)
│   ├── UI/                (HUD widget lifetime)
│   └── Tools/             (developer console)
├── Content/Localization/WarMachine/{en,fa,zh-Hans}/   (sample string tables)
├── Config/                (DefaultEngine.ini, DefaultGame.ini)
├── Scripts/               (PowerShell setup/build/test/package scripts)
└── Docs/
```

### 4. Requirements (Windows 10/11)

- Windows 10 (64-bit) or Windows 11
- **Epic Games Launcher**
- **Unreal Engine 5.4** (or later 5.x) installed through the launcher
- **Visual Studio 2022** with these workloads/components:
  - *Desktop development with C++*
  - *Game development with C++*
  - *Windows 10/11 SDK* (latest available)
- **Git** for Windows
- At least 100 GB free disk space (engine + build artifacts + derived data cache)

### 5. Installation & First Build — Step by Step

1. **Install the Epic Games Launcher** from the Epic Games website and sign in.
2. In the launcher, go to **Unreal Engine → Library** and click **Install Engine**, choosing version **5.4** (or newer 5.x). This can take a long time and a lot of disk space — let it finish fully.
3. **Install Visual Studio 2022** (Community edition is fine). In the Visual Studio Installer, tick:
   - Desktop development with C++
   - Game development with C++
   - Individual component: Windows 10/11 SDK (latest)
4. **Install Git for Windows** and make sure `git` is available from PowerShell (`git --version`).
5. Place this project folder somewhere with a short path, e.g. `C:\Projects\WarMachineZeroHour`.
6. Open **PowerShell** in the project's `Scripts` folder and run the environment check:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\setup-windows.ps1
   ```
   Fix anything it reports as `[MISSING]` before continuing.
7. **Generate project files and compile a Development Editor build**:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\build-development.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
   ```
   Adjust `-EngineRoot` if your engine is installed elsewhere.
8. Once the build succeeds, open `WarMachineZeroHour.uproject` (double-click, or right-click → *Generate Visual Studio project files* first if you prefer to compile from inside Visual Studio instead of the script).
9. In the Unreal Editor, click **Play** to run in PIE (Play-In-Editor) mode.
10. **Run automation tests**:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\run-tests.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
    ```
11. **Build a Shipping (release) build**:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\build-shipping.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
    ```
12. **Package the game** into a distributable folder:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\package.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4" -OutputDir "C:\Projects\WarMachineZeroHour\Packaged"
    ```
13. To reset all generated build artifacts and start clean:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\clean.ps1
    ```

### 6. Common Troubleshooting

- **"Modules are missing or built with a different engine version"** when opening the `.uproject`: right-click the `.uproject` file → *Generate Visual Studio project files*, then rebuild with `build-development.ps1`.
- **Unreal Build Tool errors about a missing module**: check `Source/WarMachine/WarMachine.Build.cs` — every plugin/module a `.cpp` file includes headers from must be listed there.
- **Shader compilation seems stuck at editor startup**: this is normal on first open (Derived Data Cache is empty); it will be fast on subsequent launches.
- **Visual Studio can't find the Windows SDK**: reopen the Visual Studio Installer and add the latest Windows 10/11 SDK component, then re-run `build-development.ps1`.
- **Packaging fails at the Cook step**: check `Saved/Logs/` for the first cook error — it is almost always a missing asset reference or a plugin left enabled in `.uproject` without its content.
- **Derived Data Cache grows very large**: this is expected for a Nanite/Lumen project; you can safely delete `DerivedDataCache/` with `clean.ps1` if disk space is tight (it will simply rebuild on next launch).

### 7. Campaign Clock — User-Configurable Scheduling

The **Campaign Clock** (`UWMCampaignClockSubsystem`) is fully driven by values the player/designer enters — nothing about mission scheduling is hardcoded:

- `OperationWindowLengthMinutes` — how long (in-game minutes) the Command Center accepts launching a new operation.
- `GapBetweenWindowsMinutes` — how long the cooldown/prep gap is before the next window opens.
- `MinutesPerRealSecond` — how fast in-game time passes relative to real time.

At any moment, `IsOperationWindowOpen()` tells you whether a mission can currently be launched, and `GetMinutesUntilNextTransition()` tells you exactly how many in-game minutes remain until the state flips (window closing, or the next window opening). Bind these to the Base/Command Center UI to show live countdowns.

### 8. Localization & Accessibility

- Supported languages: **English (LTR)**, **Persian/فارسی (RTL)**, **Chinese/中文 (LTR)** — sample string tables are under `Content/Localization/WarMachine/{en,fa,zh-Hans}/`.
- UI is intended to be built with **CommonUI/UMG**, using UE5's built-in text flow direction support so Persian panels/menus/subtitles/dialogue/briefings/reports render right-to-left while English/Chinese stay left-to-right.
- Planned theme set: Dark, Light, Tactical Blue, Red Alert, Green Command, High Contrast, Windows 11-inspired.
- Accessibility hooks anticipated: colorblind modes, high contrast, subtitle controls, motion reduction, camera-shake slider, FOV slider, sensitivity, hold/toggle input options, remappable bindings, controller support.

### 9. Extending This Project

- Add new fictional factions as `UWMFactionDataAsset` instances — never model a real army, unit, or weapon.
- Add new weapons/vehicles as new `UWMWeaponDataAsset` / `AWMVehicleBase` subclasses — keep all names, specs and ballistics abstracted and fictional.
- Build UMG widgets that read from `UWMTacticalMapSubsystem`, `UWMCommandComponent`, `UWMRadioSubsystem`, etc., and drive them from `AWMHUD`.
- Add a Behavior Tree asset per squad role and assign it to `AWMAIController::BehaviorTreeAsset`; branch subtree selection on the `CurrentUtilityAction` Blackboard key.
- Wire `UWMMissionDirectorSubsystem` objective generation to real `UWMFrontlineManager` sector data for your levels instead of the illustrative placeholder locations in `GenerateMissionObjectives`.

---
<a name="فارسی"></a>
## 🇮🇷 فارسی

### ۱. معرفی

**WAR MACHINE: ZERO HOUR** یک اسکلت پروژه بزرگ برای یک شبیه‌ساز نظامی Combined-Arms است که ترکیبی از نبرد اول‌شخص، فرماندهی تاکتیکی بلادرنگ، عملیات وسایل نقلیه، تدارکات، شناسایی و یک Campaign پویا و ماندگار را ارائه می‌دهد. هسته Gameplay (هوش مصنوعی، تولید مأموریت، وضعیت جنگ، وسایل نقلیه، تدارکات، Save/Load، Squad و زیرساخت UI) با **++C** نوشته شده و انیمیشن، جلوه‌های بصری، اسکریپت‌نویسی Level و ترکیب دقیق UI طبق روال استاندارد Unreal Engine باید در **Blueprint** روی کلاس‌ها و Subsystemهای C++ ارائه‌شده ساخته شوند.

> **نکته درباره محدوده کار:** این مخزن یک معماری واقعی و کامپایل‌پذیر از C++ ارائه می‌دهد که سیستم‌های اصلی زیر را با منطق واقعی Gameplay (نه Placeholder خالی) پوشش می‌دهد. این پروژه یک **پایه برای ساخت بازی کامل** است، نه یک عنوان AAA کاملاً تمام‌شده با تمام Assetها؛ ساخت پروژه‌ای با این حجم از ویژگی (محتوای دنیای باز Nanite/Lumen، Replication کامل Multiplayer، Anti-Cheat، فهرست کامل مأموریت/Level، هنر UMG نهایی و Buildهای Shipping تأییدشده توسط QA) به یک تیم تولید و ساخت Asset درون Editor نیاز دارد که در قالب فایل‌های متنی ساده قابل تحویل نیست. تمام موارد زیر کد واقعی، کارکردی و قابل توسعه هستند که می‌توانید مستقیماً در Unreal Engine 5.4 به بالا باز کنید.

### ۲. سیستم‌های اصلی موجود

| بخش | آنچه پیاده‌سازی شده |
|---|---|
| فرماندهی Squad | `UWMSquadComponent`، `UWMSquadMember` — Fireteam، Formation، قوانین درگیری، مدل Cohesion، گزارش تلفات |
| هوش مصنوعی | `AWMAIController`، `UWMUtilityAIComponent` — مبتنی بر Perception (ترکیب Behavior Tree و Utility AI)، شاخص‌های روحیه/خستگی/اعتمادبه‌نفس/انضباط |
| فرماندهی و نقشه تاکتیکی | `UWMCommandComponent`، `UWMTacticalMapSubsystem` — منابع/Cooldown فرماندهی، کاهش Confidence اطلاعات تحت Fog of War |
| مأموریت‌ها | `UWMMissionDirectorSubsystem`، `UWMObjective` — گراف Objective پویا، Objectiveهای اختیاری، جایگزینی به‌جای شکست قطعی |
| وضعیت جنگ/جهان | `UWMWarStateSubsystem`، `UWMFrontlineManager`، `UWMWorldEventEngine`، `UWMWeatherSubsystem` — شاخص‌های ماندگار جنگ، مناطق مورد مناقشه، رویدادهای وابسته به وضعیت جهان، اثرات آب‌وهوا روی Gameplay |
| ساعت Campaign | `UWMCampaignClockSubsystem` — **کاملاً توسط کاربر قابل تنظیم**: طول Operation Window، فاصله بین Windowها و مقیاس زمان؛ HUD همیشه می‌تواند بپرسد آیا الان Window باز است و چند دقیقه تا تغییر بعدی مانده |
| سلاح‌ها | `UWMWeaponDataAsset`، `AWMWeaponBase` — مشخصات سلاح کاملاً خیالی و Data-Driven، Attachmentهای با Trade-off واقعی، افت آسیب Gameplay انتزاعی |
| وسایل نقلیه | `AWMVehicleBase`، `UWMVehicleDamageComponent` — آسیب Component-Based (موتور/چرخ/توان/سنسور/ارتباطات/زره/خدمه)، سوخت/انرژی |
| تدارکات | `AWMSupplyDepot`، `AWMConvoy`، `UWMLogisticsSubsystem` — Convoyهای شبیه‌سازی‌شده، متحرک، قابل آسیب/تغییر مسیر |
| شناسایی | `UWMDroneComponent` — باتری، برد، کیفیت سیگنال، وضعیت آسیب، پرچم سنسور حرارتی |
| پزشکی | `UWMMedicalComponent` — ماشین‌حالت غیرگرافیکی Health/Injury/Downed/Recovery با تایمر خونریزی |
| تجهیزات | `UWMInventoryComponent` — Loadout مبتنی بر Slot، Preset ذخیره‌شده، جریمه Handling وابسته به وزن |
| دیالوگ | `UWMDialogueSubsystem` — گراف انشعابی مشروط به Tagهای تصمیم/شواهد و مقدار Trust ماندگار برای هر شخصیت |
| سیستم Save | `UWMSaveGame`، `UWMSaveSubsystem` — چند Slot، Version‌دار، اعتبارسنجی‌شده، همراه با Slot بازیابی پس از Crash |
| رادیو | `UWMRadioSubsystem` — گذرگاه Chatter رویدادمحور با محدودیت کیفیت سیگنال |
| Replay | `UWMReplaySubsystem` — Timeline سبک رویدادها برای بازبینی پس از مأموریت |
| زیرساخت UI | `AWMHUD` — مدیریت طول عمر Widget برای HUD / نقشه تاکتیکی / Command Wheel (ظاهر در UMG ساخته می‌شود) |
| ابزار توسعه | `UWMDeveloperConsole` — دستورات Console مخصوص Development Build که از Shipping با `UE_BUILD_SHIPPING` حذف می‌شوند |

### ۳. ساختار پروژه

```
WarMachineZeroHour/
├── WarMachineZeroHour.uproject
├── Source/
│   ├── WarMachine/        (نقطه ورود ماژول، Game Mode)
│   ├── Core/              (Enum/Struct مشترک، داده Faction، Game Instance)
│   ├── Squads/            (Squad + Fireteam + Member)
│   ├── Command/           (Command Component، Tactical Map Subsystem)
│   ├── AI/                (AI Controller، Utility AI)
│   ├── Missions/          (Objective، Mission Director)
│   ├── World/             (War State، Frontline، World Events، Weather، Campaign Clock)
│   ├── Weapons/           (داده سلاح + Actor پایه)
│   ├── Vehicles/          (وسیله نقلیه پایه + آسیب Component)
│   ├── Logistics/         (Depot، Convoy، Logistics Subsystem)
│   ├── Recon/             (Drone Component)
│   ├── Medical/           (Medical Component)
│   ├── Equipment/         (Inventory Component)
│   ├── Dialogue/          (Dialogue Subsystem)
│   ├── Save/              (Save Game + Save Subsystem)
│   ├── Radio/             (Radio Subsystem)
│   ├── Replay/            (Replay Subsystem)
│   ├── UI/                (طول عمر Widget های HUD)
│   └── Tools/             (Developer Console)
├── Content/Localization/WarMachine/{en,fa,zh-Hans}/   (نمونه جدول‌های رشته)
├── Config/                (DefaultEngine.ini، DefaultGame.ini)
├── Scripts/               (اسکریپت‌های PowerShell برای Setup/Build/Test/Package)
└── Docs/
```

### ۴. پیش‌نیازها (ویندوز ۱۰/۱۱)

- ویندوز ۱۰ (۶۴ بیتی) یا ویندوز ۱۱
- **Epic Games Launcher**
- **Unreal Engine 5.4** (یا نسخه‌های جدیدتر 5.x) نصب‌شده از طریق Launcher
- **Visual Studio 2022** همراه با این Workloadها/Componentها:
  - *Desktop development with C++*
  - *Game development with C++*
  - *Windows 10/11 SDK* (جدیدترین نسخه موجود)
- **Git** برای ویندوز
- حداقل ۱۰۰ گیگابایت فضای خالی دیسک (Engine + خروجی Build + Derived Data Cache)

### ۵. نصب و اولین Build — گام به گام

۱. **Epic Games Launcher** را از وب‌سایت Epic Games نصب و وارد حساب خود شوید.
۲. در Launcher به بخش **Unreal Engine → Library** بروید و روی **Install Engine** کلیک کنید و نسخه **۵.۴** (یا 5.x جدیدتر) را انتخاب کنید. این مرحله ممکن است زمان و فضای زیادی نیاز داشته باشد؛ اجازه دهید کامل تمام شود.
۳. **Visual Studio 2022** را نصب کنید (نسخه Community کافی است). در Visual Studio Installer این موارد را تیک بزنید:
   - Desktop development with C++
   - Game development with C++
   - Component مجزا: Windows 10/11 SDK (جدیدترین نسخه)
۴. **Git برای ویندوز** را نصب کنید و مطمئن شوید `git` در PowerShell در دسترس است (`git --version`).
۵. پوشه این پروژه را در مسیری کوتاه قرار دهید، مثلاً `C:\Projects\WarMachineZeroHour`.
۶. **PowerShell** را در پوشه `Scripts` پروژه باز کنید و بررسی محیط را اجرا کنید:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\setup-windows.ps1
   ```
   هر موردی که با `[MISSING]` گزارش شد را قبل از ادامه برطرف کنید.
۷. **تولید فایل‌های پروژه و کامپایل Development Editor Build**:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\build-development.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
   ```
   اگر Engine شما در مسیر دیگری نصب شده، مقدار `-EngineRoot` را تغییر دهید.
۸. پس از موفقیت Build، فایل `WarMachineZeroHour.uproject` را باز کنید (دابل‌کلیک، یا برای کامپایل از داخل Visual Studio ابتدا روی آن راست‌کلیک کرده و *Generate Visual Studio project files* را انتخاب کنید).
۹. در Unreal Editor روی **Play** کلیک کنید تا در حالت PIE اجرا شود.
۱۰. **اجرای Automation Testها**:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\run-tests.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
    ```
۱۱. **ساخت Shipping Build (نسخه نهایی)**:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\build-shipping.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
    ```
۱۲. **Package کردن بازی** به یک پوشه قابل توزیع:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\package.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4" -OutputDir "C:\Projects\WarMachineZeroHour\Packaged"
    ```
۱۳. برای پاک‌سازی تمام خروجی‌های Build و شروع تازه:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\clean.ps1
    ```

### ۶. رفع مشکلات رایج

- پیام **"Modules are missing or built with a different engine version"** هنگام باز کردن `.uproject`: روی فایل `.uproject` راست‌کلیک کرده و *Generate Visual Studio project files* را بزنید، سپس با `build-development.ps1` دوباره Build بگیرید.
- خطاهای Unreal Build Tool درباره ماژول گم‌شده: فایل `Source/WarMachine/WarMachine.Build.cs` را بررسی کنید — هر Plugin/ماژولی که یک فایل `.cpp` از آن Header وارد می‌کند باید در آنجا لیست شده باشد.
- **کامپایل Shader هنگام باز شدن Editor گیر کرده به نظر می‌رسد**: در اولین باز شدن طبیعی است (Derived Data Cache خالی است)؛ در دفعات بعد سریع خواهد بود.
- **Visual Studio نمی‌تواند Windows SDK را پیدا کند**: دوباره Visual Studio Installer را باز کرده و جدیدترین Component مربوط به Windows 10/11 SDK را اضافه کنید، سپس `build-development.ps1` را دوباره اجرا کنید.
- **Packaging در مرحله Cook شکست می‌خورد**: پوشه `Saved/Logs/` را برای اولین خطای Cook بررسی کنید — تقریباً همیشه یک ارجاع Asset گم‌شده یا Pluginی است که در `.uproject` فعال مانده ولی Content آن موجود نیست.
- **حجم Derived Data Cache خیلی زیاد می‌شود**: برای پروژه Nanite/Lumen طبیعی است؛ در صورت کمبود فضا می‌توانید با `clean.ps1` پوشه `DerivedDataCache/` را حذف کنید (در اجرای بعدی دوباره ساخته می‌شود).

### ۷. ساعت Campaign — زمان‌بندی کاملاً قابل تنظیم توسط کاربر

**Campaign Clock** (`UWMCampaignClockSubsystem`) کاملاً توسط مقادیری که بازیکن/طراح وارد می‌کند هدایت می‌شود — هیچ‌چیز درباره زمان‌بندی مأموریت به‌صورت ثابت در کد نوشته نشده:

- `OperationWindowLengthMinutes` — مدت‌زمان (به دقیقه درون‌بازی) که Command Center اجازه شروع یک عملیات جدید را می‌دهد.
- `GapBetweenWindowsMinutes` — مدت‌زمان فاصله/آماده‌سازی قبل از باز شدن Window بعدی.
- `MinutesPerRealSecond` — سرعت گذر زمان درون‌بازی نسبت به زمان واقعی.

در هر لحظه، تابع `IsOperationWindowOpen()` مشخص می‌کند که آیا در حال حاضر می‌توان مأموریتی را شروع کرد یا نه، و تابع `GetMinutesUntilNextTransition()` دقیقاً می‌گوید چند دقیقه درون‌بازی تا تغییر وضعیت (بسته شدن Window فعلی یا باز شدن Window بعدی) باقی مانده است. این مقادیر را به UI پایگاه/Command Center متصل کنید تا شمارش معکوس زنده نمایش داده شود.

### ۸. بومی‌سازی و دسترسی‌پذیری

- زبان‌های پشتیبانی‌شده: **انگلیسی (چپ‌چین)**، **فارسی (راست‌چین)**، **چینی (چپ‌چین)** — نمونه جدول‌های رشته در مسیر `Content/Localization/WarMachine/{en,fa,zh-Hans}/` قرار دارند.
- UI باید با **CommonUI/UMG** و با استفاده از پشتیبانی داخلی Unreal Engine 5 از جهت متن ساخته شود تا پنل‌ها/منوها/زیرنویس‌ها/دیالوگ‌ها/Briefingها/گزارش‌های فارسی به‌صورت راست‌به‌چپ و انگلیسی/چینی به‌صورت چپ‌به‌راست نمایش داده شوند.
- مجموعه تم‌های برنامه‌ریزی‌شده: تیره، روشن، آبی تاکتیکی، هشدار قرمز، فرماندهی سبز، کنتراست بالا، الهام‌گرفته از ویندوز ۱۱.
- قابلیت‌های دسترسی‌پذیری موردنظر: حالت‌های رنگ‌کوری، کنتراست بالا، تنظیمات زیرنویس، کاهش حرکت، اسلایدر لرزش دوربین، اسلایدر FOV، حساسیت، گزینه‌های Hold/Toggle برای ورودی، کلیدهای قابل تغییر، پشتیبانی از دسته کنترلر.

### ۹. توسعه این پروژه

- Factionهای خیالی جدید را به‌صورت نمونه‌های `UWMFactionDataAsset` اضافه کنید — هرگز یک ارتش، یگان یا سلاح واقعی را مدل‌سازی نکنید.
- سلاح‌ها/وسایل نقلیه جدید را به‌صورت زیرکلاس‌های جدید `UWMWeaponDataAsset` / `AWMVehicleBase` اضافه کنید — تمام نام‌ها، مشخصات و بالستیک را انتزاعی و خیالی نگه دارید.
- Widgetهای UMG بسازید که از `UWMTacticalMapSubsystem`، `UWMCommandComponent`، `UWMRadioSubsystem` و غیره داده می‌خوانند و آن‌ها را از `AWMHUD` هدایت کنید.
- برای هر نقش Squad یک Behavior Tree بسازید و آن را به `AWMAIController::BehaviorTreeAsset` اختصاص دهید؛ انتخاب Subtree را بر اساس کلید Blackboard با نام `CurrentUtilityAction` شاخه‌بندی کنید.
- تولید Objective در `UWMMissionDirectorSubsystem` را به داده واقعی Sector از `UWMFrontlineManager` برای Levelهای خودتان متصل کنید، به‌جای موقعیت‌های نمونه در `GenerateMissionObjectives`.

---
<a name="中文"></a>
## 🇨🇳 中文

### 一、项目概述

**WAR MACHINE: ZERO HOUR（战争机器：零时）** 是一个大型的多兵种战术模拟游戏的项目骨架,融合了第一人称战斗、实时班组指挥、载具作战、后勤补给、侦察以及一个持久化且会动态响应玩家决策的战役系统。核心玩法逻辑(AI、任务生成、战争状态、载具、后勤、存档/读档、班组以及UI管线)使用 **C++** 编写;按照 Unreal Engine 的标准工作流程,动画、特效、关卡脚本以及精细的UI组合应在提供的C++基类和子系统之上使用 **蓝图(Blueprint)** 完成。

> **范围说明:** 本仓库提供的是一套真实、可编译风格的C++架构,涵盖下方列出的核心系统,并包含真实的玩法逻辑(而非空占位符)。它是一个**用于构建完整游戏的基础框架**,而不是一款资源齐全、已完工的3A游戏——达到需求文档所描述规模的项目(Nanite/Lumen开放世界内容、完整的多人游戏复制、反作弊、完整的任务/关卡列表、完工的UMG美术资源以及经QA验证的发行版本)需要一个完整的制作团队以及在编辑器内完成的美术资源制作,这些都无法以纯文本文件的形式交付。以下所有内容均为真实、可运行、可扩展的代码,可直接在 Unreal Engine 5.4 及以上版本中打开使用。

### 二、已实现的核心系统

| 领域 | 已实现内容 |
|---|---|
| 班组指挥 | `UWMSquadComponent`、`UWMSquadMember` —— 火力小组、队形、交战规则、协同度模型、伤亡报告 |
| 人工智能 | `AWMAIController`、`UWMUtilityAIComponent` —— 基于感知驱动(行为树 + 效用AI),含士气/疲劳/信心/纪律等指标 |
| 指挥与战术地图 | `UWMCommandComponent`、`UWMTacticalMapSubsystem` —— 指挥资源/冷却时间,战争迷雾下的情报可信度随时间衰减 |
| 任务系统 | `UWMMissionDirectorSubsystem`、`UWMObjective` —— 动态目标图、可选目标、以替代方案取代任务硬性失败 |
| 战争/世界状态 | `UWMWarStateSubsystem`、`UWMFrontlineManager`、`UWMWorldEventEngine`、`UWMWeatherSubsystem` —— 持久化战争指数、争夺中的战区、依据世界状态生成的事件、天气对玩法的影响 |
| 战役时钟 | `UWMCampaignClockSubsystem` —— **完全由用户配置**:行动窗口时长、窗口间隔时间、时间流速;HUD 随时可查询当前窗口是否开放以及距离下次状态切换还有多少分钟 |
| 武器系统 | `UWMWeaponDataAsset`、`AWMWeaponBase` —— 完全虚构、数据驱动的武器数值,附件带有真实的取舍效果,伤害衰减为抽象化的玩法模型 |
| 载具系统 | `AWMVehicleBase`、`UWMVehicleDamageComponent` —— 基于部件的损伤系统(引擎/轮组/动力系统/传感器/通讯/装甲/乘员舱),燃料/能量管理 |
| 后勤系统 | `AWMSupplyDepot`、`AWMConvoy`、`UWMLogisticsSubsystem` —— 可模拟移动、可被摧毁/可改道的护送车队 |
| 侦察系统 | `UWMDroneComponent` —— 电量、航程、信号质量、损伤状态、热成像传感器标志 |
| 医疗系统 | `UWMMedicalComponent` —— 非血腥的健康/受伤/倒地/恢复状态机,带有失血倒计时 |
| 装备系统 | `UWMInventoryComponent` —— 基于槽位的装备配置、可保存的预设方案、由重量决定的操作惩罚 |
| 对话系统 | `UWMDialogueSubsystem` —— 由决策/证据标签门控的分支对话图,以及每个角色持久化的信任值 |
| 存档系统 | `UWMSaveGame`、`UWMSaveSubsystem` —— 多存档槽、版本化、完整性校验,并带有崩溃恢复存档槽 |
| 无线电系统 | `UWMRadioSubsystem` —— 事件驱动的通讯总线,受信号质量门控 |
| 回放系统 | `UWMReplaySubsystem` —— 用于任务后复盘的轻量级事件时间线 |
| UI管线 | `AWMHUD` —— 管理HUD/战术地图/指挥轮盘的控件生命周期(具体视觉效果在UMG中制作) |
| 开发者工具 | `UWMDeveloperConsole` —— 仅限开发版本使用的控制台命令,通过 `UE_BUILD_SHIPPING` 从发行版中剔除 |

### 三、项目结构

```
WarMachineZeroHour/
├── WarMachineZeroHour.uproject
├── Source/
│   ├── WarMachine/        (模块入口、游戏模式)
│   ├── Core/              (共享枚举/结构体、阵营数据、游戏实例)
│   ├── Squads/            (班组 + 火力小组 + 成员)
│   ├── Command/           (指挥组件、战术地图子系统)
│   ├── AI/                (AI控制器、效用AI)
│   ├── Missions/          (目标、任务导演)
│   ├── World/             (战争状态、前线、世界事件、天气、战役时钟)
│   ├── Weapons/           (武器数据 + 基础Actor)
│   ├── Vehicles/          (载具基类 + 部件损伤)
│   ├── Logistics/         (补给站、车队、后勤子系统)
│   ├── Recon/             (无人机组件)
│   ├── Medical/           (医疗组件)
│   ├── Equipment/         (物品栏组件)
│   ├── Dialogue/          (对话子系统)
│   ├── Save/              (存档 + 存档子系统)
│   ├── Radio/             (无线电子系统)
│   ├── Replay/            (回放子系统)
│   ├── UI/                (HUD控件生命周期)
│   └── Tools/             (开发者控制台)
├── Content/Localization/WarMachine/{en,fa,zh-Hans}/   (示例字符串表)
├── Config/                (DefaultEngine.ini、DefaultGame.ini)
├── Scripts/               (PowerShell 安装/编译/测试/打包脚本)
└── Docs/
```

### 四、系统要求(Windows 10/11)

- Windows 10(64位)或 Windows 11
- **Epic Games Launcher**
- 通过启动器安装的 **Unreal Engine 5.4**(或更新的5.x版本)
- **Visual Studio 2022**,并勾选以下工作负载/组件:
  - *使用C++的桌面开发*
  - *使用C++的游戏开发*
  - *Windows 10/11 SDK*(最新可用版本)
- Windows版 **Git**
- 至少100GB可用磁盘空间(引擎 + 编译产物 + 派生数据缓存)

### 五、安装与首次编译 — 分步说明

1. 从 Epic Games 官网安装 **Epic Games Launcher** 并登录账号。
2. 在启动器中进入 **Unreal Engine → Library**,点击 **Install Engine**,选择 **5.4**(或更新的5.x版本)。此过程可能耗时较长且占用大量磁盘空间,请耐心等待完成。
3. 安装 **Visual Studio 2022**(社区版即可)。在 Visual Studio Installer 中勾选:
   - 使用C++的桌面开发
   - 使用C++的游戏开发
   - 单个组件:Windows 10/11 SDK(最新版)
4. 安装 **Windows版Git**,并确认可在 PowerShell 中使用 `git`(运行 `git --version` 验证)。
5. 将本项目文件夹放置在路径较短的位置,例如 `C:\Projects\WarMachineZeroHour`。
6. 在项目的 `Scripts` 文件夹中打开 **PowerShell**,运行环境检查脚本:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\setup-windows.ps1
   ```
   在继续之前,请修复所有标记为 `[MISSING]` 的问题。
7. **生成项目文件并编译开发版编辑器**:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\build-development.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
   ```
   如果引擎安装在其他位置,请相应调整 `-EngineRoot` 参数。
8. 编译成功后,打开 `WarMachineZeroHour.uproject`(直接双击;如果希望改用 Visual Studio 编译,可先右键点击该文件选择 *Generate Visual Studio project files*)。
9. 在 Unreal 编辑器中点击 **Play** 以编辑器内运行(PIE)模式启动游戏。
10. **运行自动化测试**:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\run-tests.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
    ```
11. **编译发行版(Shipping)版本**:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\build-shipping.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4"
    ```
12. **打包游戏**为可分发的文件夹:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\package.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.4" -OutputDir "C:\Projects\WarMachineZeroHour\Packaged"
    ```
13. 如需清除所有编译产物并重新开始:
    ```powershell
    powershell -ExecutionPolicy Bypass -File .\clean.ps1
    ```

### 六、常见问题排查

- 打开 `.uproject` 时提示 **"Modules are missing or built with a different engine version"**:右键点击 `.uproject` 文件,选择 *Generate Visual Studio project files*,然后使用 `build-development.ps1` 重新编译。
- Unreal Build Tool 报告模块缺失错误:检查 `Source/WarMachine/WarMachine.Build.cs` —— 任何被某个 `.cpp` 文件引用头文件的插件/模块都必须在此列出。
- **编辑器启动时着色器编译似乎卡住**:首次打开时属于正常现象(派生数据缓存为空),后续启动会更快。
- **Visual Studio 找不到 Windows SDK**:重新打开 Visual Studio Installer,添加最新的 Windows 10/11 SDK 组件,然后重新运行 `build-development.ps1`。
- **打包在 Cook 阶段失败**:查看 `Saved/Logs/` 中的首个 Cook 错误——几乎总是由于缺失的资源引用,或是在 `.uproject` 中启用了某个插件却没有对应的内容资源。
- **派生数据缓存体积过大**:对于Nanite/Lumen项目这是正常现象;如果磁盘空间紧张,可以使用 `clean.ps1` 安全删除 `DerivedDataCache/` 文件夹(下次启动时会自动重建)。

### 七、战役时钟 —— 用户可完全自定义的日程安排

**战役时钟**(`UWMCampaignClockSubsystem`)完全由玩家/设计师输入的数值驱动——任务日程安排中没有任何硬编码内容:

- `OperationWindowLengthMinutes` —— 指挥中心允许发起新行动的持续时间(游戏内分钟)。
- `GapBetweenWindowsMinutes` —— 下一个窗口开启前的冷却/准备间隔时间。
- `MinutesPerRealSecond` —— 游戏内时间相对于真实时间的流逝速度。

在任意时刻,`IsOperationWindowOpen()` 会告知当前是否可以发起任务,而 `GetMinutesUntilNextTransition()` 会精确给出距离状态切换(当前窗口关闭,或下一个窗口开启)还剩多少游戏内分钟。请将这些数值绑定到基地/指挥中心界面上,以显示实时倒计时。

### 八、本地化与无障碍功能

- 支持语言:**英语(从左到右)**、**波斯语/فارسی(从右到左)**、**中文(从左到右)**——示例字符串表位于 `Content/Localization/WarMachine/{en,fa,zh-Hans}/` 路径下。
- UI 应使用 **CommonUI/UMG** 搭建,并利用 Unreal Engine 5 内置的文本方向支持功能,使波斯语面板/菜单/字幕/对话/简报/报告以从右到左方式显示,而英文/中文则保持从左到右显示。
- 计划中的主题集合:深色、浅色、战术蓝、红色警戒、绿色指挥、高对比度、Windows 11风格。
- 计划中的无障碍功能:色盲模式、高对比度、字幕控制、动态效果减弱、镜头晃动强度滑块、视场角(FOV)滑块、灵敏度设置、按住/切换式输入选项、可自定义按键绑定、手柄支持。

### 九、扩展本项目

- 新增虚构阵营时,请以 `UWMFactionDataAsset` 实例的形式添加——切勿还原任何真实军队、部队或武器。
- 新增武器/载具时,请创建新的 `UWMWeaponDataAsset` / `AWMVehicleBase` 子类——所有名称、参数及弹道效果均应保持抽象化且完全虚构。
- 构建从 `UWMTacticalMapSubsystem`、`UWMCommandComponent`、`UWMRadioSubsystem` 等系统读取数据的UMG控件,并通过 `AWMHUD` 驱动它们。
- 为每个班组角色制作一个行为树资源,并将其赋值给 `AWMAIController::BehaviorTreeAsset`;根据名为 `CurrentUtilityAction` 的黑板键值来分支选择子树。
- 将 `UWMMissionDirectorSubsystem` 中的目标生成逻辑,对接到你关卡中来自 `UWMFrontlineManager` 的真实战区数据,而不是使用 `GenerateMissionObjectives` 中的示例占位坐标。
