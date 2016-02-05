//
//  AudioManager.cpp
//  StepBullet
//
//  Created by luozhipeng on 15/3/27.
//
//

#include "AudioManager.h"
#include "audio/include/AudioEngine.h"
#include "base/CCUserDefault.h"

USING_NS_CC;

#define USERDEFAULT_VOLUME_KEY "volume"

int s_backgroundMusicId = -1;

AudioManager* s_audioManager = nullptr;

AudioManager* AudioManager::getInstance(){
    if(!s_audioManager){
        s_audioManager = new AudioManager();
        s_audioManager->init();
    }
    
    return s_audioManager;
}

bool AudioManager::getVolumeState(){
    return m_volume == 1.0;
}

void AudioManager::setVolumeState(bool state){
    m_volume = state ? 1.0 : 0.0;
    
    if(s_backgroundMusicId != -1){
        cocos2d::experimental::AudioEngine::setVolume(s_backgroundMusicId, m_volume);
    }
    
    UserDefault::getInstance()->setBoolForKey(USERDEFAULT_VOLUME_KEY, state);
}

int AudioManager::playAudio(const char* fileName, bool isLoop/* = false*/){
    return cocos2d::experimental::AudioEngine::play2d(fileName, isLoop, m_volume);
}

void AudioManager::playBackgroundMusic(const char* fileName){
    if(s_backgroundMusicId != -1){
        cocos2d::experimental::AudioEngine::stop(s_backgroundMusicId);
        s_backgroundMusicId = -1;
    }
    
    s_backgroundMusicId = this->playAudio(fileName, true);
}

void AudioManager::stopAudio(int audioId){
    cocos2d::experimental::AudioEngine::stop(audioId);
}

void AudioManager::init(){
    cocos2d::experimental::AudioEngine::lazyInit();
    
    bool isVolume = UserDefault::getInstance()->getBoolForKey(USERDEFAULT_VOLUME_KEY, true);
    this->setVolumeState(isVolume);
}

void AudioManager::destory(){
    cocos2d::experimental::AudioEngine::end();
}

void AudioManager::pauseAll(){
    cocos2d::experimental::AudioEngine::pauseAll();
}

void AudioManager::resumeAll(){
    cocos2d::experimental::AudioEngine::resumeAll();
}
