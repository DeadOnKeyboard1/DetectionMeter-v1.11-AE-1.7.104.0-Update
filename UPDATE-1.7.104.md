# Detection Meter 1.1.3 - Steam Skyrim 1.7.104.0

1.1.3: exclude player teammates and actors commanded by the player or a
teammate, including companion summons and reanimated actors. Follow up to
eight ownership links; malformed cycles terminate. Remove existing meters
when an actor becomes ineligible, under the existing collection lock.
Teammates use the engine's IsPlayerTeammate flag; custom followers that never
set this flag and have no allied commander need a mod-specific integration.
Only meter visibility changes, not the engine's detection/stealth behavior.
Mock tests cover direct/nested summons, missing ownership, dismissed followers,
neutral/enemy ownership and cycles. In-game confirmation is still pending.

Full package rebuilt from MaxsuDetectionMeter 1.1.1 source for SKSE 2.3.1.
Original PNG textures, INI and optional dMenu JSON are retained from the supplied
Detection Meter-v1.11 folder. Matching PDB included. Replace the previous mod
package in Vortex/MO2 and restart Skyrim. Address Library and SKSE are not bundled.
The original mod does not need to remain installed alongside this full package.

Requires the matching Address Library and SKSE for Steam Skyrim 1.7.104.0.
dMenu remains optional; INI editing and the console command MaxsuDM reload work
without it. This update does not add an MCM or change meter textures/design.

Retained: directional NPC detection in sneak mode; non-hostile/hostile meters;
combat stealth points; fade, fill and flash animations; screen positioning;
resolution scaling; player-summon/follower exclusion; INI and dMenu reload.

Changes: explicit runtime/version metadata; current format-5 Address Library
reader; ControlMap flags/stack at 0x120/0x108 with an empty-stack guard; null actor
and camera guards; locked meter collection updates and configuration reloads;
DirectX texture HRESULT checks; overlay does not change the game's mouse cursor.
Missing CommonLib convenience functions are implemented locally without changing
the shared toolkit. Hook slots and D3D call boundary were checked against the
installed SkyrimSE.exe, and renderer offsets against SKSE 2.3.1 sources.

Build: cmake -S cmake-ae -B build-ae with the shared dependency prefix, then
cmake --build build-ae --config RelWithDebInfo. Original CMake scripts are retained
but are not used: their post-build deployment commands are not executed.
ImGui is pinned to v1.89.9 (c6e0284ac58b3f205c95365478888f7b53b077e2).
Other build inputs: toolkit CommonLib, CombatPathingRevolution extern/DKUtil,
SkyrimModDevKit/Extern headers, and the existing fmt/spdlog dependency prefix.

Checks: tests/inspect-runtime.py, tests/runtime-helpers.cpp, matching DLL/PDB,
package file hashes, original PNG signatures and dMenu JSON parsing.
These are offline checks, not an in-game compatibility guarantee.

In-game test: sneak near a neutral NPC, become detected, repeat with hostile NPCs,
turn the camera, enter/leave combat, open/close menus and reload a save. Check
MaxsuDM reload after changing the INI. Report MaxsuDetectionMeter.log and a crash
log if needed. Gameplay and compatibility with the complete mod list are untested.
