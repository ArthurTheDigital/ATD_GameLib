/**
 * @file      
 * @brief     Image class implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Fs.hpp>
#include <ATD/Core/Rectangle.hpp>
#include <ATD/Core/Vector2.hpp>
#include <ATD/Core/Loadable.hpp>
#include <ATD/Graphics/Pixel.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Image : public Loadable
{
public:
	/**
	 * @brief ...
	 * @class ... */
	class Drawable
	{
	public:
		/**
		 * @brief Default constructor */
		Drawable();

		/**
		 * @brief Destructor */
		virtual ~Drawable();

		/**
		 * @brief ...
		 * @param target - ... */
		virtual void drawSelf(Image &target) const = 0;
	};

	typedef std::shared_ptr<Image> Ptr;
	typedef std::shared_ptr<const Image> CPtr;

	typedef std::function<Pixel(const Pixel &, const Pixel &)> MixerFunc;

	static const std::vector<std::string> EXTENSIONS;
	static const size_t EXT_PNG;
	static const size_t EXT_JPEG;
	static const size_t EXT_JPG;
	static const size_t EXT_GIF;


	/**
	 * @brief ... */
	Image();

	/**
	 * @brief ...
	 * @param size  - ...
	 * @param pixel - ... */
	Image(const Vector2S &size, 
			const Pixel &pixel = Pixel());

	/**
	 * @brief ...
	 * @param other - ... */
	Image(const Image &other);

	/**
	 * @brief ... */
	~Image();

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Image &operator=(const Image &other);

	/**
	 * @brief ...
	 * @param position - ...
	 * @param repeat   - ...
	 * @return ... */
	Pixel getPixel(const Vector2L &position, 
			bool repeat = false) const;

	/**
	 * @brief ...
	 * @return ... */
	inline const Pixel *data() const
	{ return m_pixels; }

	/**
	 * @brief ...
	 * @return ... */
	inline Pixel *data()
	{ return m_pixels; }

	/**
	 * @brief ...
	 * @return ... */
	inline const Vector2S &size() const
	{ return m_size; }

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
	 * @param pixel - ... */
	void clear(const Pixel &pixel = Pixel());

	/**
	 * @brief ...
	 * @param position - ...
	 * @param pixel    - ... */
	void draw(const Vector2L &position, 
			const Pixel &pixel);

	/**
	 * @brief ...
	 * @param position - ...
	 * @param image    - ...
	 * @param mixer    - ... */
	void draw(const Vector2L &position, 
			const Image &image, 
			MixerFunc mixer = nullptr);

	/**
	 * @brief ...
	 * @param position - ...
	 * @param image    - ...
	 * @param bounds   - ...
	 * @param repeat   - whether source image is repeated
	 * @param mixer    - ... */
	void draw(const Vector2L &position, 
			const Image &image, 
			const RectL &bounds, 
			bool repeat = false, 
			MixerFunc mixer = nullptr);

	/**
	 * @brief ...
	 * @param drawable - ... */
	void draw(const Drawable &drawable);

protected:
	/**
	 * @brief ...
	 * @param path - ... */
	virtual void onLoad(const Fs::Path &path) override;

	/**
	 * @brief ...
	 * @param path - ... */
	virtual void onSave(const Fs::Path &path) const override;

	/**
	 * @brief ...
	 * @param file - ...
	 * @return ... */
	bool isPng(FILE *file) const;

	/**
	 * @brief ...
	 * @param file - ... */
	void loadAsPng(FILE *file);

	/**
	 * @brief ...
	 * @param file - ... */
	void saveAsPng(FILE *file) const;

	/**
	 * @brief ...
	 * @param file - ...
	 * @return ... */
	bool isJpeg(FILE *file) const;

	/**
	 * @brief ...
	 * @param file - ... */
	void loadAsJpeg(FILE *file);

	/**
	 * @brief ...
	 * @param file - ... */
	void saveAsJpeg(FILE *file) const;

	/**
	 * @brief ...
	 * @param file - ...
	 * @return ... */
	bool isGif(FILE *file) const;

private:
	/**
	 * @brief ...
	 * @param dst - ...
	 * @param src - ...
	 * @return ... */
	virtual Pixel mixOpacity(const Pixel &dst, 
			const Pixel &src) const;


	Vector2S m_size;
	Pixel *m_pixels;
};

} /* namespace ATD */


