#include "AudioLocator.hpp"

std::shared_ptr<IAudioService> AudioLocator::_service;
std::shared_ptr<NullAudio> AudioLocator::_nullService;


void AudioLocator::initialize() { 
	_service = _nullService = std::make_shared<NullAudio>(); 
}

std::shared_ptr<IAudioService> AudioLocator::getService() { 
	return _service; 
}

void AudioLocator::setService(std::shared_ptr<IAudioService> service) {
	if (service == NULL)
		_service = _nullService;
	else
		_service = service;
}