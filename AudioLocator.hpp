/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - Service locator for audio services.
 */

#pragma once
#include <memory>
#include "AudioService.hpp"
#include "NullAudio.hpp"

class AudioLocator {
public:

	static void initialize();
	static std::shared_ptr<AudioService> getService();
	static void setService(std::shared_ptr<AudioService> service);

private:
	static std::shared_ptr<AudioService> _service;
	static std::shared_ptr<NullAudio> _nullService;
};