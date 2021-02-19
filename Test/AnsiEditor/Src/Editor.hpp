/**
@file     
@brief    A simple ascii editor for console keyboard test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#pragma once

#include <ATD/Ansi/Graphics.hpp>
#include <ATD/Ansi/Keyboard.hpp>

#include <string>


class Editor : public ATD::Ansi::Image::Drawable
{
public:

	static const unsigned VERSION_MAJ;
	static const unsigned VERSION_MIN;

	static const std::string DFT_FILENAME;

	enum class State
	{
		MAIN, 
		CHOOSE_GLYPH, 
		CHOOSE_COLOR, 
		SAVE_AS, 
		SAVED
	};

	/*                                                 
	 * CHOOSE_GLYPH <--->  MAIN  <-----> CHOOSE_COLOR  
	 *                                                 
	 *                    |    A                       
	 *                    |    |                       
	 *                    v    |                       
	 *              SAVE_AS -> SAVED                   */


	static const ATD::Vector2S CANVAS_SIZE;
	static const ATD::Vector2L CANVAS_POSITION;

	static const unsigned char UPD_PER_BLINK;

	static const ATD::Unicode CANVAS_FRAME_GLYPHS;
	static const ATD::Unicode MENU_FRAME_GLYPHS;

	static const ATD::Unicode BRUSH_GLYPHS;
	static const ATD::Vector2S GLYPH_MENU_SIZE;

	static const ATD::Vector2S COLOR_MENU_SIZE;

	static const std::set<ATD::Ansi::Key> SAVE_AS_KEYS;
	static const std::set<ATD::Ansi::Key> SAVED_KEYS;
	static const ATD::Vector2S SAVE_MENU_SIZE;


	Editor();

	void update(ATD::Ansi::Keyboard &kb);

	virtual void drawSelf(ATD::Ansi::Image &target) const override;

	inline ATD::Ansi::Glyph getBrush() const
	{ return ATD::Ansi::Glyph(BRUSH_GLYPHS[m_brushGlyphNo], m_brushFg); }

	void save(const std::string &filename);

private:
	void updOnMain(const ATD::Ansi::Key &key);
	void updOnChooseGlyph(const ATD::Ansi::Key &key);
	void updOnChooseColor(const ATD::Ansi::Key &key, unsigned char &color);

	void updOnSaveAsKey(const ATD::Ansi::Key &key);
	void updOnSaveAsStr(const std::string &input);

	void updOnSaved(const ATD::Ansi::Key &key);

	void updBlink();

	void drawFrame(ATD::Ansi::Image &target, 
			const ATD::Vector2L &position, 
			const ATD::Vector2S &size, 
			const ATD::Unicode &frameGlyphs) const;


	ATD::Ansi::Image m_canvas;
	ATD::Vector2L m_brushPos;
	State m_state;

	bool m_brushActive;
	size_t m_brushGlyphNo;
	unsigned char m_brushFg;

	std::string m_filename;
	std::string m_saveErr;

	bool m_blinkPhase;
	unsigned char m_blinkCounter;
};


