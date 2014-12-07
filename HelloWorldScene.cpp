#include "HelloWorldScene.h"

USING_NS_CC;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    // position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x - 100, visibleSize.height / 2 + origin.y - 100));
	//sprite->setVisible(false);
    // add the sprite as a child to this layer
    this->addChild(sprite, 1);

	auto sprite2 = Sprite::create("HelloWorld.png");
	sprite2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 100, visibleSize.height / 2 + origin.y + 100));
	this->addChild(sprite2, 1);

	Rect intersection;
	Rect r1 = sprite->boundingBox();
	Rect r2 = sprite2->boundingBox();
	intersection = CCRectMake(max(r1.getMinX(), r2.getMinX()), max(r1.getMinY(), r2.getMinY()), 0, 0);
	intersection.size.width = min(r1.getMaxX(), r2.getMaxX()) - intersection.getMinX();
	intersection.size.height = min(r1.getMaxY(), r2.getMaxY()) - intersection.getMinY();

	unsigned int x = intersection.origin.x;
	unsigned int y = intersection.origin.y;
	unsigned int w = intersection.size.width;
	unsigned int h = intersection.size.height;
	unsigned int numPixels = w * h;


	RenderTexture* pRT = RenderTexture::create(visibleSize.width, visibleSize.height);
	//pRT->setPosition(this->getAnchorPointInPoints().x - 150, this->getAnchorPointInPoints().y - 150);

	pRT->beginWithClear(0, 0, 0, 0);
	glColorMask(1, 0, 0, 1);
	sprite->visit();
	glColorMask(0, 1, 0, 1);
	sprite2->visit();

	Color4B *buffer = (Color4B *)malloc(sizeof(Color4B) * numPixels);
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glColorMask(1, 1, 1, 1);
	pRT->end();
	//pRT->setPosition(this->getAnchorPointInPoints().x - 150, this->getAnchorPointInPoints().y - 150);
	//this->addChild(pRT, 2);

	free(buffer);



    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
