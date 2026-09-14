const fs = require('node:fs');
const path = require('node:path');
const crypto = require('node:crypto');
const assert = require('node:assert/strict');
const root = path.resolve(__dirname, '..');
const output = path.join(root, 'package-ae');
const original = path.resolve(root, '../Detection Meter-v1.11');
const hashes = {};
function copy(source, name) {
  const bytes = fs.readFileSync(source);
  const dest = path.join(output, name);
  fs.mkdirSync(path.dirname(dest), {recursive:true});
  fs.writeFileSync(dest, bytes);
  hashes[name] = crypto.createHash('sha256').update(bytes).digest('hex');
}
for (const name of fs.readdirSync(original, {recursive:true})) {
  const source = path.join(original, name);
  if (!fs.statSync(source).isFile() || /\.(dll|pdb)$/i.test(name)) continue;
  const bytes = fs.readFileSync(source);
  if (name.endsWith('.png')) {
    assert.equal(bytes.subarray(0,8).toString('hex'), '89504e470d0a1a0a');
    assert.ok(bytes.readUInt32BE(16) > 0 && bytes.readUInt32BE(20) > 0);
  }
  if (name.endsWith('.json')) JSON.parse(bytes.toString());
  copy(source, name.replaceAll('\\','/'));
}
for (const ext of ['dll','pdb']) copy(path.join(root, `build-ae/RelWithDebInfo/MaxsuDetectionMeter.${ext}`), `SKSE/Plugins/MaxsuDetectionMeter.${ext}`);
for (const [file, label] of [['LICENSE','Maxsu'], ['external/imgui/LICENSE.txt','ImGui'], ['../CombatPathingRevolution-master/extern/DKUtil/LICENSE','DKUtil'], ['../SkyrimModUpdateToolkit/CommonLibSSE-1.7.104/LICENSE','CommonLib']])
  copy(path.resolve(root,file), `readmes/DetectionMeter-${label}-LICENSE.txt`);
copy(path.join(root,'UPDATE-1.7.104.md'), 'readmes/DetectionMeter-1.7.104.md');
for (const texture of ['Frame','Hostile','NonHostile']) assert.ok(hashes[`SKSE/Plugins/MaxsuDetectionMeter/Meter_${texture}.png`]);
assert.ok(hashes['SKSE/Plugins/dmenu/customSettings/MaxsuDetectionMeter.json']);
const actual = fs.readdirSync(output, {recursive:true}).filter(n=>fs.statSync(path.join(output,n)).isFile()).map(n=>n.replaceAll('\\','/')).sort();
assert.deepEqual(actual,Object.keys(hashes).sort());
fs.writeFileSync(path.join(root,'build-ae/package-hashes.json'),JSON.stringify(hashes,null,2));
console.log(`PASS: ${actual.length} files, three original textures, original INI/dMenu configuration, DLL and PDB.`);
