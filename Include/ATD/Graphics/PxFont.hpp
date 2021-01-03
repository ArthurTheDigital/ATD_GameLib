/**
 * @file      
 * @brief     PxFont (for pixel fonts) class implementation
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Loadable.hpp>
#include <ATD/Core/Unicode.hpp>
#include <ATD/Graphics/Texture.hpp>
#include <ATD/Graphics/Vertex2D.hpp>

#include <map>
#include <memory>
#include <string>


namespace ATD {

/**
 * @brief A storage for glyph patterns + texture. */
class PxFont : public Loadable
{
public:

	class Glyph
	{
	public:
		Glyph();

		Glyph(const RectL &textureRect, 
				const Vector2L &joint);

		RectL GetTextureRect() const;

		Vector2L GetJointRight() const;

		Vector2L GetJointLeft() const;

	private:
		RectL m_textureRect;
		Vector2L m_joint;
	};

	typedef std::shared_ptr<PxFont> Ptr;
	typedef std::shared_ptr<const PxFont> CPtr;


	PxFont();

	Texture::CPtr GetTexturePtr() const;

	const Glyph &GetGlyph(const Unicode::Glyph &glyph) const;

private:
	virtual void OnLoad(const Fs::Path &filename) override;

	virtual void OnLoadFinished() override;

	virtual void OnSave(const Fs::Path &filename) const override;

	/* Should be called after image has changed. */
	void UpdateTextureFromImage();


	Glyph m_dftGlyph;
	std::map<Unicode::Glyph, Glyph> m_glyphs;
	Texture::Ptr m_texturePtr;

	Image::Ptr m_imagePtr;
};

// TODO: Create some built-in samples, like I've done with shaders

} /* namespace ATD */


