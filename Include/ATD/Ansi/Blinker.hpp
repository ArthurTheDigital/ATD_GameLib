

#include <ATD/Ansi/Graphics.hpp>


namespace ATD {

namespace Ansi {

class Blinker : public Image::Drawable
{
public:
	Blinker(size_t ticks, 
			const Vector2L &position = Vector2L());

	Blinker(const Blinker &other);

	Blinker &operator=(const Blinker &other);

	void SetPosition(const Vector2L &position);

	void Reset() const;

	void Update(size_t ticksDelta = 1) const;

	void DrawSelf(Image &target) const;

private:
	size_t m_ticks;
	Vector2L m_position;

	mutable size_t m_ticksCount;
};

}

}
