#pragma once

namespace MaxsuDetectionMeter
{
    inline bool IsPlayerAlly(const RE::Actor& actor)
    {
        if (actor.IsPlayerRef() || actor.IsPlayerTeammate()) return true;
        // Follow ownership rather than the summoned flag, which can omit
        // reanimated/commanded actors. Bound malformed or cyclic chains.
        auto commander = actor.GetCommandingActor();
        for (unsigned depth = 0; commander && depth < 8; ++depth) {
            if (commander->IsPlayerRef() || commander->IsPlayerTeammate()) return true;
            commander = commander->GetCommandingActor();
        }
        return false;
    }

    inline float RelativeHeading(const RE::NiTransform& camera, const RE::NiPoint3& target)
    {
        const auto x = camera.rotate.entry[0][0];
        const auto y = camera.rotate.entry[0][1];
        const float yaw = std::hypot(x, y) < 1e-6f ? 0.f : std::atan2(y, x);
        const float bearing = std::atan2(target.x - camera.translate.x, target.y - camera.translate.y);
        return std::remainder((bearing - yaw) * 180.f / std::numbers::pi_v<float>, 360.f);
    }

    inline bool GameplayControlsAvailable()
    {
        auto map = RE::ControlMap::GetSingleton();
        if (!map) return false;
        auto base = reinterpret_cast<const std::byte*>(map);
        const auto flags = *reinterpret_cast<const std::uint32_t*>(base + 0x120);
        const auto& stack = *reinterpret_cast<const RE::BSTArray<RE::UserEvents::INPUT_CONTEXT_ID>*>(base + 0x108);
        return (flags & 0x81) == 0x81 && !stack.empty() && stack.back() == RE::UserEvents::INPUT_CONTEXT_ID::kGameplay;
    }
}
