#include "Hook.h"
#include "DataHandler.h"
#include "Runtime.h"

namespace MaxsuDetectionMeter
{
	void CharacterEx::Hook_Update(float a_delta)
	{
		func(this, a_delta);

		if (!MeterHandler::ShouldDisplayMeter(this)) {
			auto handler = MeterHandler::GetSingleton();
			std::scoped_lock lock(handler->m_mutex);
			handler->meterArr.erase(formID);
			return;
		}

		auto playerref = RE::PlayerCharacter::GetSingleton();

		auto camera = RE::PlayerCamera::GetSingleton();
		auto cameraState = camera ? camera->currentState : nullptr;
		auto cameraRoot = camera ? camera->cameraRoot : nullptr;

		if (playerref && playerref->IsSneaking() &&
			cameraState && (cameraState->id == RE::CameraState::kFirstPerson || cameraState->id == RE::CameraState::kThirdPerson) && cameraRoot &&
			GameplayControlsAvailable()) {
			auto level = MeterHandler::ReCalculateDetectionLevel(RequestDetectionLevel(playerref));
			auto stealthPoint = MeterHandler::GetStealthPoint(this);
			auto meterHandler = MeterHandler::GetSingleton();

			RE::NiTransform CamTrans;
			CamTrans.rotate = cameraRoot->world.rotate;
			CamTrans.translate = playerref->GetPosition();

			auto const angle = RelativeHeading(CamTrans, GetPosition());
			std::scoped_lock lock(meterHandler->m_mutex);

			if (!meterHandler->meterArr.count(formID)) {
				if ((!playerref->IsInCombat() && (meterHandler->DisplayForNonCombat(this, level, playerref))) || (playerref->IsInCombat() && stealthPoint.has_value())) {
					auto meterObj = std::make_shared<MeterObj>(angle);
					meterHandler->meterArr.emplace(formID, meterObj);
					DEBUG("Add a Meter ID : {:x}", formID);
				}
			} else {
				auto it = meterHandler->meterArr.find(formID);
				if (it != meterHandler->meterArr.end() && it->second.load() && !it->second.load()->Update(this, angle, level))
					it->second.load()->MarkForRemove();
			}
		}
	}
}
