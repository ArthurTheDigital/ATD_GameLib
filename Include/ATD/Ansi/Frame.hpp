

#include <ATD/Ansi/Graphics.hpp>
#include <ATD/Core/Rectangle.hpp>
#include <ATD/Core/Unicode.hpp>


namespace ATD {

namespace Ansi {

class Frame : public Image::Drawable
{
public:
	Frame(const std::string &glyphs, 
			unsigned char foreColor = Glyph::FOREGROUND_DEFAULT, 
			unsigned char backColor = Glyph::BACKGROUND_DEFAULT, 
			const RectL &rectangle = RectL());

	Frame(const Frame &other);

	Frame &operator=(const Frame &other);

	void SetForeColor(unsigned char foreColor);
	void SetBackColor(unsigned char backColor);

	/**
	 * @brief Set the rectangle to be surrounded by frame
	 * @param rectangle - the rectangle to be framed */
	void SetRectangle(const RectL &rectangle);

	virtual void DrawSelf(Image &target) const override;

private:
	Unicode m_unicode;
	unsigned char m_foreColor;
	unsigned char m_backColor;
	RectL m_rectangle;
};

extern Frame solidFrame;
extern Frame dashedFrame;
extern Frame doubleFrame;

} /* namespace Ansi */

} /* namespace ATD */
