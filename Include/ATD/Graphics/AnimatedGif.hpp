
#pragma once

#include <ATD/Core/Loadable.hpp>
#include <ATD/Core/Rectangle.hpp>
#include <ATD/Graphics/Image.hpp>

#include <memory>
#include <vector>


namespace ATD {

/**
 * @brief ... */
class AnimatedGif : public Loadable
{
public:
	typedef std::shared_ptr<AnimatedGif> Ptr;
	typedef std::shared_ptr<const AnimatedGif> CPtr;

	/**
	 * @brief ... */
	class Player
	{
	public:
		/**
		 * @brief ...
		 * @param gifPtr - const pointer to gif being played */
		Player(AnimatedGif::CPtr gifPtr);

		/**
		 * @brief ...
		 * @return ... */
		AnimatedGif::CPtr GetGif() const;

		/**
		 * @brief ... */
		void Reset();

		/**
		 * @brief ...
		 * @param elapsedMs - time, elapsed since last update, can be 
		 *                    negative to play backwards */
		void Update(long elapsedMs);

		/**
		 * @brief ...
		 * @return ... */
		Image::CPtr GetCurrentFrame() const;

		/**
		 * @brief ...
		 * @return time of play since last gif loop start */
		size_t GetPlayedMs() const;

		/**
		 * @brief Jumps to playedMs time point of the gif loop
		 * @param playedMs - ... */
		void SetPlayedMs(size_t playedMs);

	private:
		AnimatedGif::CPtr m_gifPtr;

		size_t m_curFrameIndex;
		size_t m_curFramePlayedMs;
		size_t m_playedMs;
	};

	static const size_t DFT_DELAY_MS;
	static const size_t MAX_DELAY_MS;

	static const size_t MAX_COLORMAP_SIZE;


	/**
	 * @brief Default constructor. */
	AnimatedGif();

	/**
	 * @brief ...
	 * @param spriteSheet    - source image
	 * @param frameSize      - size of all frames
	 * @param framePositions - positions of all frames (top-left corner) on 
	 *                         the spriteSheet
	 * @param background     - background to be used, transparency ignored
	 * @param delayMs        - delay, used for all frames */
	AnimatedGif(const Image &spriteSheet, 
			const Vector2S &frameSize, 
			const std::vector<Vector2L> &framePositions, 
			Pixel background = Pixel(), 
			size_t delayMs = DFT_DELAY_MS);

	/**
	 * @brief ...
	 * @param frames     - ...
	 * @param alignX     - align of frames on X axis
	 * @param alignY     - align of frames on Y axis
	 * @param background - background to be used, transparency ignored
	 * @param delayMs    - delay, used for all frames */
	AnimatedGif(const std::vector<Image::CPtr> &frames, 
			Align alignX = Align::CENTER, 
			Align alignY = Align::CENTER, 
			Pixel background = Pixel(), 
			size_t delayMs = DFT_DELAY_MS);

	/**
	 * @brief Copy constructor.
	 * @param other - source AnimatedGif to be copied from */
	AnimatedGif(const AnimatedGif &other);

	/**
	 * @brief Destructor. */
	~AnimatedGif();

	/**
	 * @brief ...
	 * @return size of gif image */
	Vector2S Size() const;

	/**
	 * @brief ...
	 * @return ... */
	size_t FramesCount() const;

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @return ... */
	Image::CPtr GetFrame(size_t frameIndex) const;

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @return ... */
	Image::Ptr GetFrame(size_t frameIndex);

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @return ... */
	size_t GetFrameDelayMs(size_t frameIndex) const;

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @param delayMs     - frame delay (milliseconds) */
	void SetFrameDelayMs(size_t frameIndex, size_t delayMs);

	/* FIXME: Do I need methods for editing gif? Like inserting/erasing 
	 * frames? */

	/**
	 * @brief ...
	 * @return ... */
	size_t DurationMs() const;

protected:
	/**
	 * @brief ...
	 * @param filename - ... */
	virtual void OnLoad(const Fs::Path &filename) override;

	/**
	 * @brief ...
	 * @param filename - ... */
	virtual void OnSave(const Fs::Path &filename) const override;

private:
	struct Frame
	{
		Image::Ptr imgPtr; // TODO: replace Pixel *pixels -> Image::Ptr imgPtr
		size_t delayMs;
	};


	Vector2S m_size;
	std::vector<Frame> m_frames;

	mutable size_t m_duration;
};

} /* namespace ATD */


