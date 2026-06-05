#ifndef UTIL_SFX_HPP
#define UTIL_SFX_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/AssetStore.hpp"

namespace Util {

/**
 * @class SFX
 * @brief Class for handling sound effects.
 * @see One should use Util::BGM for long audio files since it loads audio from
 * disk instead of memory.
 *            (https://wiki.libsdl.org/SDL2_mixer/Mix_LoadMUS#remarks)
 */
class SFX {
public:
    SFX() = delete;

    /**
     * @brief Constructor that initializes the SFX object and loads the sound
     *        effect from the specified file path.
     * @param path The file path of the sound effect to be loaded.
     */
    explicit SFX(const std::string &path);

    /**
     * @brief Deleted copy constructor to prevent copying of SFX objects.
     */
    SFX(const SFX &) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent copying of SFX objects.
     */
    SFX &operator=(const SFX &) = delete;

    /**
     * @brief Retrieves the current volume of this sound effect chunk.
     * @return The current volume of this sound effect.
     */
    int GetVolume() const;

    /**
     * @brief Sets the volume of this specific sound effect chunk.
     * @param volume The desired volume level. Valid range is [0, 128].
     */
    void SetVolume(int volume);

    /**
     * @brief Sets the global SFX volume.
     *        Every SFX::Play() will apply this volume before playing.
     * @param volume The desired global volume level. Valid range is [0, 128].
     */
    static void SetGlobalVolume(int volume);

    /**
     * @brief Gets the global SFX volume.
     * @return Current global SFX volume.
     */
    static int GetGlobalVolume();

    /**
     * @brief Loads the sound effect from the specified file path.
     * @param path The file path of the sound effect to be loaded.
     */
    void LoadMedia(const std::string &path);

    /**
     * @brief Increases the volume of this sound effect by step.
     * @param step The amount to increase the volume by.
     */
    void VolumeUp(int step = 1);

    /**
     * @brief Decreases the volume of this sound effect by step.
     * @param step The amount to decrease the volume by.
     */
    void VolumeDown(int step = 1);

    /**
     * @brief Plays the sound effect.
     * @param loop The number of times the sound effect will loop.
     *             A value of 0 means it will play once.
     * @param duration The duration of the sound effect in milliseconds.
     *                 A value of -1 means it will play the entire sound effect.
     */
    void Play(int loop = 0, int duration = -1);

    /**
     * @brief Fades in the sound effect gradually.
     * @param tick The duration of the fade-in effect, in milliseconds.
     * @param oop The number of times the sound effect will loop after fade-in.
     *            A value of -1 means it will loop indefinitely.
     * @param duration The duration of the sound effect in milliseconds.
     *                 A value of -1 means it will play the entire sound effect.
     */
    void FadeIn(unsigned int tick, int oop = -1, unsigned int duration = -1);

private:
    static Util::AssetStore<std::shared_ptr<Mix_Chunk>> s_Store;

    // 全域音效音量，0 = 靜音，128 = 最大
    static int s_GlobalVolume;

private:
    std::shared_ptr<Mix_Chunk> m_Chunk;
};

} // namespace Util

#endif // UTIL_SFX_HPP