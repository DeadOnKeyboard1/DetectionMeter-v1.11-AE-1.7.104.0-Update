# Third-Party Notices

## Build provenance

The tested Skyrim 1.7.104.0 binary was built from a local, format-5-capable
CommonLibSSE source tree formerly recorded as
`SkyrimModUpdateToolkit/CommonLibSSE-1.7.104`. The available artifacts do not
contain its complete Git revision. The package identified this as the
historical MIT-licensed CommonLibSSE generation, whose license is preserved.

DKUtil and SimpleIni were consumed as headers from local development trees.
The published build now accepts explicit dependency paths instead of relying
on machine-specific sibling directories.

Dear ImGui is included as a Git submodule at commit
`c6e0284ac58b3f205c95365478888f7b53b077e2` (v1.89.9).

## Components

| Component | Use | License | Notice file |
| --- | --- | --- | --- |
| Detection Meter update | Distributed source and binary | GPL-3.0-or-later | `LICENSE` |
| Original Detection Meter source | Original work | MIT | `licenses/DetectionMeter-MIT.txt` |
| CommonLibSSE | Statically linked library | MIT | `licenses/CommonLibSSE-MIT.txt` |
| DKUtil | Header dependency | MIT | `licenses/DKUtil-MIT.txt` |
| Dear ImGui 1.89.9 | Bundled UI library | MIT | `licenses/ImGui-MIT.txt` |
| SimpleIni | Header dependency | MIT | `licenses/SimpleIni-MIT.txt` |
| stb_image | Bundled image loader | MIT or public domain; MIT selected | `licenses/stb_image-MIT.txt` |
| spdlog | Logging dependency | MIT | `licenses/spdlog-MIT.txt` |

SKSE, Address Library, dMenu, Skyrim, Direct3D, and Windows are external
requirements and are not redistributed by this repository.
