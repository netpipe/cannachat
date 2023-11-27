#ifdef IRRLICHT
#include "irrlichtwidget.h"
#include "irrlicht.h"
#include "irrutil.h"
#include "movemodelanimator.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

//IrrlichtWidget::IrrlichtWidget(QWidget* parent)
//    : QGLWidget(parent)
//    , mModelNode(NULL)
//    , mMoveModelAnimator(NULL)
//    , mDevice(NULL)
//    , mDriver(NULL) {
//  parent2=parent;
//  //  setAttribute(Qt::WA_PaintOnScreen, true);
//  //  setAttribute(Qt::WA_OpaquePaintEvent, true);
//    //setMouseTracking(true);
//    //setFocusPolicy(Qt::ClickFocus);
//   // setFocus(Qt::OtherFocusReason);
//}

IrrlichtWidget::IrrlichtWidget(QWidget* parent)
    : mModelNode(NULL)
    , mMoveModelAnimator(NULL)
    , mDevice(NULL)
    , mDriver(NULL)
{
parent2=parent;
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    setFocus(Qt::OtherFocusReason);
}

IrrlichtWidget::~IrrlichtWidget(){


    if(mMoveModelAnimator){
        mMoveModelAnimator->drop();
    }

    if(mDevice){
        mDevice->drop();
        mDevice = 0;
    }
}

void IrrlichtWidget::init(){
    createIrrlichtDevice();
}

void IrrlichtWidget::initializeGL()
{
}

void IrrlichtWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void IrrlichtWidget::paintGL()
{
    // Clear the buffer with the current clearing color
   // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw stuff
   // glDrawArrays( GL_TRIANGLES, 0, 3 );
}

void IrrlichtWidget::createIrrlichtDevice(){
//    if(mDevice){
//        return;
//    }


//    this->irrRenderTarget = irrRenderTarget;

//    if(softwareRenderer)
//    {
//       this->rendererType = video::EDT_BURNINGSVIDEO;
//    } else
//    {
//        this->rendererType = video::EDT_OPENGL;
//    }



    SIrrlichtCreationParameters params;
    params.DriverType = EDT_OPENGL;
   // params.DriverType =     video::EDT_BURNINGSVIDEO;

   // QWidget *irrRenderTarget

 //  irrRenderTarget = irrRenderTarget;



    params.AntiAlias = 0;
    params.Bits = 32;
    params.DeviceType = EIDT_X11;
    params.Doublebuffer = true;
  //  params.DriverType = EDT_OPENGL;
    params.EventReceiver = 0;
    params.Fullscreen = false;
    params.HighPrecisionFPU = false;
    params.IgnoreInput = false;
    params.LoggingLevel = ELL_INFORMATION;
    params.Stencilbuffer = true;
    params.Stereobuffer = false;
    params.Vsync = false;
  params.WindowId = reinterpret_cast<void*>(parent2->winId());
    params.WindowSize.Width = parent2->size().width();
    params.WindowSize.Height = parent2->size().height();
    params.WithAlphaChannel = false;
    params.ZBufferBits = 16;

 //   device = createDeviceEx(params);

//parent2->Width = parent2->size().width();
//parent2->Height = parent2->size().height();

//parent2->setGeometry(x, y, parent2->size().width(), parent2->size().height());

  //  params.WindowId = (void*)winId();
  //  params.WindowSize =irr::core::dimension2d<irr::u32>(size().width()-10, size().height()-10);
 //   params.Stencilbuffer = true;

    mDevice = createDeviceEx(params);
    if(mDevice == 0){
        qDebug() << "failed to create irrlicht device";
    }
    mDriver = mDevice->getVideoDriver();
    mScene = mDevice->getSceneManager();

    buildIrrlichtScene();
   //  startTimer(0);
}

void IrrlichtWidget::buildIrrlichtScene(){
    scene::ISceneManager* manager = mDevice->getSceneManager();

//    mDevice->getFileSystem()->addFileArchive("./media/map-20kdm2.pk3");
//    manager->addOctreeSceneNode(manager->getMesh("maps/20kdm2.bsp"));
    if (mDevice != 0)
    {
         mDevice->setWindowCaption(L"Hello World!");
        scene::ISceneNode* n = manager->addAnimatedMeshSceneNode(manager->getMesh("./media/sydney.md2"));

        n->setMaterialTexture(0, mDevice->getVideoDriver()->getTexture("./media/sydney.bmp"));
        n->setMaterialFlag( video::EMF_LIGHTING, false );


        mDevice->getFileSystem()->addFileArchive("media/map-20kdm2.pk3");

        scene::IAnimatedMesh* mesh = this->mScene->getMesh("20kdm2.bsp");
        scene::ISceneNode* node = 0;

        if (mesh)
        {
            node = mScene->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
        }

        if (node)
        {
            node->setPosition(core::vector3df(-1300, -144, -1299));
        }
     //    manager->addCameraSceneNode(0, core::vector3df(0,30,-40), vector3df(0,5,0));
     //   manager->addCameraSceneNode();
           manager->addCameraSceneNodeFPS();


        startTimer(0);
    }
}

void IrrlichtWidget::drawIrrlichtScene(){
    mDriver->beginScene(true, true, video::SColor(255, 100, 0, 255));

    mDevice->getSceneManager()->drawAll();
    mDriver->endScene();
}

void IrrlichtWidget::keyPressEvent(QKeyEvent* event){
    irrlichtKeyEvent(event, true);
    emit keyPressed(event);
}

void IrrlichtWidget::keyReleaseEvent(QKeyEvent *event){
    irrlichtKeyEvent(event, false);
}

void IrrlichtWidget::mouseMoveEvent(QMouseEvent* event){
    irrlichtMouseEvent(event, event->button() != Qt::NoButton);
}

void IrrlichtWidget::mousePressEvent(QMouseEvent *event){
    irrlichtMouseEvent(event, true);
}

void IrrlichtWidget::mouseReleaseEvent(QMouseEvent *event){
    irrlichtMouseEvent(event, false);
}

void IrrlichtWidget::irrlichtKeyEvent(QKeyEvent* event, bool pressed){
    irr::SEvent irrEvent;
    irrEvent.EventType = irr::EET_KEY_INPUT_EVENT;

    SIrrlichtKey irrKey = convertToIrrlichtKey(event->key());
    if(irrKey.code == 0){
        return;
    }
    irrEvent.KeyInput.Key = irrKey.code;
    irrEvent.KeyInput.Key = irrKey.code;
    irrEvent.KeyInput.Control = ((event->modifiers() & Qt::ControlModifier) != 0);
    irrEvent.KeyInput.Shift = ((event->modifiers() & Qt::ShiftModifier) != 0);
    irrEvent.KeyInput.Char = irrKey.ch;
    irrEvent.KeyInput.PressedDown = pressed;

    mDevice->postEventFromUser(irrEvent);

    repaint();
}

void IrrlichtWidget::irrlichtMouseEvent(QMouseEvent* event, bool keyPressed) {
    irr::SEvent irrEvent;

    irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

    switch ( event->button() ){
    case Qt::LeftButton:
        irrEvent.MouseInput.Event = keyPressed ? irr::EMIE_LMOUSE_PRESSED_DOWN:irr::EMIE_LMOUSE_LEFT_UP;
        break;

    case Qt::MidButton:
        irrEvent.MouseInput.Event = keyPressed ? irr::EMIE_MMOUSE_PRESSED_DOWN:irr::EMIE_MMOUSE_LEFT_UP;
        break;

    case Qt::RightButton:
        irrEvent.MouseInput.Event = keyPressed ? irr::EMIE_RMOUSE_PRESSED_DOWN:irr::EMIE_RMOUSE_LEFT_UP;
        break;

    default:
        irrEvent.MouseInput.Event = EMIE_MOUSE_MOVED;
        break;
    }

    irrEvent.MouseInput.X = event->x();
    irrEvent.MouseInput.Y = event->y();
    irrEvent.MouseInput.Wheel = 0.0f; // Zero is better than undefined

    mDevice->postEventFromUser( irrEvent );

    repaint();
}

void IrrlichtWidget::animatedMoveModelToPosition(irr::core::vector3df transition, irr::scene::ISceneNode* modelNode){
    if(modelNode != 0){
        return;
    }
    f32 distanceToNewPoint = transition.getDistanceFrom(mModelNode->getPosition());
    u32 timeForAnimation = u32(distanceToNewPoint);
    stopMoveAnimation();
//    mMoveModelAnimator = new MoveModelAnimator(mModelNode->getPosition(), transition, timeForAnimation, mModelNode->getRotation());
//    mModelNode->addAnimator(mMoveModelAnimator);
//    mModelNode->setMD2Animation(EMAT_RUN);
}

void IrrlichtWidget::onCollisionDetected(){
    stopMoveAnimation();
}

void IrrlichtWidget::checkMoveAnimation(){
    if(mMoveModelAnimator && mMoveModelAnimator->hasFinished()){
        stopMoveAnimation();
    }
}

void IrrlichtWidget::stopMoveAnimation(){
    if(mMoveModelAnimator && mModelNode){
        mModelNode->removeAnimator(mMoveModelAnimator);
        mMoveModelAnimator->drop();
        mMoveModelAnimator = 0;
        mModelNode->setMD2Animation(EMAT_STAND);
    }
}

void IrrlichtWidget::timerEvent(QTimerEvent* event)
{
    if(mDevice != 0 && bActive)
    {

        mDevice->getTimer()->tick();

        mDriver->beginScene(true, true, video::SColor(255, 100, 0, 255));
        mDevice->getSceneManager()->drawAll();
        mDriver->endScene();

        mDevice->sleep(15);

  //      video::SColor color (255, 100, 100, 140);

//        device->getVideoDriver()->beginScene(true, true, color);
//        device->getSceneManager()->drawAll();
//        device->getVideoDriver()->endScene();
    }
    else{
        mDevice->sleep(50);
    }
}


void IrrlichtWidget::resizeIrrWidget(int x, int y, int newWidth, int newHeight)
{
    //Resize the render target widget which shows the Irrlicht output.
    parent2->setGeometry(x, y, newWidth, newHeight);

    //Resize Irrlicht render output itself
    if(mDevice != 0)
    {
        core::dimension2d<u32> widgetSize;
        widgetSize.Width = newWidth;
        widgetSize.Height = newHeight;

        mDevice->getVideoDriver()->OnResize(widgetSize);

        scene::ICameraSceneNode *cam = mDevice->getSceneManager()->getActiveCamera();
        if (cam != 0)
        {
            cam->setAspectRatio((f32)widgetSize.Width / (f32)widgetSize.Height);
        }
    }
}

#endif


