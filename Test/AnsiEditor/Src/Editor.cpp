/**
@file     
@brief    A simple ANSI editor for console keyboard test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#include "Editor.hpp"

#include <ATD/Core/Fs.hpp>
#include <ATD/Core/Printf.hpp>

#include <errno.h>
#include <stdio.h>
#include <string.h>


// TODO: Redesign to be more clear?

/* Editor constants */

const unsigned Editor::VERSION_MAJ = 0;
const unsigned Editor::VERSION_MIN = 2;

const std::string Editor::DFT_FILENAME = "UNTITLED.txt";



const ATD::Vector2S Editor::CANVAS_SIZE = ATD::Vector2S(40, 20);
const ATD::Vector2L Editor::CANVAS_POSITION = ATD::Vector2L(60, 8);

const unsigned char Editor::UPD_PER_BLINK = 3;

const ATD::Unicode Editor::CANVAS_FRAME_GLYPHS = ATD::Unicode(
		"━┃┏┓┛┗"
		);

const ATD::Unicode Editor::MENU_FRAME_GLYPHS = ATD::Unicode(
		"╍╏┏┓┛┗"
		);

const ATD::Unicode Editor::BRUSH_GLYPHS = ATD::Unicode(
		"⬮@AVWGIH0♦✱❖✖✕✜✚"
		"+⋅ ░▒▓█▲▶▼◀⠶⡂⡪⡯⣿"
		);
	/*   +---+---+---+---  */

const ATD::Vector2S Editor::GLYPH_MENU_SIZE = ATD::Vector2S(16, 2);

const ATD::Vector2S Editor::COLOR_MENU_SIZE = ATD::Vector2S(32, 8);

const std::set<ATD::Ansi::Key> Editor::SAVE_AS_KEYS = {
	ATD::Ansi::Key::ENTER, 
	ATD::Ansi::Key::ERASE, 
	ATD::Ansi::Key::UP, 
	ATD::Ansi::Key::DOWN, 
	ATD::Ansi::Key::LEFT, 
	ATD::Ansi::Key::RIGHT
};

const std::set<ATD::Ansi::Key> Editor::SAVED_KEYS = {
	ATD::Ansi::Key::ENTER
};

const ATD::Vector2S Editor::SAVE_MENU_SIZE = 
	ATD::Vector2S(Editor::CANVAS_SIZE.x, 4);


/* Editor */

Editor::Editor()
	: m_canvas(CANVAS_SIZE)
	, m_brushPos(ATD::Vector2L(2, 2))
	, m_state(State::MAIN)
	, m_brushActive(false)
	, m_brushGlyphNo(0)
	, m_brushFg(177)
	, m_filename("")
	, m_saveErr("")
	, m_blinkPhase(false)
	, m_blinkCounter(0)
{}

void Editor::update(ATD::Ansi::Keyboard &kb)
{
	if (m_brushActive && m_state == State::MAIN) {
		m_canvas.draw(m_brushPos, getBrush());
	}

	auto key = kb.get();
	if (key) {
		switch (m_state) {
			case State::MAIN:
				updOnMain(*key);
				if (m_state == State::SAVE_AS) {
					kb.setFilter(SAVE_AS_KEYS);
				}
				break;

			case State::CHOOSE_GLYPH:
				updOnChooseGlyph(*key);
				break;

			case State::CHOOSE_COLOR:
				updOnChooseColor(*key, m_brushFg);
				break;

			case State::SAVE_AS:
				updOnSaveAsKey(*key);
				if (m_state != State::SAVE_AS) {
					kb.setFilter(SAVED_KEYS);
				} else {
					updOnSaveAsStr(kb.getUnparsed());
				}
				break;

			case State::SAVED:
				updOnSaved(*key);
				kb.setFilter(ATD::Ansi::Keyboard::ALL_KEYS);
				break;

			default:
				{}
				break;
		}
	} else if (m_state == State::SAVE_AS) {
		updOnSaveAsStr(kb.getUnparsed());
	}
	updBlink();
}

void Editor::drawSelf(ATD::Ansi::Image &target) const
{
	/* Draw canvas */
	drawFrame(target, CANVAS_POSITION - ATD::Vector2L(1, 1), CANVAS_SIZE, 
			CANVAS_FRAME_GLYPHS);

	target.draw(CANVAS_POSITION, m_canvas);

	/* Draw brush */
	ATD::Ansi::Glyph brush = getBrush();
	if (m_brushActive) {
		brush.setMode(
				m_blinkPhase ? 
				(brush.mode() | ATD::Ansi::Glyph::M_REVERSED) : 
				(brush.mode() & ~ATD::Ansi::Glyph::M_REVERSED)
				);
	} else {
		brush.setUnicode(ATD::Unicode("⎕").front());
	}
	target.draw(CANVAS_POSITION + m_brushPos, brush);

	/* Draw status */
	target.draw(ATD::Vector2L(2, CANVAS_POSITION.y), 
			ATD::Aux::printf("Brush [%s]", m_brushActive ? " ON" : "OFF"));

	target.draw(ATD::Vector2L(2, CANVAS_POSITION.y + 2), 
			ATD::Aux::printf("Glyph [%s]", 
				ATD::Unicode::strFromGlyph(
					BRUSH_GLYPHS[m_brushGlyphNo]).c_str()));

	target.draw(ATD::Vector2L(2, CANVAS_POSITION.y + 4), 
			ATD::Aux::printf("Color [\033[38;5;%lum█\033[0m] %lu", 
				m_brushFg, m_brushFg));

	ATD::Vector2L hintPosition = 
		CANVAS_POSITION + 
		ATD::Vector2L(0, CANVAS_SIZE.y + 1) + 
		ATD::Vector2L(0, 1);

	switch (m_state) {
		case State::MAIN:
			{
				/* Draw hint */
				target.draw(
						hintPosition, 
						ATD::Aux::printf(
							"b - activate/deactivate brush\n"
							"i - pick brush states from canvas\n"
							"g - choose glyph for brush\n"
							"c - choose color for brush\n"
							"s - save ascii art\n"
							)
						);
			}
			break;

		case State::CHOOSE_GLYPH:
			{
				ATD::Vector2L cgMenuPosition = 
					hintPosition + ATD::Vector2L(0, 1);

				/* Draw 'Choose Glyph' menu */
				drawFrame(target, 
						cgMenuPosition - ATD::Vector2L(1, 1), 
						GLYPH_MENU_SIZE, 
						MENU_FRAME_GLYPHS);

				for (size_t gIter = 0; gIter < BRUSH_GLYPHS.size(); gIter++) {
					ATD::Vector2L delta(
							static_cast<long>(gIter % GLYPH_MENU_SIZE.x), 
							static_cast<long>(gIter / GLYPH_MENU_SIZE.x)
							);

					target.draw(cgMenuPosition + delta, 
							ATD::Ansi::Glyph(BRUSH_GLYPHS[gIter]));
				}
				if (m_blinkPhase) {
					ATD::Vector2L delta(
							static_cast<long>(m_brushGlyphNo % 
								GLYPH_MENU_SIZE.x), 

							static_cast<long>(m_brushGlyphNo / 
								GLYPH_MENU_SIZE.x)
							);
					target.draw(cgMenuPosition + delta, 
							ATD::Ansi::Glyph(
								BRUSH_GLYPHS[m_brushGlyphNo], 
								ATD::Ansi::Glyph::FORECOLOR_DEFAULT, 
								ATD::Ansi::Glyph::BACKCOLOR_DEFAULT, 
								ATD::Ansi::Glyph::M_REVERSED
								)
							);
				}
			}
			break;

		case State::CHOOSE_COLOR:
			{
				ATD::Vector2L ccMenuPosition = hintPosition + ATD::Vector2L(0, 1);

				/* Draw 'Choose Color' menu */
				drawFrame(target, ccMenuPosition - ATD::Vector2L(1, 1), COLOR_MENU_SIZE, 
						MENU_FRAME_GLYPHS);

				for (size_t cIter = 0; cIter < 0x100; cIter++) {
					ATD::Vector2L delta(
							static_cast<long>(cIter % COLOR_MENU_SIZE.x), 
							static_cast<long>(cIter / COLOR_MENU_SIZE.x)
							);

					target.draw(
							ccMenuPosition + delta, 
							ATD::Ansi::Glyph(
								ATD::Unicode(
									(m_brushFg == static_cast<unsigned char>(cIter)) ? 
									"█" : 
									"⣿").front(), 
								static_cast<unsigned char>(cIter)
								)
							);
				}
			}
			break;

		case State::SAVE_AS:
			{
				ATD::Vector2L saveMenuPosition = 
					hintPosition + ATD::Vector2L(0, 1);

				drawFrame(target, 
						saveMenuPosition - ATD::Vector2L(1, 1), 
						SAVE_MENU_SIZE, 
						MENU_FRAME_GLYPHS);

				ATD::Unicode fnGlyphs = 
					ATD::Unicode(m_filename).copy(0, SAVE_MENU_SIZE.x);

				ATD::Vector2S caretPos(fnGlyphs.size(), 0);

				if (caretPos.x >= SAVE_MENU_SIZE.x) {
					caretPos.x = 0;
					caretPos.y++;
				}

				target.draw(saveMenuPosition, 
						ATD::Ansi::Image(fnGlyphs.str()));

				target.draw(saveMenuPosition, 
						ATD::Ansi::Image(m_filename));

				if (m_blinkPhase) {
					ATD::Ansi::Glyph caret = 
						target.getGlyph(saveMenuPosition + caretPos);

					caret.setMode(ATD::Ansi::Glyph::M_REVERSED);
					target.draw(saveMenuPosition + caretPos, caret);
				}
			}
			break;

		case State::SAVED:
			{
				ATD::Vector2L errMenuPosition = 
					hintPosition + ATD::Vector2L(0, 1);

				ATD::Ansi::Image errImage(
						ATD::Unicode(m_saveErr).copy(0, 
							SAVE_MENU_SIZE.x).str());

				drawFrame(target, 
						errMenuPosition - ATD::Vector2L(1, 1), 
						SAVE_MENU_SIZE, 
						MENU_FRAME_GLYPHS);

				target.draw(errMenuPosition, errImage);
			}

		default:
			{}
			break;
	}

}

void Editor::save(const std::string &name)
{
	ATD::Fs::Path path(name);
	if (!path.exists()) {
		FILE *file = ::fopen(path.native().c_str(), "w");
		if (file) {
			std::string output = m_canvas.str();
			::fwrite(output.c_str(), 1, output.size(), file);
			::fclose(file);
			m_saveErr = ATD::Aux::printf("Saved as '%s'", name.c_str());
		} else {
			m_saveErr = ATD::Aux::printf("Did not save:\n'%s'\n%s", 
					name.c_str(), ::strerror(errno));
		}
	} else {
		m_saveErr = ATD::Aux::printf("Did not save:\nFile '%s' already exists", 
				name.c_str());
	}
}

/* === */


void Editor::updOnMain(const ATD::Ansi::Key &key)
{
	ATD::Vector2L n_brushPos = m_brushPos;

	switch (key) {
		case ATD::Ansi::Key::B:
			/* Brush up/down. */
			{
				m_brushActive = !m_brushActive;
			}
			break;

		case ATD::Ansi::Key::I:
			/* Pick color glyph, front color and back color from canvas. */
			{
				if (!m_brushActive) {
					ATD::Ansi::Glyph glyph = m_canvas.getGlyph(m_brushPos);
					m_brushFg = glyph.forecolor();
					for (size_t gNo = 0; gNo < BRUSH_GLYPHS.size(); gNo++) {
						if (glyph.unicode() == BRUSH_GLYPHS[gNo]) {
							m_brushGlyphNo = gNo;
							break;
						}
					}
				}
			}
			break;

		case ATD::Ansi::Key::G:
			/* Open 'choose glyph' menu. */
			{
				m_state = State::CHOOSE_GLYPH;
			}
			break;

		case ATD::Ansi::Key::C:
			/* Open 'choose color' menu. */
			{
				m_state = State::CHOOSE_COLOR;
			}
			break;

		case ATD::Ansi::Key::S:
			/* Open 'savw as' menu. */
			{
				m_state = State::SAVE_AS;
				m_filename = "";
			}
			break;

		case ATD::Ansi::Key::UP:
			/* Move brush ▲. */
			{
				n_brushPos += ATD::Vector2L(0, -1);
			}
			break;

		case ATD::Ansi::Key::DOWN:
			/* Move brush ▼. */
			{
				n_brushPos += ATD::Vector2L(0, 1);
			}
			break;

		case ATD::Ansi::Key::LEFT:
			/* Move brush ◀. */
			{
				n_brushPos += ATD::Vector2L(-1, 0);
			}
			break;

		case ATD::Ansi::Key::RIGHT:
			/* Move brush ▶. */
			{
				n_brushPos += ATD::Vector2L(1, 0);
			}
			break;

		default:
			{}
			break;
	}

	if (ATD::RectL(CANVAS_SIZE).contains(n_brushPos)) {
		m_brushPos = n_brushPos;
		if (m_brushActive) {
			m_canvas.draw(m_brushPos, getBrush());
		}
	}
}

void Editor::updOnChooseGlyph(const ATD::Ansi::Key &key)
{
	/* 'gs' stands for Glyph Selection */
	ATD::Vector2L n_gsPos(
			static_cast<long>(m_brushGlyphNo % GLYPH_MENU_SIZE.x), 
			static_cast<long>(m_brushGlyphNo / GLYPH_MENU_SIZE.x)
			);

	switch(key) {
		case ATD::Ansi::Key::UP:    { n_gsPos += ATD::Vector2L(0, -1); } break;
		case ATD::Ansi::Key::DOWN:  { n_gsPos += ATD::Vector2L(0, 1);  } break;
		case ATD::Ansi::Key::LEFT:  { n_gsPos += ATD::Vector2L(-1, 0); } break;
		case ATD::Ansi::Key::RIGHT: { n_gsPos += ATD::Vector2L(1, 0);  } break;
		case ATD::Ansi::Key::ENTER: { m_state = State::MAIN; } break;
		default: {} break;
	}

	if (ATD::RectL(GLYPH_MENU_SIZE).contains(n_gsPos)) {
		m_brushGlyphNo = 
			static_cast<size_t>(n_gsPos.y * GLYPH_MENU_SIZE.x + n_gsPos.x);
	}
}

void Editor::updOnChooseColor(const ATD::Ansi::Key &key, unsigned char &color)
{
	/* 'cs' stands for Color Selection */
	ATD::Vector2L n_csPos(
			static_cast<long>(color % COLOR_MENU_SIZE.x), 
			static_cast<long>(color / COLOR_MENU_SIZE.x)
			);

	switch(key) {
		case ATD::Ansi::Key::UP:    { n_csPos += ATD::Vector2L(0, -1); } break;
		case ATD::Ansi::Key::DOWN:  { n_csPos += ATD::Vector2L(0, 1);  } break;
		case ATD::Ansi::Key::LEFT:  { n_csPos += ATD::Vector2L(-1, 0); } break;
		case ATD::Ansi::Key::RIGHT: { n_csPos += ATD::Vector2L(1, 0);  } break;
		case ATD::Ansi::Key::ENTER: { m_state = State::MAIN; } break;
		default: {} break;
	}

	if (ATD::RectL(COLOR_MENU_SIZE).contains(n_csPos)) {
		color = static_cast<size_t>(n_csPos.y * COLOR_MENU_SIZE.x + n_csPos.x);
	}
}

void Editor::updOnSaveAsKey(const ATD::Ansi::Key &key)
{
	switch(key) {
		case ATD::Ansi::Key::ENTER:
			{
				save(m_filename);
				m_state = State::SAVED;
			}
			break;

		case ATD::Ansi::Key::ERASE:
			{
				if (m_filename.size()) {
					ATD::Unicode fnGlyphs(m_filename);
					fnGlyphs.erase(fnGlyphs.size() - 1, 1);
					m_filename = fnGlyphs.str();
				}
			}
			break;

		default:
			{}
			break;
	}
}

void Editor::updOnSaveAsStr(const std::string &input)
{
	m_filename += input;
}

void Editor::updOnSaved(const ATD::Ansi::Key &key)
{
	switch(key) {
		case ATD::Ansi::Key::ENTER: { m_state = State::MAIN; } break;
		default: {} break;
	}
}

void Editor::updBlink()
{
	m_blinkCounter++;
	while (m_blinkCounter > UPD_PER_BLINK) {
		m_blinkCounter -= UPD_PER_BLINK;
		m_blinkPhase = !m_blinkPhase;
	}
}

/* FIXME: Use Frame.hpp? */
void Editor::drawFrame(ATD::Ansi::Image &target, 
		const ATD::Vector2L &position, 
		const ATD::Vector2S &size, 
		const ATD::Unicode &frameGlyphs) const
{
	if (frameGlyphs.size() < 6) { return; }

	ATD::Vector2L frameTL = position;
	ATD::Vector2L frameBR = 
		position + static_cast<ATD::Vector2L>(size) + ATD::Vector2L(1, 1);

	target.draw(frameTL, ATD::Ansi::Glyph(frameGlyphs[2]));

	target.draw(ATD::Vector2L(frameTL.x, frameBR.y), 
			ATD::Ansi::Glyph(frameGlyphs[5]));

	target.draw(frameBR, ATD::Ansi::Glyph(frameGlyphs[4]));

	target.draw(ATD::Vector2L(frameBR.x, frameTL.y), 
			ATD::Ansi::Glyph(frameGlyphs[3]));

	for (size_t iH = 1; iH <= size.x; iH++) {
		target.draw(ATD::Vector2L(frameTL.x + iH, frameTL.y), 
				ATD::Ansi::Glyph(frameGlyphs[0]));

		target.draw(ATD::Vector2L(frameTL.x + iH, frameBR.y), 
				ATD::Ansi::Glyph(frameGlyphs[0]));
	}
	for (size_t iV = 1; iV <= size.y; iV++) {
		target.draw(ATD::Vector2L(frameTL.x, frameTL.y + iV), 
				ATD::Ansi::Glyph(frameGlyphs[1]));

		target.draw(ATD::Vector2L(frameBR.x, frameTL.y + iV), 
				ATD::Ansi::Glyph(frameGlyphs[1]));
	}
}


