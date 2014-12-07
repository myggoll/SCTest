#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

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

	/*virtual void TouchesBegan(Set *pTouches, Event *pEvent);
	virtual void TouchesMoved(Set *pTouches, Event *pEvent);
	virtual void TouchesEnded(Set *pTouches, Event *pEvent);
	virtual void TouchesCancelled(Set *pTouches, Event *pEvent);*/

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

	Sprite *_ship;
	Sprite *_enemy1;
	Sprite *_enemy2;
	RenderTexture* _rt;
	Sprite *m_pBrush;

private:
	Point m_tBeginPos;
	Point m_tEndPos;

	void	tick(float dt);
	void	checkCollisions();
	bool	isCollisionBetweenSpriteA(cocos2d::CCSprite* spr1, cocos2d::CCSprite* spr2, bool pp);
};

#endif // __HELLOWORLD_SCENE_H__
