# Skyrim 1.7.104 render-hook compatibility hardening

This source tree includes a compatibility hardening pass for the Detection Meter
HUD renderer after a crash was observed in a render/HUD hook chain containing
ImprovedCameraSE, CommunityShaders and SKSEMenuFramework.

Changes:

1. `Renderer::MenuPresentHook::Hook_PostDisplay` now calls the previous/original
   `PostDisplay` hook first, then renders Detection Meter. This prevents Detection
   Meter from changing D3D state before downstream HUD/camera hooks execute.
2. The bundled Dear ImGui DX11 backend now backs up and restores Hull, Domain and
   Compute shader state (including class instances). ImGui 1.89.9 clears these
   stages but normally does not restore them.
3. Geometry-shader class instances are released after restoration to avoid a
   per-frame COM reference leak.
4. Detection Meter verifies its D3D device/context and ImGui context before
   beginning a frame.

The matching DLL/PDB build containing these changes was confirmed in game on
Skyrim 1.7.104.0.

Recommended regression test:
- Skyrim 1.7.104 + matching SKSE/Address Library
- Detection Meter + Improved Camera + Community Shaders + SKSE Menu Framework
- Test sneaking/detection for 15-30 minutes, opening/closing HUD menus, changing
  first/third person, loading a save and changing cells.
- If a crash remains, retain the PDB and capture a new Crash Logger log so the
  new call site can be compared with the previous `HUDMenu::PostDisplay` crash.
