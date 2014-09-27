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
   //float _radian = CC_RADIANS_TO_DEGREES( atan(-1));
    
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
    
    Vec2 winSize = Director::getInstance()->getWinSize();
    m_mapSize = winSize;
    //this->setScale(0.5);
    
    velocity = 10;
    m_center = b2Vec2( winSize.x/2 * PTM_RATIO , winSize.y /2 * PTM_RATIO);
    m_point = m_center;
    m_isReCalculate = false;
    m_mouseJoint = NULL;
    m_enabelMove = false;
    m_enableCenter = false;
    m_isScaring = false;
    m_joint = NULL;
    m_touchObj = NULL;
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
                bodyDefRe.position.Set((winSize.x/3 * i + 200 ) * PTM_RATIO,(winSize.y/3 * j + 130)* PTM_RATIO); //初始位置
                ReBody = mWorld->CreateBody(&bodyDefRe);
                m_allB2bodys.push_back(ReBody);
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
                        
                        BodyInformation* _info = new BodyInformation;
                        _info->m_shapeType = Box;
                        _info->m_width = 3;
                        _info->m_height = 2;
                        ReBody->SetUserData(_info);
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
                        
                        BodyInformation* _info = new BodyInformation;
                        _info->m_shapeType = Circle;
                        _info->m_radian = 2.0f;
                        ReBody->SetUserData(_info);
                        
                        
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
        bodyDef.position.Set((winSize.x/2 - 150) * PTM_RATIO,winSize.y /2 * PTM_RATIO); //初始位置
        mBallBody = mWorld->CreateBody(&bodyDef);
        
        //申请到之后设置物体属性
        {
            b2CircleShape shape;
            shape.m_radius = 1.0f;
            
//            b2PolygonShape shape;
//            shape.SetAsBox(1,1);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = & shape;
            fixtureDef.density = 0.0f;
            fixtureDef.friction = 10.0f;
            fixtureDef.restitution = 0.0f;
            //fixtureDef.filter.groupIndex = -8;
            
            mBallBody->CreateFixture(&fixtureDef);
            
            BodyInformation* _info = new BodyInformation;
            _info->m_shapeType = Circle;
            _info->m_radian = 1.0f;
            mBallBody->SetUserData(_info);
        }
        
        //让小球有冲力
        //body->ApplyForce(b2Vec2(1000,100),b2Vec2(0,0),true);
        //body->ApplyForceToCenter(b2Vec2(1000,-3000), true);
    }

    
    
    
    
    
    
    //添加触摸事件
    // Adds Touch Event Listener
    auto listener = EventListenerTouchAllAtOnce::create();
    //listener->setSwallowTouches(false);//
    
    listener->onTouchesBegan = std::bind(&HelloWorld::onTouchesBegan,this,std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesMoved = std::bind(&HelloWorld::onTouchesMoved,this,std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesEnded = std::bind(&HelloWorld::onTouchesEnded,this,std::placeholders::_1, std::placeholders::_2);
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -10);//先被触摸
    _touchListener = listener;

    this->scheduleUpdate();
    return true;
}
void HelloWorld::onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent){
    
    if (pTouches.size() == 2) {
        m_gestureId0Point = pTouches[0]->getLocation();
        m_gestureId1Point = pTouches[1]->getLocation();
        auto _offsetp = m_gestureId0Point - m_gestureId1Point;
        gesturesLenth = sqrt(_offsetp.x*_offsetp.x + _offsetp.y*_offsetp.y);
        m_enabelMove = false;
        m_isScaring = true;
        return;

    }
    
    if (pTouches[0]->getID() == 0) {
        m_gestureId0Point = pTouches[0]->getLocation();
        m_oldPoint = pTouches[0]->getLocation();
    }
    if (pTouches[0]->getID() == 1) {
        m_gestureId1Point = pTouches[0]->getLocation();
        auto _offsetp = m_gestureId0Point - m_gestureId1Point;
        gesturesLenth = sqrt(_offsetp.x*_offsetp.x + _offsetp.y*_offsetp.y);
        m_enabelMove = false;
        m_isScaring = true;
        return;
    }
    
    
    //m_enableCenter = true;
    m_isScaring = false;//不正在缩放
    m_enabelMove = true;//允许点击屏幕小球移动
   
    
}
void HelloWorld::onTouchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent){
//    auto touchLocation = touch->getLocation();
//    auto nodePosition = convertToNodeSpace( touchLocation );
//    
//    log("Box2DView::onTouchMoved, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
//    
//    MouseMove(b2Vec2(nodePosition.x*PTM_RATIO,nodePosition.y*PTM_RATIO));
    if (pTouches.size() == 1 && !m_isScaring) {//移动
        auto _offsetPoint = m_oldPoint - pTouches[0]->getLocation();
        auto _offsetLenth = sqrt(_offsetPoint.x* _offsetPoint.x + _offsetPoint.y*_offsetPoint.y);
        if (_offsetLenth <1) {
            return;
        }
        log("%f, %f", _offsetPoint.x, _offsetPoint.y);
        this->setPosition(this->getPosition() - _offsetPoint * (1/this->getScale()));
        this->setAnchorPoint(Vec2(-this->getPositionX()/ m_mapSize.x + 0.5,-this->getPositionY()/ m_mapSize.y + 0.5));
        m_oldPoint = pTouches[0]->getLocation();
        //m_enableCenter = false;
        m_enabelMove = false;//移动成功后不能点击屏幕移动小球
    }
    if (pTouches.size() == 2) {//放大
        
        auto _1p = pTouches[0]->getLocation();
        auto _2p = pTouches[1]->getLocation();
//        if (m_gestureId0Point != _1p && m_gestureId1Point != _2p )
//        {
            auto _offsetp = _1p - _2p;
            float lenth = sqrt(_offsetp.x*_offsetp.x + _offsetp.y*_offsetp.y);
            //log("%f", lenth);
            float _lenthOffset = lenth - gesturesLenth;
            
            
            this->setScale(this->getScale() + _lenthOffset*0.001);
        if (this->getScale()<0.2) {
            this->setScale(0.2);
        }
        if (this->getScale()>3) {
            this->setScale(3);
        }
            
            gesturesLenth = lenth;
//            m_gestureId0Point = _1p;
//            m_gestureId1Point = _2p;
//        }

        
    }
    
}
void HelloWorld::onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent){
//    auto touchLocation = touch->getLocation();
//    auto nodePosition = convertToNodeSpace( touchLocation );
//    
//    log("Box2DView::onTouchEnded, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
//    
//    MouseUp(b2Vec2(nodePosition.x*PTM_RATIO,nodePosition.y*PTM_RATIO));
    
    
    if (m_enabelMove) {
        for (auto& touch : pTouches)
        {
            if(!touch)
                break;
            
            log("touch id: %d",  touch->getID());
            auto touchLocation = touch->getLocation();
            
            auto nodePosition = convertToNodeSpace( touchLocation );//视图层不是当前场景大小, 所以需要转换视图
            log("Box2DView::onTouchBegan, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x * PTM_RATIO, nodePosition.y * PTM_RATIO);
            
            
            MouseDown(b2Vec2(nodePosition.x*PTM_RATIO,nodePosition.y*PTM_RATIO));
            
        }

    }
    
}
b2Vec2 HelloWorld::getTeminalPoint(b2Vec2 p){
    for (int i = 0; i < m_allB2bodys.size(); i ++) {
        b2Body* _body = m_allB2bodys[i];
        BodyInformation* _info = (BodyInformation*)(_body->GetUserData());
        
        if (_info->m_shapeType == Circle) {
            b2Vec2 _objCenter = _body->GetPosition();
            float _lenth = calculateLenthByTwoPoint(p, _objCenter);
            float _maxTouchLenth = ((BodyInformation*)(mBallBody->GetUserData()))->m_radian + _info->m_radian;
            if (_lenth < _maxTouchLenth) {//如果触摸点在圆形区域周边或里面
                float _radian = calculateRadianByTwoPoint(p, _objCenter);
                b2Vec2 _point = calculateTeminalPointByRadianAndLenth(_radian, _maxTouchLenth);
                return -_point+_objCenter;
            }
        }
        else if(_info->m_shapeType == Box)
        {
            b2Vec2 _objCenter = _body->GetPosition();
            float _maxWidth = ((BodyInformation*)(mBallBody->GetUserData()))->m_radian + _info->m_width;
            float _maxHeight = ((BodyInformation*)(mBallBody->GetUserData()))->m_radian + _info->m_height;
            b2Vec2 _lowPoint = _objCenter - b2Vec2(_maxWidth, _maxHeight);
            b2Vec2 _hightPoint = _objCenter + b2Vec2(_maxWidth, _maxHeight);
            if (p.x > _lowPoint.x && p.y > _lowPoint.y && p.x < _hightPoint.x && p.y < _hightPoint.y) {
                float _offsetWidth = p.x - _objCenter.x;
                float _offsetHeight = p.y - _objCenter.y;
                b2Vec2 _point;
                
                if (_offsetWidth>=0 && _offsetHeight>=0) {
                    if ( _offsetWidth > _offsetHeight) {
                        _point.y = p.y;
                        _point.x = _hightPoint.x;
                    }else{
                        _point.x = p.x;
                        _point.y = _hightPoint.y;
                    }
                }
                else if(_offsetWidth>=0 && _offsetHeight<=0){
                    if ( _offsetWidth > -_offsetHeight) {
                        _point.y = p.y;
                        _point.x = _hightPoint.x;
                    }else{
                        _point.x = p.x;
                        _point.y = _lowPoint.y;
                    }
                }
                else if(_offsetWidth<=0 && _offsetHeight<=0){
                    if ( -_offsetWidth > -_offsetHeight) {
                        _point.y = p.y;
                        _point.x = _lowPoint.x;
                    }else{
                        _point.x = p.x;
                        _point.y = _lowPoint.y;
                    }
                }
                else if(_offsetWidth<=0 && _offsetHeight>=0){
                    if ( -_offsetWidth > _offsetHeight) {
                        _point.y = p.y;
                        _point.x = _lowPoint.x;
                    }else{
                        _point.x = p.x;
                        _point.y = _hightPoint.y;
                    }
                }
                return _point;
                
            }
            
        }
    }
    return p;
}

float HelloWorld::calculateRadianByTwoPoint(b2Vec2 originPoint, b2Vec2 teminalPoint)
{
    float _dtx = (teminalPoint.x - originPoint.x);
    float _dty = (teminalPoint.y - originPoint.y);
    float _tan = _dtx/_dty ;
    float _radian ;
    //float velocity = 5;
    //float _x, _y;
    if (_dty >=0) {
        _radian = atan(_tan);
    }
    else  {
        _radian = PI + atan(_tan);
    }

    return _radian;

}
b2Vec2 HelloWorld::calculateTeminalPointByRadianAndLenth(float radian, float lenth)
{
    float _x = lenth * sin(radian);
    float _y = lenth * cos(radian);
    
    return b2Vec2(_x , _y);
}
float HelloWorld::calculateLenthByTwoPoint(b2Vec2 p1, b2Vec2 p2)
{
    float _dtx = (p1.x - p2.x);
    float _dty = (p1.y - p2.y);
    return sqrt(_dtx*_dtx + _dty*_dty);
}
bool HelloWorld::MouseDown(const b2Vec2& p)
{
    
    
    
    m_point = getTeminalPoint(p);
    //mBallBody->SetAngularVelocity(0);
    m_isReCalculate = true;
    
    
//    b2Vec2 _b2 =mBallBody->GetPosition();
//    float _dtx = (m_point.x - _b2.x);
//    float _dty = (m_point.y - _b2.y);
//    m_oldPathLenth = sqrt(_dtx*_dtx + _dty*_dty);
    
    
    m_mouseWorld = p;
    
    if (m_joint != nullptr)
    {
        mWorld->DestroyJoint(m_joint);
    }
    
    // Make a small box.
    b2AABB aabb;
    b2Vec2 d;
    d.Set(0.001, 0.001);
    aabb.lowerBound = p - d;
    aabb.upperBound = p + d;
    
    // Query the world for overlapping shapes.
    QueryCallback callback(p);
    mWorld->QueryAABB(&callback, aabb);
    
    if (callback.m_fixture)
    {
        m_touchObj = callback.m_fixture->GetBody();
    }
    else{
        m_touchObj = NULL;
    }

    
    
    
    return false;
}

void HelloWorld::MouseMove(const b2Vec2& p)
{
    m_mouseWorld = p;
    
    if (m_mouseJoint)
    {
        m_mouseJoint->SetTarget(p);
    }
}

void HelloWorld::MouseUp(const b2Vec2& p)
{
    if (m_mouseJoint)
    {
        mWorld->DestroyJoint(m_mouseJoint);
        m_mouseJoint = nullptr;
    }
    
    //    if (m_bombSpawning)
    //    {
    //        CompleteBombSpawn(p);
    //    }
}

void HelloWorld::moveBall()
{
    b2Vec2 _b2 =mBallBody->GetPosition();
    float _lenth = calculateLenthByTwoPoint(m_point, _b2);
    
    if (m_isReCalculate) {
        float _radian = calculateRadianByTwoPoint(_b2, m_point);
        m_isReCalculate = false;
        mBallBody->SetLinearVelocity(calculateTeminalPointByRadianAndLenth(_radian, velocity));
        
    }
    if (_lenth<0.2)
    {
        mBallBody->SetLinearVelocity(b2Vec2(0,0));
    }
}
void HelloWorld::update(float delta)
{
    int32 velocityIterations = 10;
    int32 positionIterations = 10;
    
    mWorld->Step(delta, velocityIterations, positionIterations);
    //mWorld->ClearForces();
    b2Vec2 _b2 =mBallBody->GetPosition();
    b2Vec2 _offset = _b2 - m_center;
    
    if (m_enableCenter) {
        this->setPosition(Vec2(int (-_offset.x / PTM_RATIO * this->getScale()), int (-_offset.y / PTM_RATIO* this->getScale())));
    }
    
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
//    b2Vec2 _b2 =mBallBody->GetPosition();
//    float _dtx = (m_point.x - _b2.x);
//    float _dty = (m_point.y - _b2.y);
//    float _lenth = sqrt(_dtx*_dtx + _dty*_dty);
//    if (m_oldPathLenth <= _lenth+0.1 && m_oldPathLenth >= _lenth-0.1/*_objBody == m_touchObj*/) {//如果是和触摸的物体接触
//        //mBallBody->SetAngularVelocity(0);
//        mBallBody->SetLinearVelocity(b2Vec2(0,0));
//        m_isReCalculate = false;
//    }
//    else
//    {
//        m_oldPathLenth = _lenth;
//    }
    
}

void HelloWorld::EndContact(b2Contact* contact)
{
   //mBallBody->SetAngularVelocity(0);
}

void HelloWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    b2Vec2 _velocity = mBallBody->GetLinearVelocity();
    float _lenthVelocity = sqrt(_velocity.x*_velocity.x + _velocity.y * _velocity.y);
    //log("%f , %f" ,_velocity.x, _velocity.y);
    
    b2Vec2 _ball;
    b2Vec2 _obj;
    
    b2Body* _objBody;
    if (mBallBody == contact->GetFixtureA()->GetBody()) {
        _ball = contact->GetFixtureA()->GetBody()->GetPosition();
        _obj = contact->GetFixtureB()->GetBody()->GetPosition();
        _objBody = contact->GetFixtureB()->GetBody();
    }
    else
    {
        _obj = contact->GetFixtureA()->GetBody()->GetPosition();
        _ball = contact->GetFixtureB()->GetBody()->GetPosition();
        _objBody = contact->GetFixtureA()->GetBody();
    }
    
    b2Vec2 _b2 =mBallBody->GetPosition();
    float _dtx = (m_point.x - _b2.x);
    float _dty = (m_point.y - _b2.y);
    float _lenth = sqrt(_dtx*_dtx + _dty*_dty);
    
    if (0/*_objBody == m_touchObj*/) {//如果是和触摸的物体接触
        //mBallBody->SetAngularVelocity(0);
        
        mBallBody->SetLinearVelocity(b2Vec2(0,0));
        m_isReCalculate =false;
        //contact->SetEnabled(false);
    }
    else
    {
        myFunc1(_ball.x, _ball.y, m_point.x, m_point.y);
        b2Vec2 _b2 = mBallBody->GetLinearVelocity();
        if (_b2.x > 0) {
            //myFunc2(_obj.x, _obj.y)?mBallBody->SetAngularVelocity(-velocity):mBallBody->SetAngularVelocity(velocity);
            myFunc2(_obj.x, _obj.y)?contact->SetTangentSpeed(velocity):contact->SetTangentSpeed(-velocity);;
        }
        else
        {
            //myFunc2(_obj.x, _obj.y)?mBallBody->SetAngularVelocity(velocity):mBallBody->SetAngularVelocity(-velocity);
            myFunc2(_obj.x, _obj.y)?contact->SetTangentSpeed(-velocity):contact->SetTangentSpeed(velocity);;
        }
        
        m_isReCalculate = true;
        m_oldPathLenth = _lenth;
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
