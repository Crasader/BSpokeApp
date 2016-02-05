#include "CocosAppDelegate.h"
#include "platform/ios/CCEAGlView-ios.h"
#include "util/AudioManager.h"

USING_NS_CC;

static bool s_isFinishLaunching = false;

CocosAppDelegate::CocosAppDelegate() {

}

CocosAppDelegate::~CocosAppDelegate()
{
    
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void CocosAppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

void* CocosAppDelegate::getEAGlView(){
    
    GLView* view = cocos2d::Director::getInstance()->getOpenGLView();
    
    return view? view->getEAGLView() : NULL;
}

void* CocosAppDelegate::createEAGlView(int x, int y, int width, int height){
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Init the CCEAGLView
    CGRect bounds = CGRectMake(x, y, width, height);
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: bounds
                                         pixelFormat: (__bridge NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    [eaglView setMultipleTouchEnabled:YES];
    [eaglView retain];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    glview->setDesignResolutionSize(640, 800, ResolutionPolicy::FIXED_WIDTH);
    
//    if(!s_isFinishLaunching)
    app->run();
    
    return eaglView;
}

bool CocosAppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
    AudioManager::getInstance();

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // run
    //为了避免因为scene数为0而释放掉
    Scene* scene = Scene::create();
    director->pushScene(scene);
    
    director->pause();
    director->stopAnimation();
    
    s_isFinishLaunching = true;

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void CocosAppDelegate::applicationDidEnterBackground() {
    if(s_isFinishLaunching)
        Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    AudioManager::getInstance()->pauseAll();
}

// this function will be called when the app is active again
void CocosAppDelegate::applicationWillEnterForeground() {
    if(s_isFinishLaunching)
        Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    AudioManager::getInstance()->resumeAll();
}
