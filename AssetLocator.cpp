#include "AssetLocator.hpp"

std::shared_ptr<IAssetService> AssetLocator::_service;
std::shared_ptr<DefaultAssetManager> AssetLocator::_defaultService;


void AssetLocator::initialize() {
	_service = _defaultService = std::make_shared<DefaultAssetManager>();
	_service->loadAssets();
}

std::shared_ptr<IAssetService> AssetLocator::getService() {
	return _service;
}

void AssetLocator::setService(std::shared_ptr<IAssetService> service) {
	if (service == NULL)
		_service = _defaultService;
	else
		_service = service;

	_service->loadAssets();
}