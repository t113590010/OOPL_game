#include "Util/SFX.hpp"
#include "Util/Logger.hpp"

std::shared_ptr<Mix_Chunk> LoadChunk(const std::string &filepath) {
    auto chunk = std::shared_ptr<Mix_Chunk>(
        Mix_LoadWAV(filepath.c_str()),
        Mix_FreeChunk
    );

    if (chunk == nullptr) {
        LOG_DEBUG("Failed to load SFX: '{}'", filepath);
        LOG_DEBUG("{}", Mix_GetError());
    }

    return chunk;
}

namespace Util {

Util::AssetStore<std::shared_ptr<Mix_Chunk>> SFX::s_Store(LoadChunk);

// 全域音效音量，0 = 靜音，128 = 最大
int SFX::s_GlobalVolume = 128;

SFX::SFX(const std::string &path)
    : m_Chunk(s_Store.Get(path)) {}

int SFX::GetVolume() const {
    if (!m_Chunk) {
        return 0;
    }

    return Mix_VolumeChunk(
        m_Chunk.get(),
        -1
    );
}

void SFX::SetVolume(const int volume) {
    if (!m_Chunk) {
        return;
    }

    int clampedVolume = volume;

    if (clampedVolume < 0) {
        clampedVolume = 0;
    }

    if (clampedVolume > 128) {
        clampedVolume = 128;
    }

    Mix_VolumeChunk(
        m_Chunk.get(),
        clampedVolume
    );
}

void SFX::SetGlobalVolume(int volume) {
    if (volume < 0) {
        volume = 0;
    }

    if (volume > 128) {
        volume = 128;
    }

    s_GlobalVolume = volume;

    LOG_DEBUG(
        "Global SFX volume set to {}",
        s_GlobalVolume
    );
}

int SFX::GetGlobalVolume() {
    return s_GlobalVolume;
}

void SFX::LoadMedia(const std::string &path) {
    m_Chunk = s_Store.Get(path);
}

void SFX::VolumeUp(const int step) {
    int volume = GetVolume();

    SetVolume(
        volume + step
    );
}

void SFX::VolumeDown(const int step) {
    int volume = GetVolume();

    SetVolume(
        volume - step
    );
}

void SFX::Play(const int loop, const int duration) {
    if (!m_Chunk) {
        return;
    }

    Mix_VolumeChunk(
        m_Chunk.get(),
        s_GlobalVolume
    );

    Mix_PlayChannelTimed(
        -1,
        m_Chunk.get(),
        loop,
        duration
    );
}

void SFX::FadeIn(
    const unsigned int tick,
    const int loop,
    const unsigned int duration
) {
    if (!m_Chunk) {
        return;
    }

    Mix_VolumeChunk(
        m_Chunk.get(),
        s_GlobalVolume
    );

    Mix_FadeInChannelTimed(
        -1,
        m_Chunk.get(),
        loop,
        static_cast<int>(tick),
        static_cast<int>(duration)
    );
}

} // namespace Util