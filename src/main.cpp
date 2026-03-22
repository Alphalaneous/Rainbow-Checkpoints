#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "Utils.h"

using namespace geode::prelude;

class $modify(PlayLayer) {

    void updateCheckpoints() {
		
		if (auto obj = static_cast<CheckpointObject*>(m_checkpointArray->lastObject())) {
			
			if (obj->m_physicalCheckpointObject->getChildByID("outer"_spr)) return;

			auto color = Mod::get()->getSettingValue<ccColor3B>("starting-color"); 

			if (m_checkpointArray->count() >= 2) {
				if (auto obj2 = static_cast<CheckpointObject*>(m_checkpointArray->objectAtIndex(m_checkpointArray->count() - 2))) {
					auto outer2 = obj2->m_physicalCheckpointObject->getChildByID("outer"_spr);
					auto inner2 = outer2->getChildByID("inner"_spr);

					color = static_cast<CCSprite*>(inner2)->getColor();
				}
			}

			obj->m_physicalCheckpointObject->m_addToNodeContainer = true;

			auto outerSpr = CCSprite::create("checkpoint_outer.png"_spr);
			outerSpr->setID("outer"_spr);
			outerSpr->setCascadeColorEnabled(true);
			outerSpr->setCascadeOpacityEnabled(true);

			auto innerSpr = CCSprite::create("checkpoint_inner.png"_spr);
			innerSpr->setID("inner"_spr);

			auto blank = CCSprite::create();

			outerSpr->setPosition(obj->m_physicalCheckpointObject->getContentSize()/2);
			innerSpr->setPosition(outerSpr->getContentSize()/2);

			auto origSize = obj->m_physicalCheckpointObject->getContentSize();

			obj->m_physicalCheckpointObject->setTexture(blank->getTexture());
			obj->m_physicalCheckpointObject->setTextureRect(blank->getTextureRect());
			obj->m_physicalCheckpointObject->setContentSize(origSize);
			obj->m_physicalCheckpointObject->setCascadeOpacityEnabled(true);
			obj->m_physicalCheckpointObject->setCascadeColorEnabled(true);

			auto hsv = Utils::rgbToHsv(color);
			hsv.h += Mod::get()->getSettingValue<int64_t>("hue-shift");
			if (hsv.h >= 360) hsv.h -= 360;

			innerSpr->setColor(Utils::hsvToRgb(hsv));

			obj->m_physicalCheckpointObject->addChild(outerSpr);
			outerSpr->addChild(innerSpr);
		}
	}

	void storeCheckpoint(CheckpointObject* p0) {
		PlayLayer::storeCheckpoint(p0);
		updateCheckpoints();
	}
};
