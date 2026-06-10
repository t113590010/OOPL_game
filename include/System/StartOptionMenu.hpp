#ifndef START_OPTION_MENU_HPP
#define START_OPTION_MENU_HPP

#include <functional>
#include <memory>
#include <string>

#include "Button.hpp"
#include "Util/GameObject.hpp"

class StartOptionMenu
{
public:
    StartOptionMenu();

    void Update();
    void Draw();
    void DrawBackgroundOnly();

    void SetOnClose(std::function<void()> callback);
    void SetOnBgmVolumeChanged(std::function<void(int)> callback);
    void SetOnSfxVolumeChanged(std::function<void(int)> callback);
    void SetOnHelp(std::function<void()> callback);
private:
    Util::GameObject m_Background;
    std::string GetBgmVolumeImagePath(int level) const;
    std::string GetSfxVolumeImagePath(int level) const;
    int VolumeLevelToMixerVolume(int level) const;
    std::shared_ptr<Button> m_MenuBg;
    std::shared_ptr<Button> m_CloseBtn;
    std::shared_ptr<Button> m_HelpBtn;
    std::shared_ptr<Button> m_BgmVolumeBtn;
    std::shared_ptr<Button> m_SfxVolumeBtn;

    int m_BgmVolumeLevel = 3;
    int m_SfxVolumeLevel = 3;

    std::function<void()> m_OnClose;
    std::function<void()> m_OnHelp;
    std::function<void(int)> m_OnBgmVolumeChanged;
    std::function<void(int)> m_OnSfxVolumeChanged;
};

#endif