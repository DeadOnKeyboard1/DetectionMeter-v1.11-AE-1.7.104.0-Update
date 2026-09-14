#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <numbers>
#include <vector>
#include <cstdio>

namespace RE {
struct Actor {
    bool summoned{}, player{};
    Actor* commander{};
    bool teammate{};
    bool IsSummoned() const { return summoned; }
    Actor* GetCommandingActor() const { return commander; }
    bool IsPlayerRef() const { return player; }
    bool IsPlayerTeammate() const { return teammate; }
};
struct NiPoint3 { float x{}, y{}, z{}; };
struct NiTransform { struct { float entry[3][3]{}; } rotate; NiPoint3 translate; };
struct UserEvents { enum class INPUT_CONTEXT_ID : std::uint32_t { kGameplay, kMenu }; };
template<class T> using BSTArray = std::vector<T>;
struct ControlMap {
    std::byte prefix[0x108]{};
    BSTArray<UserEvents::INPUT_CONTEXT_ID> stack;
    std::uint32_t flags{};
    static inline ControlMap* singleton{};
    static ControlMap* GetSingleton() { return singleton; }
};
static_assert(offsetof(ControlMap, flags) == 0x120);
}
#include "../src/Runtime.h"
int main()
{
    using namespace MaxsuDetectionMeter;
    assert(!GameplayControlsAvailable());
    RE::ControlMap map;
    RE::ControlMap::singleton = &map;
    map.flags = 0x81;
    assert(!GameplayControlsAvailable());
    map.stack.push_back(RE::UserEvents::INPUT_CONTEXT_ID::kGameplay);
    assert(GameplayControlsAvailable());
    map.stack.push_back(RE::UserEvents::INPUT_CONTEXT_ID::kMenu);
    assert(!GameplayControlsAvailable());
    map.stack.pop_back();
    map.flags = 1;
    assert(!GameplayControlsAvailable());
    RE::Actor player{false, true}, summon{true, false, &player};
    assert(IsPlayerAlly(player));
    assert(IsPlayerAlly(summon));
    summon.commander = nullptr;
    assert(!IsPlayerAlly(summon));
    RE::Actor follower{false, false, nullptr, true}, stranger;
    assert(IsPlayerAlly(follower));
    assert(!IsPlayerAlly(stranger));
    summon.commander = &follower;
    assert(IsPlayerAlly(summon));
    RE::Actor nested{true, false, &summon};
    assert(IsPlayerAlly(nested));
    summon.summoned = false;
    assert(IsPlayerAlly(summon)); // Reanimated/commanded ally.
    follower.teammate = false;
    assert(!IsPlayerAlly(summon)); // Dismissed follower, no stale cache.
    summon.commander = &stranger;
    assert(!IsPlayerAlly(summon)); // Enemy/neutral summon remains visible.
    stranger.commander = &summon;
    assert(!IsPlayerAlly(summon)); // Cycle terminates.
    RE::NiTransform camera;
    camera.rotate.entry[0][0] = 1;
    assert(std::abs(RelativeHeading(camera, {0, 1, 0})) < 0.001f);
    assert(std::abs(RelativeHeading(camera, {1, 0, 0}) - 90) < 0.001f);
    assert(std::abs(RelativeHeading(camera, {-1, 0, 0}) + 90) < 0.001f);
    camera.rotate.entry[0][0] = 0;
    camera.rotate.entry[0][1] = 1;
    assert(std::abs(RelativeHeading(camera, {1, 0, 0})) < 0.001f);
    std::puts("PASS: control contexts/flags, summoned actor ownership, relative heading. Mock tests only.");
}
