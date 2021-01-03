/**
 * @file     
 * @brief    Terminal ansi graphics.
 * @details  License: GPL v3.
 * @author   ArthurTheDigital (arthurthedigital@gmail.com)
 * @since    $Id: $ */

#pragma once

#include <ATD/Core/Rectangle.hpp>
#include <ATD/Core/Unicode.hpp>

#include <array>
#include <memory>
#include <string>
#include <vector>


namespace ATD {

namespace Ansi {

/**
 * @brief ...
 * @class ... */
class Glyph
{
public:
	static const unsigned char FOREGROUND_DEFAULT;
	static const unsigned char BACKGROUND_DEFAULT;


	static const Unicode::Glyph UTF8_GLYPH_DEFAULT;

	static const std::string STR_RESET;

	/* Number of mode flags */
	static const size_t MODE_NUM = 7;

	/* Table, describing mode flags, setters and removers */
	typedef std::array<uint16_t, 3> ModeTableEntry;
	typedef std::array<ModeTableEntry, MODE_NUM> ModeTable;
	static const ModeTable MODE_TABLE;

	/* Mode flags */
	static const uint16_t M_INTENSE;
	static const uint16_t M_DIM;
	static const uint16_t M_ITALIC;
	static const uint16_t M_UNDERLINED;
	static const uint16_t M_REVERSED;
	static const uint16_t M_HIDDEN;
	static const uint16_t M_STRIKED;


	/**
	 * @brief Default Glyph constructor */
	Glyph();

	/**
	 * @brief ...
	 * @param unicode    - unicode character to be drawn
	 * @param foreground - foreground color number
	 * @param background - background color number
	 * @param mode       - font mode */
	Glyph(const Unicode::Glyph &unicode, 
			unsigned char foreground = FOREGROUND_DEFAULT, 
			unsigned char background = BACKGROUND_DEFAULT, 
			uint16_t mode = 0x0000);

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator==(const Glyph &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator!=(const Glyph &other) const;

	/**
	 * @brief Get the string with control sequences
	 * @return ... */
	std::string Str() const;

	/**
	 * @brief String with control sequence, based on previous glyph
	 * @param previ - ...
	 * @return ... */
	std::string StrOptimized(const Glyph *prev = nullptr) const;

	/**
	 * @brief ...
	 * @return ... */
	Unicode::Glyph GetUnicode() const;

	/**
	 * @brief ...
	 * @param unicode - ... */
	void SetUnicode(const Unicode::Glyph &unicode);

	/**
	 * @brief ...
	 * @return ... */
	unsigned char Foreground() const;

	/**
	 * @brief ...
	 * @param foreground - ... */
	void Foreground(unsigned char foreground);

	/**
	 * @brief ...
	 * @return ... */
	unsigned char Background() const;

	/**
	 * @brief ...
	 * @param background - ... */
	void Background(unsigned char background);

	/**
	 * @brief ...
	 * @return ... */
	uint16_t Mode() const;

	/**
	 * @brief ...
	 * @param mode - ... */
	void Mode(uint16_t mode);


private:
	Unicode::Glyph m_unicode;

	unsigned char m_foreground;
	unsigned char m_background;
	uint16_t m_mode;
};


/**
 * @brief ...
 * @class ... */
class GlyphMask
{
public:
	static const uint16_t BIT_UTF8  = 0x0001;
	static const uint16_t BIT_FG    = 0x0002;
	static const uint16_t BIT_BG    = 0x0004;
	static const uint16_t BIT_MODE  = 0x0008;
	static const uint16_t BIT_ANY   = 0x0010;
	static const uint16_t BIT_M_ANY = 0x0020;


	/**
	 * @brief ...
	 * @param glyph - ...
	 * @param bitMask - ... */
	GlyphMask(const Glyph &glyph, 
			uint16_t bitMask = BIT_UTF8 | BIT_FG | BIT_BG | BIT_MODE);

	/**
	 * @brief ... */
	GlyphMask();

	/**
	 * @brief ...
	 * @param glyph - ...
	 * @return ... */
	bool Check(const Glyph &glyph) const;

	/**
	 * @brief ...
	 * @param glyph - ... */
	void Apply(Glyph &glyph) const;

private:
	Glyph m_glyph;
	uint16_t m_bitMask;
};


/**
 * @brief ...
 * @class ... */
class Image
{
public:
	/**
	 * @brief Drawable interface
	 * @class ...
	 *
	 * Allows to extend things, that can be drawn on image. */
	class Drawable
	{
	public:
		/**
		 * @brief ... */
		Drawable();

		/**
		 * @brief ... */
		virtual ~Drawable();

		/**
		 * @brief ...
		 * @param target - ... */
		virtual void DrawSelf(Image &target) const = 0;

	protected:
		Glyph *GetGlyphs(Image &image) const;
		const Glyph *GetGlyphs(const Image &image) const;
	};

	/* Since a image can be big, it is better to access via shared 
	 * pointers */
	typedef std::shared_ptr<const Image> CPtr;
	typedef std::shared_ptr<Image> Ptr;

	typedef std::function<Glyph(const Glyph &, const Glyph &)> MixerFunc;

	/**
	 * @brief ... */
	Image();

	/**
	 * @brief ...
	 * @param size  - ...
	 * @param glyph - ... */
	Image(const Vector2S &size, 
			const Glyph &glyph = Glyph());

	/**
	 * @brief Copy constructor
	 * @param other - ... */
	Image(const Image &other);

	/**
	 * @brief ...
	 * @param chars        - ...
	 * @param initFg       - initial foreground
	 * @param initBg       - initial background
	 * @param initMode     - initial mode
	 * @param transpGlyphs - lists glyps, that are to be transparent */
	Image(const std::string &chars, 
			unsigned char initFg = Glyph::FOREGROUND_DEFAULT, 
			unsigned char initBg = Glyph::BACKGROUND_DEFAULT, 
			uint16_t initMode = 0x0000);

	/**
	 * @brief Destructor */
	~Image();

	/**
	 * @brief ...
	 * @param position - ...
	 * @param repeat   - whether the image shall be repeated if x and y are 
	 *                   out of it's range
	 * @return ...
	 *
	 * If x and y are out of range and repeat == false, 
	 * Glyph() is returned */
	Glyph GetGlyph(const Vector2L &position, 
			bool repeat = false) const;

	/**
	 * @brief Get printable string from the whole image
	 * @param plain - set true for non-colored text
	 * @return ... */
	std::string Str(bool plain = false) const;

	/**
	 * @brief Get printable string from a bounded piece of image
	 * @param bounds - ...
	 * @param repeat - whether the image shall be repeated if bounds are out 
	 *                 of it's range
	 * @param plain  - set true for non-colored text
	 * @return ... */
	std::string Str(const RectL &bounds, 
			bool repeat = false, 
			bool plain = false) const;

	/**
	 * @brief ...
	 * @return ... */
	Vector2S Size() const;

	/**
	 * @brief ...
	 * @return ... */
	const Glyph *Data() const;

	/**
	 * @brief ...
	 * @return ... */
	Glyph *DataModifyable();

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator==(const Image &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator!=(const Image &other) const;

	/**
	 * @brief ...
	 * @param glyph - ... */
	void Clear(const Glyph &glyph = Glyph());

	/**
	 * @brief Sets background to all glyphs */
	void SetBackground(unsigned char background);

	/**
	 * @brief Sets foreground to all glyphs */
	void SetForeground(unsigned char foreground);

	/**
	 * @brief Sets mode to all glyphs */
	void SetMode(uint16_t mode);

	/**
	 * @brief Draw glyph
	 * @param position - ...
	 * @param glyph    - ... */
	void Draw(const Vector2L &position, 
			const Glyph &glyph);

	/**
	 * @brief Draw image
	 * @param position - ...
	 * @param image  - ...
	 * @param mixer    - ... */
	void Draw(const Vector2L &position, 
			const Image &image, 
			MixerFunc mixer = nullptr);

	/**
	 * @brief Draw bounded image
	 * @param position - ...
	 * @param image  - ...
	 * @param bounds   - ...
	 * @param repeat   - ...
	 * @param mixer    - ... */
	void Draw(const Vector2L &position, 
			const Image &image, 
			const RectL &bounds, 
			bool repeat = false, 
			MixerFunc mixer = nullptr);

	/**
	 * @brief Draw the drawable
	 * @param drawable - ... */
	void Draw(const Drawable &drawable);

	/**
	 * @brief Applies 'applyMask' to every glyph, fullfilling 'checkMask'
	 * @param checkMask - ...
	 * @param applyMask - ... */
	void Filter(const GlyphMask &checkMask, 
			const GlyphMask &applyMask);

	/**
	 * @brief Default mixer for drawing
	 * @param dst - destination glyph
	 * @param src - source glyph
	 * @return ... */
	virtual Glyph MixOpacity(const Glyph &dst, 
			const Glyph &src) const;

protected:
	Vector2S m_size;
	Glyph *m_glyphs;
};


/**
 * @brief ...
 * @class ... */
class Screen : public Image
{
public:
	/**
	 * @brief ...
	 * @param alignX - ...
	 * @param alignY - ...
	 * @param offset - ...
	 *
	 * Automatically detects whether the output is a tty, obtains it's width 
	 * and height, creates a image for render of the same size */
	explicit Screen(const Align &alignX = Align::CENTER, 
			const Align &alignY = Align::CENTER, 
			const Vector2L &offset = Vector2L(0, 0));

	/**
	 * @brief ...
	 * @param rtSize - size of image, used for rendering
	 * @param alignX - ...
	 * @param alignY - ...
	 * @param offset - ... */
	explicit Screen(const Vector2S &rtSize, 
			const Align &alignX = Align::CENTER, 
			const Align &alignY = Align::CENTER, 
			const Vector2L &offset = Vector2L(0, 0));

	/**
	 * @brief ... */
	~Screen();

	/**
	 * @brief ...
	 * @return ... */
	RectL View() const;

	/**
	 * @brief ...
	 * @return ... */
	Vector2L Offset() const;

	/**
	 * @brief ...
	 * @param offset - ... */
	void Offset(const Vector2L &offset);

	/**
	 * @brief ...
	 * @return ... */
	Align AlignX() const;

	/**
	 * @brief ...
	 * @param alignX - ... */
	void AlignX(const Align &alignX);

	/**
	 * @brief ...
	 * @return ... */
	Align AlignY() const;

	/**
	 * @brief ...
	 * @param alignY - ... */
	void AlignY(const Align &alignY);

	/**
	 * @brief ...
	 *
	 * Deletes previously drawn symbols, then yet again obtains tty's width 
	 * and height, adjusts view bounds and prints refreshed symbols to tty.
	 *
	 * When called too often and a way too complex pattern is drawn, may cause 
	 * console blinking and random shifting down. */
	virtual void Display() const;

protected:
	virtual Glyph MixOpacity(const Glyph &dst, 
			const Glyph &src) const override;


	/* Hints to adjust view */
	Align m_alignX;
	Align m_alignY;
	Vector2L m_offset;

	/* Number of lines, drawn on Display() previously */
	mutable size_t m_linesDisplayed;
};

} /* namespace Con */

} /* namespace ATD */


