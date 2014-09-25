#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <Box2D/Box2D.h>
#include "GLES-Render.h"
#include "cocos2d.h"
#include <vector>
#define PTM_RATIO 0.0315
USING_NS_CC;
using namespace std;
class HelloWorld : public cocos2d::Layer,public b2ContactListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    /////////////////
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);
    void update(float delta);
    bool onTouchBegan(Touch* touch, Event* event);
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    //void onTouchMoved(Touch* touch, Event* event);
    //void onTouchEnded(Touch* touch, Event* event);
    virtual bool MouseDown(const b2Vec2& p);
    //virtual void MouseUp(const b2Vec2& p);
    // implement the "static create()" method manually
    
    CREATE_FUNC(HelloWorld);
    
    void moveBall();
    void myFunc1(float x1, float y1, float x2, float y2);
    bool myFunc2(float x, float y);
    float m_p1, m_p2;
    
    
    b2World* mWorld;
    b2Body * mBallBody;
    
    GLESDebugDraw *m_debugDraw;
    
    b2MouseJoint* m_mouseJoint;
    b2Vec2 m_mouseWorld;
    b2Body* m_groundBody;
    float m_x, m_y;
    b2Vec2 m_point;
    float velocity ;
};

class QueryCallback : public b2QueryCallback
{
public:
    QueryCallback(const b2Vec2& point)
    {
        m_point = point;
        m_fixture = nullptr;
    }
    
    virtual bool ReportFixture(b2Fixture* fixture)
    {
        b2Body* body = fixture->GetBody();
        if (body->GetType() == b2_dynamicBody)
        {
            bool inside = fixture->TestPoint(m_point);
            if (inside)
            {
                m_fixture = fixture;
                
                // We are done, terminate the query.
                return false;
            }
        }
        
        // Continue the query.
        return true;
    }
    
    b2Vec2 m_point;
    b2Fixture* m_fixture;
};

#endif // __HELLOWORLD_SCENE_H__
