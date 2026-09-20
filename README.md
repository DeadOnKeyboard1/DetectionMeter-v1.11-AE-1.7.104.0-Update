# Detection Meter 1.1.3 - Skyrim 1.7.104.0

Source for the Skyrim 1.7.104.0 and SKSE 2.3.1 update of Detection Meter by
max-su-2019. It retains the original interface and configuration while adding
the confirmed render-hook compatibility fix and excluding the player's
followers, companions, summons, commanded actors and reanimated allies from
the overlay.

## Compatibility

- Skyrim runtime 1.7.104.0
- SKSE 2.3.1
- Address Library for runtime 1.7.104.0
- dMenu is optional

This build is runtime-specific. Older Skyrim runtimes are not supported by the
published DLL.

## Build

Initialize the pinned Dear ImGui submodule, then configure `cmake-ae` with an
x64 MSVC environment and explicit dependency paths:

```powershell
git submodule update --init --recursive
cmake -S cmake-ae -B build-ae -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DCMAKE_PREFIX_PATH="$env:DEPENDENCY_PREFIX" `
  -DCOMMONLIBSSE_SOURCE_DIR="$env:COMMONLIBSSE_SOURCE_DIR" `
  -DCOMMONLIBSSE_COMPAT_INCLUDE_DIR="$env:COMMONLIBSSE_COMPAT_INCLUDE_DIR" `
  -DDKUTIL_INCLUDE_DIR="$env:DKUTIL_INCLUDE_DIR" `
  -DSIMPLEINI_INCLUDE_DIR="$env:SIMPLEINI_INCLUDE_DIR"
cmake --build build-ae --config RelWithDebInfo
```

The confirmed build used CommonLibSSE-NG/CommonLibVR 8.0.0 based on commit
`1504349dddfc622d4d25704bba19e2ade669dc5a`, with the workspace 1.7.104
compatibility patch SHA-256
`600DFC3EBB3FBEA2C3D948CBDE5368AB25BEC6E2ABB92EEF211E6E898BDA363A`,
and MSVC 19.51.36256. The installed neutral library was paired with the
format-5-compatible headers through `COMMONLIBSSE_COMPAT_INCLUDE_DIR`.

Dear ImGui is pinned to commit
`c6e0284ac58b3f205c95365478888f7b53b077e2` (v1.89.9). The modified DX11
backend used by this port is stored in `include/external` so the submodule
remains reproducible and clean.

## License

This update is distributed under GPL-3.0-or-later; see `LICENSE` and
`EXCEPTIONS.md`. The original Detection Meter copyright and MIT permission
notice are preserved in `licenses/DetectionMeter-MIT.txt`. Directly included
third-party components retain the notices in `licenses`.
