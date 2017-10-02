/*
 * offsets.hpp
 *
 *  Created on: May 4, 2017
 *      Author: nullifiedcat
 */

#ifndef OFFSETS_HPP_
#define OFFSETS_HPP_

#include <stdint.h>
#include <exception>

enum class platform {
	PLATFORM_LINUX,
	PLATFORM_WINDOWS,
	PLATFORM_OSX,
	PLATFORM_UNSUPPORTED
};

#ifdef LINUX
constexpr platform PLATFORM = platform::PLATFORM_LINUX;
#else
constexpr platform PLATFORM = platform::PLATFORM_UNSUPPORTED;
#endif

struct offsets {
	static constexpr uint32_t PlatformOffset(uint32_t offset_linux, uint32_t offset_windows, uint32_t offset_osx) {
		uint32_t result = -1;
		switch (PLATFORM) {
		case platform::PLATFORM_LINUX:
			result = offset_linux; break;
		case platform::PLATFORM_WINDOWS:
			result = offset_windows; break;
		case platform::PLATFORM_OSX:
			result = offset_osx; break;
		}
		// pCompileError.
		//static_assert(result != -1, "No offset defined for this platform!");
		return result;
	}
	static constexpr uint32_t GetUserCmd() { 		return PlatformOffset(8, -1, -1); }
	static constexpr uint32_t ShouldDraw() { 		return PlatformOffset(136, -1, -1); }
	static constexpr uint32_t DrawModelExecute() { 	return PlatformOffset(19, -1, -1); }
	static constexpr uint32_t GetClientName() { 	return PlatformOffset(44, -1, -1); }
	static constexpr uint32_t ProcessSetConVar() { 	return PlatformOffset(4, -1, -1); }
	static constexpr uint32_t ProcessGetCvarValue() { 	return PlatformOffset(29, -1, -1); }
	static constexpr uint32_t GetFriendPersonaName() { 	return PlatformOffset(7, -1, -1); }
	static constexpr uint32_t CreateMove() { 		return PlatformOffset(22, -1, -1); }
	static constexpr uint32_t PaintTraverse() { 	return PlatformOffset(42, -1, -1); }
	static constexpr uint32_t OverrideView() { 		return PlatformOffset(17, -1, -1); }
	static constexpr uint32_t FrameStageNotify() { 	return PlatformOffset(35, -1, -1); }
	static constexpr uint32_t DispatchUserMessage() { 	return PlatformOffset(36, -1, -1); }
	static constexpr uint32_t CanPacket() { 	return PlatformOffset(57, -1, -1); }
	static constexpr uint32_t SendNetMsg() { 	return PlatformOffset(41, -1, -1); }
	static constexpr uint32_t Shutdown() { 	return PlatformOffset(37, -1, -1); }
	static constexpr uint32_t IN_KeyEvent() { 	return PlatformOffset(20, -1, -1); }
	static constexpr uint32_t HandleInputEvent() { 	return PlatformOffset(78, -1, -1); }
	static constexpr uint32_t LevelInit() { 		return PlatformOffset(23, -1, -1); }
	static constexpr uint32_t LevelShutdown() { 	return PlatformOffset(24, -1, -1); }
	static constexpr uint32_t BeginFrame() { 		return PlatformOffset(5, -1, -1); }

};

#endif /* OFFSETS_HPP_ */
