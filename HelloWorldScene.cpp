#include "xType.h"
#include "HelloWorldScene.h"

bool Block::init()
{
	baseNode_ = CSLoader::createNode("Block.csb");
	if (baseNode_ == nullptr)
	{
		log("Error - Not Find .csb : Block.csb");
		return false;
	}
	this->addChild(baseNode_);

	return true;
}

b2Fixture* Block::createBody_Box2d(Node_Tag _tag, b2World* _world)
{
	Sprite* collisionBox = (Sprite*)baseNode_->getChildByName("CollisionBox");

	Vec2 bodyPos = this->convertToWorldSpace(collisionBox->getPosition());
	bodyPos = this->getParent()->convertToNodeSpace(bodyPos);

	b2BodyDef block_body_def;
	block_body_def.type = b2_dynamicBody;
	block_body_def.position.Set(SCREEN_TO_BOX2D(bodyPos.x), SCREEN_TO_BOX2D(bodyPos.y));
	block_body_def.userData = collisionBox;

	b2Body* pBody = _world->CreateBody(&block_body_def);

	b2PolygonShape block_polygon;
	block_polygon.SetAsBox(SCREEN_TO_BOX2D(collisionBox->getScaleX()/2), SCREEN_TO_BOX2D(collisionBox->getScaleY()/2));
	b2FixtureDef block_fixture_def;
	block_fixture_def.shape = &block_polygon;
	block_fixture_def.isSensor = true;

	switch (_tag)
	{
	case Node_Tag::Tag_MyBlock :
		{
			block_fixture_def.filter.categoryBits = ProjectileCategory_My;
			block_fixture_def.filter.maskBits = 0xFFFF ^ (BlockCategory_My | ProjectileCategory_My | SkillAttackAreaCategory_My);
		}
		break;
	case Node_Tag::Tag_OtherBlock:
		{
			block_fixture_def.filter.categoryBits = ProjectileCategory_Other;
			block_fixture_def.filter.maskBits = 0xFFFF ^ (BlockCategory_Other | ProjectileCategory_Other | SkillAttackAreaCategory_Other);
		}
		break;
	}


	return pBody->CreateFixture(&block_fixture_def);
}


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

void MyContactListener::BeginContact( b2Contact* contact )
{
	log("Begin Contact");
}

void MyContactListener::EndContact( b2Contact* contact )
{
	log("End Contact");
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

	//Create World
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	world_ = new b2World(gravity);
	world_->SetAllowSleeping(false);

	b2BodyDef screen_body_def;
	screen_body_def.position.Set(0.0f, 0.0f);
	screen_body_ = world_->CreateBody(&screen_body_def);

	world_->SetDebugDraw(&m_debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_pairBit;
	m_debugDraw.SetFlags(flags);

	contactListener_ = new MyContactListener();
	world_->SetContactListener(contactListener_);


	this->schedule(schedule_selector(HelloWorld::tick));

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);

	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	myBlock_ = Block::create();
	myBlock_->setPosition(100, 300);
	myBlock_->setTag(Node_Tag::Tag_MyBlock);
	this->addChild(myBlock_);
	myBlock_->createBody_Box2d(Node_Tag::Tag_MyBlock, world_);

	Block* pTest = Block::create();
	pTest->setPosition(300, 300);
	pTest->setTag(Node_Tag::Tag_MyBlock);
	this->addChild(pTest);
	pTest->createBody_Box2d(Node_Tag::Tag_MyBlock, world_);

	pTest = Block::create();
	pTest->setPosition(500, 300);
	pTest->setTag(Node_Tag::Tag_OtherBlock);
	this->addChild(pTest);
	pTest->createBody_Box2d(Node_Tag::Tag_OtherBlock, world_);

	otherBlock_ = Block::create();
	otherBlock_->setPosition(700, 300);
	otherBlock_->setTag(Node_Tag::Tag_OtherBlock);
	this->addChild(otherBlock_);
	otherBlock_->createBody_Box2d(Node_Tag::Tag_OtherBlock, world_);

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

void HelloWorld::draw( Renderer *renderer, const Mat4& transform, uint32_t flags )
{
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	world_->DrawDebugData();
	kmGLPopMatrix();
}

void HelloWorld::tick( float dt )
{
	for (b2Body *b = world_->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			Node *node = (Node*)b->GetUserData();
			Vec2 pos = node->getParent()->convertToWorldSpace(node->getPosition());
			pos = this->convertToNodeSpace(pos);
			b->SetTransform(b2Vec2(SCREEN_TO_BOX2D(pos.x), SCREEN_TO_BOX2D(pos.y)), b->GetAngle());
		}
	}

	world_->Step(dt, 8, 3);

	//for (b2Body *b = world_->GetBodyList(); b; b = b->GetNext()) {
	//	if (b->GetUserData() != NULL) {
	//		Node *node = (Node*)b->GetUserData();
	//		node->setPosition(node->getParent()->convertToNodeSpace(Vec2(BOX2D_TO_SCREEN(b->GetPosition().x), BOX2D_TO_SCREEN(b->GetPosition().y))));
	//		node->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
	//	}
	//}
}

void HelloWorld::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPos = this->convertTouchToNodeSpace(pTouch);

	MoveTo* pMoveTo = MoveTo::create(myBlock_->getPosition().distance(touchPos) / 200, touchPos);
	myBlock_->runAction(pMoveTo);
}




