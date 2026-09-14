#include "Hook.h"
#include "Renderer.h"

DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) {
		Sleep(100);
	}
#endif

	if (a_skse->RuntimeVersion() != REL::Version{1, 7, 104, 0}) return false;
	auto logPath = SKSE::log::log_directory();
	if (!logPath) return false;
	std::filesystem::create_directories(*logPath);
	*logPath /= "MaxsuDetectionMeter.log";
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath->string(), true);
	spdlog::set_default_logger(std::make_shared<spdlog::logger>("MaxsuDetectionMeter", sink));
	spdlog::flush_on(spdlog::level::info);

	SKSE::Init(a_skse);

	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version.string());

	// do stuff
	if (!MaxsuDetectionMeter::Renderer::Install()) return false;
	MaxsuDetectionMeter::CharacterEx::InstallHook();

	return true;
}
