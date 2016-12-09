//
//  SoundTool.hpp
//  MyGame
//
//  Created by sfbest on 16/8/31.
//
//

#ifndef SoundTool_hpp
#define SoundTool_hpp

USING_NS_CC;
using namespace std;

typedef enum {
    Stop,
    Playing,
    Pause
} MusicStatus;

class SoundTool {
public:
    static SoundTool *getInstance();
    SoundTool();
    
    void playBackgroundMusic(char* pszFilePath);
    void playEffect(char* pszFilePath);
    
    bool getMusicIsMute() {
        return _musicMute;
    };
    bool getEffectIsMute() {
        return _effectMute;
    };
    
    void setMusicMute(bool mute);
    void setEffectMute(bool mute);
    void stopEffect(char* pszFilePath);
private:
    bool _musicMute;
    bool _effectMute;
    MusicStatus _musicStatus;
    char* _lastMusic;
    ValueMap _soundIDs;
};

#endif /* SoundTool_hpp */
