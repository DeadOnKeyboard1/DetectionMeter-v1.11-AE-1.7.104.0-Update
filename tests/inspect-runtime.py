from pathlib import Path
import struct
import pefile

root = Path(__file__).resolve().parents[2]
pe = pefile.PE(r'D:\SteamLibrary\steamapps\common\Skyrim Special Edition\SkyrimSE.exe', fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
ids = {}
for line in (root / 'adresslibrary/offsets-1-7-104-0.txt').read_text().splitlines():
    fields = line.split()
    if len(fields) == 2:
        ids[int(fields[0])] = int(fields[1], 16)

def pointer(va):
    return struct.unpack('<Q', pe.get_data(va - base, 8))[0]

for identifier, name, slot in [(207886, 'Character', 0xAD), (215362, 'HUDMenu', 6)]:
    table = ids[identifier]
    col = pointer(table - 8)
    type_rva = struct.unpack('<I', pe.get_data(col - base + 12, 4))[0]
    rtti = pe.get_data(type_rva + 16, 120).split(b'\0')[0].decode()
    assert rtti == f'.?AV{name}@@', rtti
    target = pointer(table + slot * 8) - base
    assert pe.get_section_by_rva(target).Characteristics & 0x20000000
    if name == 'Character':
        assert target == 0x67A7E0
        assert pe.get_data(target + 0x42, 3) == bytes.fromhex('0f 28 f1')
    print(f'PASS: {name} RTTI and hook slot {slot:X}: RVA {target:X}')

site = ids[77226] - base + 0x275
call = pe.get_data(site, 5)
assert call[0] == 0xE8
assert site + 5 + struct.unpack('<i', call[1:])[0] == 0x1012060
assert ids[411393] - base == 0x3331F40
print('PASS: D3D init call boundary/target and renderer singleton match current runtime/SKSE.')

table = ids[365650]
matches = []
for index in range(0x1B4):
    entry = table + index * 0x50
    name = pe.get_data(pointer(entry) - base, 100).split(b'\0')[0]
    if name == b'ShowWhoDetectsPlayer':
        matches.append(index)
assert len(matches) == 1
print('PASS: original console command replacement target exists:', matches[0])

project = Path(__file__).resolve().parents[1]
dll = pefile.PE(str(project / 'build-ae/RelWithDebInfo/MaxsuDetectionMeter.dll'))
exports = {s.name.decode(): s.address for s in dll.DIRECTORY_ENTRY_EXPORT.symbols if s.name}
assert {'SKSEPlugin_Version', 'SKSEPlugin_Load', 'SKSEPlugin_Query'} <= exports.keys()
print('PASS: SKSE plugin exports present.')
for name in ['Hook.cpp', 'Renderer.cpp']:
    source = (project / 'src' / name).read_text()
    assert 'contextPriorityStack.back()' not in source
    assert 'IsMovementControlsEnabled()' not in source
print('PASS: legacy ControlMap accesses removed.')
