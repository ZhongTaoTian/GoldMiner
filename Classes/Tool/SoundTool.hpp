//
//  SoundTool.hpp
//  MyGame
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

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
