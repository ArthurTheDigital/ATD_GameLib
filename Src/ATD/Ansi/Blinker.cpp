
#include <ATD/Ansi/Blinker.hpp>


ATD::Ansi::Blinker::Blinker(size_t ticks, 
		const Vector2L &position)
	: m_ticks(ticks ? ticks : 1) /* Shall not be zero. */
	, m_position(position)
	, m_ticksCount(0)
{}

ATD::Ansi::Blinker::Blinker(const ATD::Ansi::Blinker &other)
	: m_ticks(other.m_ticks)
	, m_position(other.m_position)
	, m_ticksCount(other.m_ticksCount)
{}

ATD::Ansi::Blinker &ATD::Ansi::Blinker::operator=(const ATD::Ansi::Blinker &other)
{
	m_ticks = other.m_ticks;
	m_position = other.m_position;
	m_ticksCount = other.m_ticksCount;
	return *this;
}

void ATD::Ansi::Blinker::setPosition(const ATD::Vector2L &position)
{ m_position = position; }

void ATD::Ansi::Blinker::reset() const
{ m_ticksCount = 0; }

void ATD::Ansi::Blinker::update(size_t ticksDelta) const
{ m_ticksCount += ticksDelta; }

void ATD::Ansi::Blinker::drawSelf(ATD::Ansi::Image &target) const
{
	if (m_ticksCount / m_ticks % 2 == 1) {
		m_ticksCount %= m_ticks;

		// TODO: Switch Ansi glyph mode
	}
}


