/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - Service locator for audio services.
 */

#pragma once
#include <memory>
#include "IAssetService.hpp"
#include "DefaultAssetManager.hpp"

class AssetLocator {
public:

	static void initialize();
	static std::shared_ptr<IAssetService> getService();
	static void setService(std::shared_ptr<IAssetService> service);

private:
	static std::shared_ptr<IAssetService> _service;
	static std::shared_ptr<DefaultAssetManager> _defaultService;
};