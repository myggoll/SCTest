#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"

#define PTM_RATIO   32.0f

USING_NS_CC;
USING_NS_TIMELINE;

enum Node_Tag
{
	Tag_MyBlock=1,
	Tag_OtherBlock,
	Tag_Skill,
};


#define BlockCategory_My 0x0001
#define BlockCategory_Other 0x0002 
#define ProjectileCategory_My 0x0004
#define ProjectileCategory_Other 0x0008
#define SkillAttackAreaCategory_My 0x0020
#define SkillAttackAreaCategory_Other 0x0040

#define SCREEN_TO_BOX2D(fVal) (fVal/PTM_RATIO)
#define BOX2D_TO_SCREEN(fVal) (fVal*PTM_RATIO)



class Block : public Node
{
public:
	CREATE_FUNC(Block);

	virtual bool init();

	b2Fixture* createBody_Box2d(Node_Tag _tag, b2World* _world);

private:
	Node* baseNode_;
};



class MyContactListener : public b2ContactListener
{
public:
	MyContactListener()
	{}
	~MyContactListener(){}
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold){}
	virtual void PostSolve(b2Contact* contact, const b2Manifold* oldManifold){}
};


class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	HelloWorld() : world_(nullptr)
		, screen_body_(nullptr)
		, m_debugDraw(PTM_RATIO)
	{

	}

private:
	void tick( float dt );

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent){return true;}
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent){}
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent){}
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

private:
	b2World *world_;
	b2Body *screen_body_;
	MyContactListener *contactListener_;
	GLESDebugDraw m_debugDraw;

	Block* myBlock_;
	Block* otherBlock_;
};

#endif // __HELLOWORLD_SCENE_H__
