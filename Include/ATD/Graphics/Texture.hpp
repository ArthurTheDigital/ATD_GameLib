/**
 * @file      
 * @brief     Wrap around OpenGl texture.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Graphics/Gl.hpp>
#include <ATD/Graphics/Image.hpp>

#include <memory>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Texture
{
public:
	typedef std::shared_ptr<Texture> Ptr;
	typedef std::shared_ptr<const Texture> CPtr;

	/**
	 * @brief ... */
	enum Data {
		COLOR, 
		DEPTH
	};

	/**
	 * @brief ... */
	enum Type {
		TEX_2D, 
		CUBE_MAP
	};

	/**
	 * @brief ... */
	enum Unit {
		TEX_0 = 0, 
		TEX_1, 
		TEX_2, 
		TEX_3, 
		TEX_4, 
		TEX_5, 
		TEX_6, 
		TEX_7 /* Reserved for canvas texture. */
	};

	/**
	 * @brief ... */
	class Usage
	{
	public:
		/**
		 * @brief ...
		 * @param texture - ...
		 * @param unit    - ... */
		Usage(const Texture &texture, const Unit &unit = TEX_0);

		/**
		 * @param ... */
		~Usage();

	private:
		Gl::Uint m_prevTexture;
		Gl::Enum m_prevUnit;
		/* Gl::Enum m_currUnit; */
		Gl::Enum m_type;
		bool m_activated;
	};

	/**
	 * @brief ...
	 * @param image - ...
	 * @param data  - ...
	 * @param type  - ... */
	Texture(const Image &image, 
			const Data &data = COLOR, 
			const Type &type = TEX_2D);

	/**
	 * @brief ...
	 * @param size  - ...
	 * @param color - ...
	 * @param data  - ...
	 * @param type  - ... */
	Texture(const Vector2S &size, 
			const Pixel &color = Pixel(), 
			const Data &data = COLOR, 
			const Type &type = TEX_2D);

	/* TODO: Copy constructor. */

	/**
	 * @brief ... */
	~Texture();

	/**
	 * @brief ...
	 * @return OpenGL id of texture */
	Gl::Uint GetGlId() const;

	/**
	 * @brief ...
	 * @return ... */
	Type GetType() const;

	/**
	 * @brief ...
	 * @return ... */
	Vector2S Size() const;

	/**
	 * @brief Copy the texture to image.
	 * @return ... */
	Image::Ptr GetImage() const;

private:
	Gl::Uint m_texture;
	Data m_data;
	Type m_type;
	Vector2S m_size;
};

} /* namespace ATD */


