# BBTAG-Improvement-Mod
Additional features and tweaks for BlazBlue Cross Tag Battle

The goal is to extend the game with community-created content, provide additional graphical options, and improve performance for players with computer specs below the minimal system requirements.

Join the [BB Improvement Mod Discord Server](https://discord.gg/j2mCX9s) to discuss about the development and the game itself!

## Legal
```
BBTAG Improvement Mod is NOT associated with Arc System Works or any of its partners / affiliates.
BBTAG Improvement Mod is NOT intended for malicious use.
BBTAG Improvement Mod is NOT intended to give players unfair advantages in online matches.
BBTAG Improvement Mod is NOT intended to unlock unreleased / unpurchased contents of the game.
BBTAG Improvement Mod should only be used on the official version that you legally purchased and own.

Use BBTAG Improvement Mod at your own risk. 
I, GrimFlash, am not responsible for what happens while using BBTAG Improvement Mod. You take full reponsibility for any outcome that happens to you while using this application.
```

## Compiling and usage
BBTAG Improvement Mod is coded using Visual Studio 2017 (toolset v141). <br>
To compile, you should only need to load the sln file and compile as-is. No changes should be needed to the solution or source.<br>
Copy the compiled binary, settings.ini, and palettes.ini file from the bin/ folder into the game's root folder.

## Requirements:
- Visual Studio 2017 (toolset v141) (Windows SDK 10.0.17763.0)
- Detours 1.5
- DirectX SDK
- Steamworks SDK (v1.42)

## Quick file overview
- **info.h** <br>
Defines for version numbering and outside links
- **logging.h** <br>
Self explanatory
- **interfaces.h** <br>
Access to all the wrappers and hooked game variables
- **D3D9ExWrapper/** <br>
Straightforward D3D wrapping
- **Game/** <br>
Player, ingame characters, meters information
- **PaletteManager/** <br>
Loading and in-game management of custom palettes
- **SteamApiWrapper/** <br>
Straightforward Steam API wrapping
- **Web/** <br>
HTTP request functions for update checking, palette downloading, fetching donator names
- **WindowManager/** <br>
Implementation of the mod's windows using the ImGui library
- **Hooks/hooks_bbtag** <br>
Implements game functions overriding
- **Hooks/hooks_detours** <br>
Implements imported functions overriding using the Detours library
- **Hooks/hooks_steamApiWrapper** <br>
Implements Steam API overriding

## Thanks to the people who have helped the mod along the way:
* KoviDomi
* Neptune
* Rouzel
* Labryz
* Dormin
* NeoStrayCat
* TheDukeOfErl
* WistfulHopes
* Everybody in the BlazBlue PC community that has given support or feedback of any kind!

## Extra thanks to:
Atom0s for their DirectX9.0 Hooking article<br>
Durante for their dsfix source
