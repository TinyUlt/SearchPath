#include "HelloWorldScene.h"
//#include "GB2ShapeCache-x.h"
USING_NS_CC;

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

    // add "HelloWorld" splash screen
    
    
    
    
    velocity = 5;
    
    //设置地球和绘图
    {
        b2Vec2 gravity;
        gravity.Set(0.0f, 0.0f); //一个向下10单位的向量，作为重力减速度，Box2D中默认的单位是秒和米
        mWorld = new b2World(gravity); // 创建一个有重力加速度的世界
        m_debugDraw = new GLESDebugDraw(1/PTM_RATIO);   //这里新建一个 debug渲染模块
        //mWorld->SetDebugDraw(m_debugDraw);    //设置
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit ;
        //flags += b2Draw::e_centerOfMassBit;   //获取需要显示debugdraw的块
        //flags += b2Draw::e_aabbBit;  //AABB块
        //flags += b2Draw::e_centerOfMassBit; //物体质心
        flags += b2Draw::e_jointBit;  //关节
        //flags += b2Draw::e_shapeBit;   //形状
        //flags += b2Draw::e_centerOfMassBit;
        m_debugDraw->SetFlags(flags);   //需要显示那些东西
        mWorld->SetDebugDraw(m_debugDraw);
        mWorld->SetContactListener(this);
    }

    b2Body* ReBody;
    //创建地面和墙壁
    {
        //向世界申请一个物体
        b2BodyDef bodyDef;
        bodyDef.position.Set(0,2);
        m_groundBody = mWorld->CreateBody(&bodyDef);//添加地面
        
        //申请到之后设置物体属性
        {
            b2EdgeShape shape;
            b2FixtureDef fd;
            
            //fd.filter.categoryBits = grandMark;
            //fd.filter.maskBits = reMark | cirMark;//只和reMark 和 cirMark 碰撞
            
            
            fd.shape = &shape;
            
//            shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(Director::getInstance()->getWinSize().width * PTM_RATIO, 0.0f));
//            m_groundBody->CreateFixture(&fd);
//            
//            shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, Director::getInstance()->getWinSize().height * PTM_RATIO));
//            m_groundBody->CreateFixture(&fd);
//            
//            shape.Set(b2Vec2(Director::getInstance()->getWinSize().width * PTM_RATIO, 0.0f), b2Vec2(Director::getInstance()->getWinSize().width * PTM_RATIO, Director::getInstance()->getWinSize().height * PTM_RATIO));
//            m_groundBody->CreateFixture(&fd);
        }
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0;  j < 3; j++) {
                //向世界申请一个矩形
                b2BodyDef bodyDefRe;
                bodyDefRe.type = b2_staticBody;
                bodyDef.linearDamping = 0;
                bodyDef.angularDamping = 0;
                bodyDefRe.position.Set((Director::getInstance()->getWinSize().width/3 * i + 200 ) * PTM_RATIO,(Director::getInstance()->getWinSize().height/3 * j + 130)* PTM_RATIO); //初始位置
                ReBody = mWorld->CreateBody(&bodyDefRe);
                
                //申请到之后设置物体属性
                {
                    if (i ==1) {
                        b2PolygonShape shape;
                        shape.SetAsBox(3,2);
                        b2FixtureDef fixtureDef;
                        fixtureDef.shape = & shape;
                        fixtureDef.density = 1.0f;
                        fixtureDef.friction = 1.0f;
                        fixtureDef.restitution = 0.0f;
                        
                        ReBody->CreateFixture(&fixtureDef);
                    }
                    else
                    {
                        b2CircleShape shape;
                        shape.m_radius = 2.0f;
                        b2FixtureDef fixtureDef;
                        fixtureDef.shape = & shape;
                        fixtureDef.density = 0.0f;
                        fixtureDef.friction = 1.0f;
                        fixtureDef.restitution = 0.0f;
                        
                        ReBody->CreateFixture(&fixtureDef);
                    }
                    
                    
                }

            }
        }
        
        
    }

    
    //创建小球
    {
        //向世界申请一个物体
        b2BodyDef bodyDef;
        //bodyDef.linearVelocity.Set(-10.0f, -10.0f);
        bodyDef.type = b2_dynamicBody;
        bodyDef.linearDamping = 0;
        bodyDef.angularDamping = 0;
        bodyDef.position.Set((Director::getInstance()->getWinSize().width - 400) * PTM_RATIO/2,Director::getInstance()->getWinSize().height * PTM_RATIO/2); //初始位置
        mBallBody = mWorld->CreateBody(&bodyDef);
        
        //申请到之后设置物体属性
        {
            b2CircleShape shape;
            shape.m_radius = 1.0f;
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = & shape;
            fixtureDef.density = 0.0f;
            fixtureDef.friction = 10.0f;
            fixtureDef.restitution = 0.0f;
            //fixtureDef.filter.groupIndex = -8;
            
            mBallBody->CreateFixture(&fixtureDef);
        }
        
        //让小球有冲力
        //body->ApplyForce(b2Vec2(1000,100),b2Vec2(0,0),true);
        //body->ApplyForceToCenter(b2Vec2(1000,-3000), true);
    }

    
    
    
    
    
    
    //添加触摸事件
    // Adds Touch Event Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);//
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -10);//先被触摸
    _touchListener = listener;

    this->scheduleUpdate();
    return true;
}
bool HelloWorld::onTouchBegan(Touch* touch, Event* event){
    auto touchLocation = touch->getLocation();
    
    auto nodePosition = convertToNodeSpace( touchLocation );//视图层不是当前场景大小, 所以需要转换视图
    log("Box2DView::onTouchBegan, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
    
    return MouseDown(b2Vec2(nodePosition.x*PTM_RATIO,nodePosition.y*PTM_RATIO));
    
}
bool HelloWorld::MouseDown(const b2Vec2& p)
{
    m_point = p;
    mBallBody->SetAngularVelocity(0);

    return true;
}
void HelloWorld::moveBall()
{
    b2Vec2 _b2 =mBallBody->GetPosition();
    float _dtx = (m_point.x - _b2.x);
    float _dty = (m_point.y - _b2.y);
    float _tan = _dty/_dtx ;
    float _radian = tanh(_tan);
    //float velocity = 5;
    //float _x, _y;
    if (_dtx >= 0 ) {
        m_x = velocity * cos(_radian);
        m_y = velocity * sin(_radian);
    }
    else if ( _dtx <=0 ){
        m_x = -velocity * cos(_radian);
        m_y = -velocity * sin(_radian);
    }
    //mBallBody->ApplyForceToCenter(b2Vec2(m_x,m_y), true);
    mBallBody->SetLinearVelocity(b2Vec2(m_x,m_y));

}
void HelloWorld::update(float delta)
{
    int32 velocityIterations = 10;
    int32 positionIterations = 10;
    
    mWorld->Step(delta, velocityIterations, positionIterations);
    //mWorld->ClearForces();
    
//    mBallBody->ApplyForceToCenter(b2Vec2(10,0), true);
//    b2Vec2 _b2 = mBallBody->GetLinearVelocity();
//    float _tan = _b2.y/_b2.x ;
//    float _radian = tanh(_tan);
    //float velocity = 5;
//    float _x = velocity * sin(_radian);
//    float _y = velocity * cos(_radian);
    
//    log("%f, %f", _x, _y);
    moveBall();
}
void HelloWorld::myFunc1(float x1, float y1, float x2, float y2)
{
    m_p1 = (y1 -y2)/(x1-x2);
    m_p2 = y1 - m_p1 * x1;
}
bool HelloWorld::myFunc2(float x, float y)
{
    float _y = m_p1* x + m_p2;
    return _y>y? true : false;
}
void HelloWorld::BeginContact(b2Contact* contact)
{
    //B2_NOT_USED(contact);
    
}

void HelloWorld::EndContact(b2Contact* contact)
{
   mBallBody->SetAngularVelocity(0);
}

void HelloWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    
    b2Vec2 _ball;
    b2Vec2 _obj;
    if (mBallBody == contact->GetFixtureA()->GetBody()) {
        _ball = contact->GetFixtureA()->GetBody()->GetPosition();
        _obj = contact->GetFixtureB()->GetBody()->GetPosition();
    }
    else
    {
        _obj = contact->GetFixtureA()->GetBody()->GetPosition();
        _ball = contact->GetFixtureB()->GetBody()->GetPosition();
    }
    myFunc1(_ball.x, _ball.y, m_point.x, m_point.y);
    b2Vec2 _b2 = mBallBody->GetLinearVelocity();
    if (_b2.x > 0) {
        myFunc2(_obj.x, _obj.y)?mBallBody->SetAngularVelocity(-velocity):mBallBody->SetAngularVelocity(velocity);
    }
    else
    {
        myFunc2(_obj.x, _obj.y)?mBallBody->SetAngularVelocity(velocity):mBallBody->SetAngularVelocity(-velocity);
    }

}
void HelloWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Layer::draw(renderer, transform, flags);
    Director* director = Director::getInstance();
   // CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    //m_test->Step(&settings);
    mWorld->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
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
