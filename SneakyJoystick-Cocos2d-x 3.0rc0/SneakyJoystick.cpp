#include "SneakyJoystick.h"

using namespace cocos2d;

#define SJ_PI 3.14159265359f
#define SJ_PI_X_2 6.28318530718f
#define SJ_RAD2DEG 180.0f/SJ_PI
#define SJ_DEG2RAD SJ_PI/180.0f

SneakyJoystick::~SneakyJoystick()
{
    
}

bool SneakyJoystick::initWithRect(Rect rect)
{
    stickPosition = CCPointZero;
    degrees = 0.0f;
    velocity = CCPointZero;
    autoCenter = true;
    isDPad = false;
    hasDeadzone = false;
    numberOfDirections = 4;
    
    this->setJoystickRadius(rect.size.width/2);
    this->setThumbRadius(32.0f);
    this->setDeadRadius(0.0f);
    
    //Cocos node stuff
    setPosition(rect.origin);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = SneakyJoystick::onTouchBegan;
    listener->onTouchMoved = SneakyJoystick::onTouchMoved;
    listener->onTouchEnded = SneakyJoystick::onTouchEnded;
    listener->onTouchCancelled = SneakyJoystick::onTouchCancelled;
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

float round(float r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

void SneakyJoystick::updateVelocity(Point point)
{
	// Calculate distance and angle from the center.
	float dx = point.x;
	float dy = point.y;
	float dSq = dx * dx + dy * dy;
	
	if(dSq <= deadRadiusSq)
    {
		velocity = CCPointZero;
		degrees = 0.0f;
		stickPosition = point;
		return;
	}

	float angle = atan2f(dy, dx); // in radians
	if(angle < 0)
    {
		angle		+= SJ_PI_X_2;
	}
	float cosAngle;
	float sinAngle;
	
	if(isDPad)
    {
		float anglePerSector = 360.0f / numberOfDirections * SJ_DEG2RAD;
		angle = round(angle/anglePerSector) * anglePerSector;
	}
	
	cosAngle = cosf(angle);
	sinAngle = sinf(angle);
	
	// NOTE: Velocity goes from -1.0 to 1.0.
	if (dSq > joystickRadiusSq || isDPad)
    {
		dx = cosAngle * joystickRadius;
		dy = sinAngle * joystickRadius;
	}
	
	velocity = CCPointMake(dx/joystickRadius, dy/joystickRadius);
	degrees = angle * SJ_RAD2DEG;
	
	// Update the thumb's position
	stickPosition = ccp(dx, dy);
}

void SneakyJoystick::setIsDPad(bool b)
{
	isDPad = b;
	if(isDPad){
		hasDeadzone = true;
		this->setDeadRadius(10.0f);
	}
}

void SneakyJoystick::setJoystickRadius(float r)
{
	joystickRadius = r;
	joystickRadiusSq = r*r;
}

void SneakyJoystick::setThumbRadius(float r)
{
	thumbRadius = r;
	thumbRadiusSq = r*r;
}

void SneakyJoystick::setDeadRadius(float r)
{
	deadRadius = r;
	deadRadiusSq = r*r;
}

bool SneakyJoystick::onTouchBegan(Touch *touch, Event *event)
{
	Point location = Director::sharedDirector()->convertToGL(touch->getLocationInView());

    auto target = static_cast<SneakyJoystick*>(event->getCurrentTarget());
    
	location = target->convertToNodeSpace(location);
	//Do a fast rect check before doing a circle hit check:
	if(location.x < -target->getJoystickRadius() || location.x > target->getJoystickRadius() || location.y < -target->getJoystickRadius() || location.y > target->getJoystickRadius())
    {
		return false;
	}
    else
    {
		float dSq = location.x*location.x + location.y*location.y;
		if(target->getJoystickRadiusSq() > dSq)
        {
			target->updateVelocity(location);
			return true;
		}
	}
	return false;
}

void SneakyJoystick::onTouchMoved(Touch *touch, Event *event)
{
	Point location = Director::sharedDirector()->convertToGL(touch->getLocationInView());
    auto target = static_cast<SneakyJoystick*>(event->getCurrentTarget());
	location = target->convertToNodeSpace(location);
	target->updateVelocity(location);
}

void SneakyJoystick::onTouchEnded(Touch *touch, Event *event)
{
	Point location = CCPointZero;
    auto target = static_cast<SneakyJoystick*>(event->getCurrentTarget());

	if(!target->getAutoCenter())
    {
		Point location = Director::sharedDirector()->convertToGL(touch->getLocationInView());
		location = target->convertToNodeSpace(location);
	}
	target->updateVelocity(location);
}

void SneakyJoystick::onTouchCancelled(Touch *touch, Event *event)
{
    auto target = static_cast<SneakyJoystick*>(event->getCurrentTarget());

	target->onTouchEnded(touch, event);
}

void SneakyJoystick::touchDelegateRelease()
{
	this->release();
}

void SneakyJoystick::touchDelegateRetain()
{
	this->retain();
}
