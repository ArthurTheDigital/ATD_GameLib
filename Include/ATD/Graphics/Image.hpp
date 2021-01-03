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
 * @brief ... */
class Image : public Loadable
{
public:
	/**
	 * @brief ... */
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
		virtual void DrawSelf(Image &target) const = 0;
	};

	typedef std::shared_ptr<Image> Ptr;
	typedef std::shared_ptr<const Image> CPtr;

	typedef std::function<Pixel(const Pixel &, const Pixel &)> MixerFunc;

	static const std::vector<std::string> EXTENSIONS;
	static const size_t EXT_PNG;
	static const size_t EXT_JPEG;
	static const size_t EXT_JPG;
	static const size_t EXT_GIF;


	Image();

	Image(const Vector2S &size, 
			const Pixel &pixel = Pixel());

	Image(const Image &other);

	~Image();

	Image &operator=(const Image &other);

	Pixel GetPixel(const Vector2L &position, 
			bool repeat = false) const;

	const Pixel *GetPixels() const;

	Pixel *GetPixels();

	Vector2S Size() const;

	bool operator==(const Image &other) const;
	bool operator!=(const Image &other) const;

	void Clear(const Pixel &pixel = Pixel());

	void Draw(const Vector2L &position, 
			const Pixel &pixel);

	void Draw(const Vector2L &position, 
			const Image &image, 
			MixerFunc mixer = nullptr);

	void Draw(const Vector2L &position, 
			const Image &image, 
			const RectL &bounds, 
			bool repeat = false, 
			MixerFunc mixer = nullptr);

	void Draw(const Drawable &drawable);

protected:
	virtual void OnLoad(const Fs::Path &path) override;

	virtual void OnSave(const Fs::Path &path) const override;

	bool IsPng(FILE *file) const;
	void LoadAsPng(FILE *file);
	void SaveAsPng(FILE *file) const;

	bool IsJpeg(FILE *file) const;
	void LoadAsJpeg(FILE *file);
	void SaveAsJpeg(FILE *file) const;

	bool IsGif(FILE *file) const;

private:
	virtual Pixel MixOpacity(const Pixel &dst, 
			const Pixel &src) const;


	Vector2S m_size;
	Pixel *m_pixels;
};

} /* namespace ATD */


