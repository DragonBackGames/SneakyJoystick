#include "SneakyButton.h"

using namespace cocos2d;

bool SneakyButton::initWithRect(Rect rect)
{
    bounds = CCRectMake(0, 0, rect.size.width, rect.size.height);
    center = CCPointMake(rect.size.width/2, rect.size.height/2);
    status = 1; //defaults to enabled
    active = false;
    value = 0;
    isHoldable = 0;
    isToggleable = 0;
    radius = 32.0f;
    rateLimit = 1.0f/120.0f;
    
    setPosition(rect.origin); //not sure about this
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = SneakyButton::onTouchBegan;
    listener->onTouchMoved = SneakyButton::onTouchMoved;
    listener->onTouchEnded = SneakyButton::onTouchEnded;
    listener->onTouchCancelled = SneakyButton::onTouchCancelled;
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
	return true;
}

void SneakyButton::limiter(float delta)
{
	value = 0;
	this->unschedule(schedule_selector(SneakyButton::limiter));
	active = false;
}

void SneakyButton::setRadius(float r)
{
	radius = r;
	radiusSq = r*r;
}

bool SneakyButton::onTouchBegan(Touch *touch, Event *event)
{
    auto target = static_cast<SneakyButton*>(event->getCurrentTarget());

	if (target->getIsActive())
    {
        return false;
    }
	
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = target->convertToNodeSpace(location);
		//Do a fast rect check before doing a circle hit check:
	if(location.x < -target->getRadius() || location.x > target->getRadius() || location.y < -target->getRadius() || location.y > target->getRadius())
    {
		return false;
	}
    else
    {
		float dSq = location.x*location.x + location.y*location.y;
		if(target->getRadiusSq() > dSq){
			target->setIsActive(true);
			if (!target->getIsHoldable() && !target->getIsToggleable())
            {
				target->setValue(1);
				target->schedule(schedule_selector(SneakyButton::limiter), target->getRateLimit());
			}
			if (target->getIsHoldable())
            {
                target->setValue(1);
            }
			if (target->getIsToggleable())
            {
                target->setValue(!target->getValue());
            }
			return true;
		}
	}
return false;
}

void SneakyButton::onTouchMoved(Touch *touch, Event *event)
{
    auto target = static_cast<SneakyButton*>(event->getCurrentTarget());

	if (!target->getIsActive())
    {
        return;
    }
    
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = target->convertToNodeSpace(location);
		//Do a fast rect check before doing a circle hit check:
	if(location.x < -target->getRadius() || location.x > target->getRadius() || location.y < -target->getRadius() || location.y > target->getRadius())
    {
		return;
	}
    else
    {
		float dSq = location.x*location.x + location.y*location.y;
		if(target->getRadiusSq() > dSq)
        {
			if (target->getIsHoldable())
            {
                target->setValue(1);
            }
		}
		else
        {
			if (target->getIsHoldable())
            {
                target->setValue(0);
                target->setIsActive(false);
            }
		}
	}
}

void SneakyButton::onTouchEnded(Touch *touch, Event *event)
{
    auto target = static_cast<SneakyButton*>(event->getCurrentTarget());

	if (!target->getIsActive())
    {
        return;
    }
	if (target->getIsHoldable())
    {
        target->setValue(0);
    }
	if (target->getIsHoldable()||target->getIsToggleable())
    {
        target->setIsActive(false);
    }
}

void SneakyButton::onTouchCancelled(Touch *touch, Event *event)
{
    auto target = static_cast<SneakyButton*>(event->getCurrentTarget());

	target->onTouchEnded(touch, event);
}

void SneakyButton::touchDelegateRelease()
{
	this->release();
}

void SneakyButton::touchDelegateRetain()
{
	this->retain();
}
