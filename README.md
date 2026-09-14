# Detection Meter 1.1.3 - Skyrim 1.7.104.0 Update

Source for the Skyrim 1.7.104.0 compatibility update of Detection Meter. The
update retains the original meter graphics and behavior while excluding the
player's followers, companions, summons, and commanded or reanimated allies
from the overlay.

## Compatibility

- Skyrim Special Edition / Anniversary Edition runtime 1.7.104.0
- SKSE 2.3.1
- Address Library for SKSE Plugins for runtime 1.7.104.0
- dMenu is optional

See `UPDATE-1.7.104.md` for the implementation and verification record.

## Credits

Detection Meter was created by max-su-2019. The Skyrim 1.7.104.0 compatibility
update was prepared by DeadOnKeyboard1.

Dear ImGui is included as the pinned submodule commit
`c6e0284ac58b3f205c95365478888f7b53b077e2` (v1.89.9).

## Building the 1.7.104.0 update

Install Visual Studio 2022, CMake, vcpkg, and compatible CommonLibSSE, DKUtil,
and SimpleIni source trees. Initialize the ImGui submodule, set the dependency
paths, and configure the update build:

```powershell
git submodule update --init --recursive
cmake -S cmake-ae -B build-ae `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DCOMMONLIBSSE_SOURCE_DIR="$env:COMMONLIBSSE_SOURCE_DIR" `
  -DDKUTIL_INCLUDE_DIR="$env:DKUTIL_INCLUDE_DIR" `
  -DSIMPLEINI_INCLUDE_DIR="$env:SIMPLEINI_INCLUDE_DIR"
cmake --build build-ae --config RelWithDebInfo
```

The tested binary was built with a local format-5-capable CommonLibSSE tree.
Its complete Git revision was not embedded in the available DLL or PDB. The
historical MIT license shipped with that tested package is preserved. See
`THIRD_PARTY_NOTICES.md`.

## License

This update is distributed under GPL-3.0-or-later as stated in `LICENSE`. The
original Detection Meter source and copyright notice remain available under
their original MIT terms in `licenses/DetectionMeter-MIT.txt`. All bundled and
linked third-party components retain their own licenses.
