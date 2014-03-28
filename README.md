SneakyJoystick
==============

SneakyJoystick is a library that provides a 'joystick-like' features to a layer in Cocos2d-x. The library support:

- Joystick Thumb
- Joystick D-Pad
- Regular Buttons
- Holdable Buttons
- Toggleable Buttons

The project was originally created CJ Hanson (https://github.com/cjhanson). This is a port for Cocos2d-x version 2.2.1 and 3.0rc0

How to Use the Library
======================

The best way to use the library is to create a JoystickLayer, and then add that layer to your Game Scene.

This is an example on how to create a Joystick Thumb

	CCRect joystickBaseDimensions;
	joystickBaseDimensions = CCRectMake(0, 0, 160.0f, 160.0f);    

	CCPoint joystickBasePosition;
	joystickBasePosition = ccp(screenSize.width * 0.2f, screenSize.height*0.2f);

	SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
	joystickBase->init();
	joystickBase->setPosition(joystickBasePosition);
	joystickBase->setBackgroundSprite(CCSprite::createWithSpriteFrameName("backgroundJoy.png"));
	joystickBase->setThumbSprite(CCSprite::createWithSpriteFrameName("thumbJoy.png"));
    
	SneakyJoystick *aJoystick = new SneakyJoystick();
	aJoystick->initWithRect(joystickBaseDimensions);
	aJoystick->autorelease();
	joystickBase->setJoystick(aJoystick);
	joystickBase->setPosition(joystickBasePosition);

	leftJoystick = joystickBase->getJoystick();
	leftJoystick->retain();
	this->addChild(joystickBase);	
	
To use it you just need to call 'getStickPosition'

This is an example on how to create a Joystick Button

    CCRect attackButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f);
    CCPoint attackButtonPosition;
	attackButtonPosition = ccp(screenSize.width * 0.9f, screenSize.height * 0.2f);
    
	SneakyButtonSkinnedBase *attackButtonBase = new SneakyButtonSkinnedBase();
    attackButtonBase->init();
	attackButtonBase->setPosition(attackButtonPosition);
    
    attackButtonBase->setDefaultSprite(CCSprite::createWithSpriteFrameName("attackBtn.png"));
    attackButtonBase->setActivatedSprite(CCSprite::createWithSpriteFrameName("attackBtn.png"));
    attackButtonBase->setDisabledSprite(CCSprite::createWithSpriteFrameName("attackBtn.png"));
    attackButtonBase->setPressSprite(CCSprite::createWithSpriteFrameName("attackBtn.png"));
    
	SneakyButton *aAttackButton = new SneakyButton();
	aAttackButton->initWithRect(attackButtonDimensions);
	aAttackButton->autorelease();
    
	attackButtonBase->setButton(aAttackButton);
    attackButtonBase->setPosition(attackButtonPosition);
    
	attackButton = attackButtonBase->getButton();
    attackButton->retain();
	this->addChild(attackButtonBase);
    
	// Enable scheduler
    scheduleUpdate();
    this->setTouchEnabled(true);
    
To use it you just need to call 'getValue'
