//
//  AudioManager.h
//  StepBullet
//
//  Created by luozhipeng on 15/3/27.
//
//

#ifndef __StepBullet__AudioManager__
#define __StepBullet__AudioManager__

class AudioManager{

public:
    static AudioManager* getInstance();
    
    bool getVolumeState();
    void setVolumeState(bool state);
    
public:
    int playAudio(const char* fileName, bool isLoop = false);
    void playBackgroundMusic(const char* fileName);
    
    void stopAudio(int audioId);
    void destory();

public:
    void pauseAll();
    void resumeAll();
    
protected:
    void init();
    
private:
    float m_volume;
    
};
#endif /* defined(__StepBullet__AudioManager__) */
