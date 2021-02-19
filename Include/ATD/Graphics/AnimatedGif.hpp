/**
 * @file      
 * @brief     Animated gif handling.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Loadable.hpp>
#include <ATD/Core/Rectangle.hpp>
#include <ATD/Graphics/Image.hpp>

#include <memory>
#include <vector>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class AnimatedGif : public Loadable
{
public:
	typedef std::shared_ptr<AnimatedGif> Ptr;
	typedef std::shared_ptr<const AnimatedGif> CPtr;

	/**
	 * @brief ...
	 * @class ... */
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
		inline AnimatedGif::CPtr gifPtr() const
		{ return m_gifPtr; }

		/**
		 * @brief ... */
		void reset();

		/**
		 * @brief ...
		 * @param elapsedMs - time, elapsed since last update, can be 
		 *                    negative to play backwards */
		void update(long elapsedMs);

		/**
		 * @brief Get current frame.
		 * @return ... */
		inline Image::CPtr currFrame() const
		{ return m_gifPtr->framePtr(m_curFrameIndex); }

		/**
		 * @brief ...
		 * @return time of play since last gif loop start */
		inline size_t playedMs() const
		{ return m_playedMs; }

		/**
		 * @brief Jumps to playedMs time point of the gif loop
		 * @param playedMs - ... */
		void setPlayedMs(size_t playedMs);

	private:
		AnimatedGif::CPtr m_gifPtr;

		size_t m_curFrameIndex;
		size_t m_curFramePlayedMs;
		size_t m_playedMs;
	};

	/**
	 * @brief ...
	 * @class ... */
	struct Frame
	{
		Image::Ptr imgPtr;
		size_t delayMs;
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
	inline const Vector2S &size() const
	{ return m_size; }

	/**
	 * @brief ...
	 * @return ... */
	inline size_t framesCount() const
	{ return m_frames.size(); }

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @return ... */
	inline Image::CPtr framePtr(size_t frameIndex) const
	{
		checkFrameIndex(frameIndex);
		return static_cast<Image::CPtr>(m_frames[frameIndex].imgPtr);
	}

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @return ... */
	inline Image::Ptr framePtr(size_t frameIndex)
	{ checkFrameIndex(frameIndex); return m_frames[frameIndex].imgPtr; }

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @return ... */
	inline size_t frameDelayMs(size_t frameIndex) const
	{ checkFrameIndex(frameIndex); return m_frames[frameIndex].delayMs; }

	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @param delayMs     - frame delay (milliseconds) */
	void setFrameDelayMs(size_t frameIndex, size_t delayMs);

	/* FIXME: Do I need methods for editing gif? Like inserting/erasing 
	 * frames? */

	/**
	 * @brief ...
	 * @return ... */
	size_t durationMs() const;

protected:
	/**
	 * @brief ...
	 * @param filename - ... */
	virtual void onLoad(const Fs::Path &filename) override;

	/**
	 * @brief ...
	 * @param filename - ... */
	virtual void onSave(const Fs::Path &filename) const override;

private:
	/**
	 * @brief ...
	 * @param frameIndex - ...
	 * @throws ... */
	void checkFrameIndex(size_t frameIndex) const;

private:
	Vector2S m_size;
	std::vector<Frame> m_frames;

	mutable size_t m_duration;
};

} /* namespace ATD */


