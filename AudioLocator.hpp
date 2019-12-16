/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - Service locator for audio services.
 */

#pragma once
#include <memory>
#include "IAudioService.hpp"
#include "NullAudio.hpp"

class AudioLocator {
public:

	static void initialize();
	static std::shared_ptr<IAudioService> getService();
	static void setService(std::shared_ptr<IAudioService> service);

private:
	static std::shared_ptr<IAudioService> _service;
	static std::shared_ptr<NullAudio> _nullService;
};