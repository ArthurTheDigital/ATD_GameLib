/**
@file     
@brief    A firework animation for ANSI graphics test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#pragma once

#include <ATD/Ansi/Graphics.hpp>

#include <array>
#include <list>
#include <string>


class Firework : public ATD::Ansi::Image::Drawable
{
public:
	class Particle : public ATD::Ansi::Image::Drawable
	{
	public:
		enum class State
		{
			BRIGHT, 
			BURN, 
			BLINK, 
			FINAL
		};

		/* Render constants */

		static const ATD::Ansi::Image HEAD_IMG;
		static const ATD::Vector2S     HEAD_FRAME_SIZE;
		static const size_t            HEAD_FRAME_NUM;

		static const ATD::Ansi::Image BODY_IMG;
		static const ATD::Vector2S     BODY_FRAME_SIZE;
		static const size_t            BODY_FRAME_NUM;

		static const ATD::Ansi::Image TAIL_IMG;
		static const ATD::Vector2S     TAIL_FRAME_SIZE;
		static const size_t            TAIL_FRAME_NUM;

		/* Animation constants */

		static const size_t FRAMES_BRIGHT = 2;

		static const size_t FRAMES_BURN_INIT = 5;
		static const size_t FRAMES_BURN_CONT = 2;

		static const size_t FRAMES_BLINK_INIT = 5;
		static const size_t FRAMES_BLINK_CONT = 4;

		/* Physics constants */

		static const float VELOCITY_INIT;
		static const float VELOCITY_MAX;
		static const float ACCELERATION;


		/**
		 * @brief ...
		 * @param p0 - ...
		 * @param velocity - fraction of VELOCITY_INIT
		 * @param angle - particle angle - fraction of circle */
		Particle(const ATD::Vector2L &p0, 
				float velocity, 
				float angle, 
				unsigned char mainColor, 
				unsigned char burnColor);

		bool finished() const { return (m_state == State::FINAL); }

		void update();

		virtual void drawSelf(ATD::Ansi::Image &target) const override;

	private:
		ATD::Vector2D m_p;
		ATD::Vector2D m_v;

		unsigned char m_mainColor;
		unsigned char m_burnColor;

		State m_state;
		size_t m_remain; /* How long current state lasts */

		ATD::Vector2L m_drawPos;
	};

	enum class State
	{
		MISSILE, 
		EXPLOSION, 
		FINAL
	};

	/* Render constants */

	static const ATD::Ansi::Image MISSILE_IMG;
	static const ATD::Vector2S     MISSILE_FRAME_SIZE;
	static const size_t            MISSILE_FRAME_NUM;

	static const ATD::Ansi::Image EXPLOSION_IMG;
	static const ATD::Vector2S     EXPLOSION_FRAME_SIZE;
	static const size_t            EXPLOSION_FRAME_NUM;


	Firework(long x, 
			size_t h, 
			size_t hScreen, 
			unsigned char mainColor, 
			unsigned char burnColor);

	size_t particlesNum() const { return m_particles.size(); }

	bool finished() const { return (m_state == State::FINAL && !m_particles.size()); }

	void update();

	virtual void drawSelf(ATD::Ansi::Image &target) const override;

private:
	ATD::Vector2L m_pos;

	unsigned char m_mainColor;
	unsigned char m_burnColor;

	std::list<Particle> m_particles;

	State m_state;
	size_t m_frameNo;
	size_t m_remain;
};


