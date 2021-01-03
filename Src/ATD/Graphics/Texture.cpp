/**
 * @file      
 * @brief     Wrap around OpenGl texture.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/Texture.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Graphics/GlCheck.hpp>

#include <map>
#include <vector>


/* ATD::Texture auxiliary: */

struct DataTypes
{
	inline DataTypes(ATD::Gl::Enum n_internal, 
			ATD::Gl::Enum n_format, 
			ATD::Gl::Enum n_type)
		: internal(n_internal)
		, format(n_format)
		, type(n_type)
	{}

	ATD::Gl::Enum internal;
	ATD::Gl::Enum format;
	ATD::Gl::Enum type;
};

const std::map<ATD::Texture::Data, DataTypes> dataTypes = {
	{ ATD::Texture::COLOR, 
		DataTypes(ATD::Gl::RGBA, 
				ATD::Gl::RGBA, 
				ATD::Gl::UNSIGNED_BYTE) }, 
	{ ATD::Texture::DEPTH, 
		DataTypes(ATD::Gl::DEPTH_COMPONENT, 
				ATD::Gl::DEPTH_COMPONENT, 
				ATD::Gl::FLOAT) }
};

const std::map<ATD::Texture::Type, ATD::Gl::Enum> texTypes = {
	{ ATD::Texture::TEX_2D, ATD::Gl::TEXTURE_2D }, 
	{ ATD::Texture::CUBE_MAP, ATD::Gl::TEXTURE_CUBE_MAP }
};

const std::map<ATD::Texture::Unit, ATD::Gl::Enum> texUnits = {
	{ ATD::Texture::TEX_0, ATD::Gl::TEXTURE0 }, 
	{ ATD::Texture::TEX_1, ATD::Gl::TEXTURE1 }, 
	{ ATD::Texture::TEX_2, ATD::Gl::TEXTURE2 }, 
	{ ATD::Texture::TEX_3, ATD::Gl::TEXTURE3 }, 
	{ ATD::Texture::TEX_4, ATD::Gl::TEXTURE4 }, 
	{ ATD::Texture::TEX_5, ATD::Gl::TEXTURE5 }, 
	{ ATD::Texture::TEX_6, ATD::Gl::TEXTURE6 }, 
	{ ATD::Texture::TEX_7, ATD::Gl::TEXTURE7 }
};


/* ATD::Texture::Usage: */

ATD::Texture::Usage::Usage(const ATD::Texture &texture, 
		const ATD::Texture::Unit &unit)
	: m_prevTexture(0)
	, m_prevUnit(ATD::Gl::TEXTURE0)
/*	, m_currUnit(texUnits.at(unit)) */
	, m_type(texTypes.at(texture.GetType()))
	, m_activated(false)
{
	gl._GetIntegerv(Gl::ACTIVE_TEXTURE, 
			reinterpret_cast<Gl::Int *>(&m_prevUnit));

	/* gl._ActiveTexture(m_currUnit); */
	gl._ActiveTexture(texUnits.at(unit));

	gl._GetIntegerv((m_type == Gl::TEXTURE_2D ? 
				Gl::TEXTURE_BINDING_2D : 
				Gl::TEXTURE_BINDING_CUBE_MAP), 
			reinterpret_cast<Gl::Int *>(&m_prevTexture));

	if (m_prevTexture != texture.GetGlId()) {
		gl._BindTexture(m_type, texture.GetGlId());
		m_activated = true;
		//IPRINTF("", "texture %u bound", texture.GetGlId());
	}

	/* gl._ActiveTexture(ATD::Gl::TEXTURE0); */
}

ATD::Texture::Usage::~Usage()
{
	if (m_activated) {
		/* gl._ActiveTexture(m_currUnit); */

		gl._BindTexture(m_type, m_prevTexture);
		//IPRINTF("", "texture unbound");

		/* gl._ActiveTexture(ATD::Gl::TEXTURE0); */
	}
	gl._ActiveTexture(m_prevUnit);
}


/* ATD::Texture: */

ATD::Texture::Texture(const ATD::Image &image, 
		const ATD::Texture::Data &data, 
		const ATD::Texture::Type &type)
	: m_texture(0)
	, m_data(data)
	, m_type(type)
	, m_size(image.Size())
{
	gl._GenTextures(1, &m_texture);

	/* IPRINTF("", "created texture %u", 
			static_cast<unsigned>(m_texture)); // DEBUG */

	Usage use(*this);

	/* Load image into texture */
	gl._TexImage2D(texTypes.at(m_type), /* target */
			0, /* Level of detail */
			dataTypes.at(m_data).internal, /* Internal format */
			static_cast<Gl::Sizei>(m_size.x), 
			static_cast<Gl::Sizei>(m_size.y), 
			0, /* Border? Why border? It's a texture not a window! */
			dataTypes.at(m_data).format, /* Format. Not internal. */
			dataTypes.at(m_data).type, /* Type of data passed */
			image.GetPixels()); /* Ah, finally! */

	/* Set filters */
	gl._TexParameterf(texTypes.at(m_type), Gl::TEXTURE_MIN_FILTER, 
			Gl::NEAREST);
	gl._TexParameterf(texTypes.at(m_type), Gl::TEXTURE_MAG_FILTER, 
			Gl::NEAREST);
}

ATD::Texture::Texture(const ATD::Vector2S &size, 
		const ATD::Pixel &color, 
		const ATD::Texture::Data &data, 
		const ATD::Texture::Type &type)
	: Texture(Image(size, color), data, type)
{}

/* TODO: How to create a decent copy constructor?
 * https://stackoverflow.com/questions/23981016/best-method-to-copy-texture-to-texture
 * The discussion here concludes, that the fastest way to copy a texture is to 
 * draw it to a FrameBuffer and then take texture from it.
 *
 * Will it be fast if I create FrameBuffer every time, when copying occurs?
 *
 * If I have a single cached FrameBuffer, then, I'll need a mutex. 
 * Taking mutex may take a while. Can this method be fast?
 * */

ATD::Texture::~Texture()
{
	gl._DeleteTextures(1, &m_texture);

	/* IPRINTF("", "deleted texture %u", 
			static_cast<unsigned>(m_texture)); // DEBUG */
}

ATD::Gl::Uint ATD::Texture::GetGlId() const
{ return m_texture; }

ATD::Texture::Type ATD::Texture::GetType() const
{ return m_type; }

ATD::Vector2S ATD::Texture::Size() const
{ return m_size; }

ATD::Image::Ptr ATD::Texture::GetImage() const
{
	Image::Ptr imagePtr(new Image(m_size));
	Pixel *pixels = imagePtr->GetPixels();

	/* Create a temporary FrameBuffer for this operation 
	 * (low-level access needed only). */
	Gl::Uint tempFbId = 0;
	gl._GenFramebuffers(1, &tempFbId);
	if (tempFbId) {
		/* Memorize previous FrameBuffer and bind the newly created. */
		Gl::Uint prevFbId;
		gl._GetIntegerv(Gl::FRAMEBUFFER_BINDING, 
				reinterpret_cast<Gl::Int *>(&prevFbId));
		gl._BindFramebuffer(Gl::FRAMEBUFFER, tempFbId);

		/* Attach the texture being read as color attachment #0. */
		gl._FramebufferTexture2D(Gl::FRAMEBUFFER, Gl::COLOR_ATTACHMENT0, 
				Gl::TEXTURE_2D, m_texture, 0);

		/* Read pixels from FrameBuffer. */
		gl._ReadPixels(0, 0, m_size.x, m_size.y, /* Rectangle to read. */
				Gl::RGBA, Gl::UNSIGNED_BYTE, /* Format & type. */
				reinterpret_cast<Gl::Void *>(pixels)); /* Target ptr. */

		/* Restore the previous FrameBuffer and destroy the temporary one. */
		gl._BindFramebuffer(Gl::FRAMEBUFFER, prevFbId);
		gl._DeleteFramebuffers(1, &tempFbId);
	}

	return imagePtr;
}


