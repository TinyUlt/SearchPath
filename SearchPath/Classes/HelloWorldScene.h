#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <Box2D/Box2D.h>
#include "GLES-Render.h"
#include "cocos2d.h"
#include <vector>
#define PTM_RATIO 0.0315
#define PI 3.14159265
USING_NS_CC;
using namespace std;

enum ShapeType{
    Box,
    Circle
};
struct BodyInformation
{
    float m_width;
    float m_height;
    float m_radian;
    ShapeType m_shapeType;
    Sprite* m_sprite;
    
};
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
    
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    void onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent);
    void onTouchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent);
    void onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent);
    virtual bool MouseDown(const b2Vec2& p);
    virtual void MouseUp(const b2Vec2& p);
    void MouseMove(const b2Vec2& p);

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
    
    //力的方向
    float m_x, m_y;
    //点击屏幕让小球到达的坐标
    b2Vec2 m_point;
    //世界中心
    b2Vec2 m_center;
    //是否需要重新计算力的方向
    bool m_isReCalculate;
    //小球的速度
    float velocity ;
    //小球到终点旧的长度
    float m_oldPathLenth;
    
    //手势缩放的长度
    float gesturesLenth;
    //是否能让小球移动(点击屏幕放开后移动小球)
    bool m_enabelMove;
    //是否人物在中间
    bool m_enableCenter;
    //2个触摸点开始的位置
    Vec2 m_gestureId0Point;
    Vec2 m_gestureId1Point;
    //单机时的点
    Vec2 m_oldPoint;
    //是否正在缩放
    bool m_isScaring;
    //地图大小
    Vec2 m_mapSize;
    //
    b2Joint* m_joint;
    //
    b2Body* m_touchObj;
    //
    float m_de;
    
    vector<b2Body*> m_allB2bodys;
    
    b2Vec2 getTeminalPoint(b2Vec2);
    float calculateRadianByTwoPoint(b2Vec2 p1, b2Vec2 p2);
    b2Vec2 calculateTeminalPointByRadianAndLenth(float radian, float lenth);
    float calculateLenthByTwoPoint(b2Vec2 p1, b2Vec2 p2);
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
        if (body->GetType() == b2_staticBody)
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
