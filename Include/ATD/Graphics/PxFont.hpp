/**
 * @file      
 * @brief     PxFont (for pixel fonts) class.
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
 * @brief A storage for glyph patterns + texture.
 * @class ... */
class PxFont : public Loadable
{
public:
	/**
	 * @brief ...
	 * @class ... */
	class Glyph
	{
	public:
		/**
		 * @brief ... */
		Glyph();

		/**
		 * @brief ...
		 * @param textureRect - ...
		 * @param joint       - right joint */
		Glyph(const RectL &textureRect, 
				const Vector2L &joint);

		/**
		 * @brief ...
		 * @return ... */
		inline const RectL &textureRect() const
		{ return m_textureRect; }

		/**
		 * @brief ...
		 * @return ... */
		inline const Vector2L &rightJoint() const
		{ return m_joint; }

		/**
		 * @brief ...
		 * @return ... */
		inline Vector2L leftJoint() const
		{ return Vector2L(0, m_joint.y); }

	private:
		RectL m_textureRect;
		Vector2L m_joint;
	};

	typedef std::shared_ptr<PxFont> Ptr;
	typedef std::shared_ptr<const PxFont> CPtr;


	/**
	 * @brief ... */
	PxFont();

	/**
	 * @brief ...
	 * @return ... */
	inline Texture::CPtr texturePtr() const
	{ return static_cast<Texture::CPtr>(m_texturePtr); }

	/**
	 * @brief ...
	 * @param glyph - unicode glyph
	 * @return proper graphical glyph. */
	const Glyph &getGlyph(const Unicode::Glyph &glyph) const;

private:
	/**
	 * @brief ...
	 * @param filename - ... */
	virtual void onLoad(const Fs::Path &filename) override;

	/**
	 * @brief ... */
	virtual void onLoadFinished() override;

	/**
	 * @brief ...
	 * @param filename - ... */
	virtual void onSave(const Fs::Path &filename) const override;

	/**
	 * @brief ...
	 *
	 * Should be called after image has changed. */
	void updateTextureFromImage();


	Glyph m_dftGlyph;
	std::map<Unicode::Glyph, Glyph> m_glyphs;
	Texture::Ptr m_texturePtr;

	Image::Ptr m_imagePtr;
};

/* FIXME: Create some built-in samples, like I've done with shaders. */

} /* namespace ATD */


