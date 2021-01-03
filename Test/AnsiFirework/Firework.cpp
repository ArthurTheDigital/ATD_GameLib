/**
@file     
@brief    A firework animation for console graphics test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#include "Firework.hpp"

#include <ATD/Core/Unicode.hpp>

#include <math.h>


/* Firework::Particle constants: */

const ATD::Ansi::Image Firework::Particle::HEAD_IMG = ATD::Ansi::Image(
	"G" "@" "0"
	);

const ATD::Vector2S Firework::Particle::HEAD_FRAME_SIZE = ATD::Vector2S(1, 1);

const size_t Firework::Particle::HEAD_FRAME_NUM = 
	Firework::Particle::HEAD_IMG.Size().x / 
	Firework::Particle::HEAD_FRAME_SIZE.x;

const ATD::Ansi::Image Firework::Particle::BODY_IMG = ATD::Ansi::Image(
	"· ·" " · " "+ ·" " + " "· +" " · " "\n"
	" + " "+ ·" " · " "· ·" " · " "· +"
	);

const ATD::Vector2S Firework::Particle::BODY_FRAME_SIZE = ATD::Vector2S(3, 2);

const size_t Firework::Particle::BODY_FRAME_NUM = 
	Firework::Particle::BODY_IMG.Size().x / 
	Firework::Particle::BODY_FRAME_SIZE.x;

const ATD::Ansi::Image Firework::Particle::TAIL_IMG = ATD::Ansi::Image(
	" + · " " · + " "   + " " +   " "\n"
	"· x +" "· +  " "+ x ·" "· + ·" "\n"
	" + · " "   · " " · + " " ·   ");

const ATD::Vector2S Firework::Particle::TAIL_FRAME_SIZE = ATD::Vector2S(5, 3);

const size_t Firework::Particle::TAIL_FRAME_NUM = 
	Firework::Particle::TAIL_IMG.Size().x / 
	Firework::Particle::TAIL_FRAME_SIZE.x;

const float Firework::Particle::VELOCITY_INIT = 2.5;
const float Firework::Particle::VELOCITY_MAX = 8.;
const float Firework::Particle::ACCELERATION = 0.5;


/* Firework::Particle: */

Firework::Particle::Particle(const ATD::Vector2L &p0, 
		float velocity, 
		float angle, 
		unsigned char mainColor, 
		unsigned char burnColor)
	: ATD::Ansi::Image::Drawable()
	, m_p(p0)
	, m_v(ATD::Vector2D(VELOCITY_INIT * velocity, 0).RotatedCw(angle) - ATD::Vector2D(0., 1.))
	, m_mainColor(mainColor)
	, m_burnColor(burnColor)
	, m_state(State::BRIGHT)
	, m_remain(FRAMES_BRIGHT)
	, m_drawPos()
{
	Update();
}

void Firework::Particle::Update()
{
	/* Note:
	 * Not a bug, but feature: the initial point is shifted from the given one 
	 * towards current velocity direction. */

	/* Update point */
	m_p += m_v;

	/* Update velocity */
	m_v += ATD::Vector2D(0, ACCELERATION);
	if (m_v.Length() > VELOCITY_MAX) {
		m_v = m_v.Normalized() * VELOCITY_MAX;
	}

	/* Update state/remain */
	switch (m_state) {
		case State::BRIGHT:
			{
				if (m_remain) {
					m_remain--;
				} else {
					m_state = State::BURN;
					m_remain = FRAMES_BURN_INIT + 
						::rand() % FRAMES_BURN_CONT;
				}
			}
			break;

		case State::BURN:
			{
				if (m_remain) {
					m_remain--;
				} else {
					m_state = State::BLINK;
					m_remain = FRAMES_BLINK_INIT + 
						::rand() % FRAMES_BLINK_CONT;
				}
			}
			break;

		case State::BLINK:
			{
				if (m_remain) {
					m_remain--;
				} else {
					m_state = State::FINAL;
					m_remain = 0;
				}
			}
			break;

		default:
			{}
			break;
	};

	/* Update draw position */
	m_drawPos = ATD::Vector2L(
			static_cast<long>(m_p.x + 0.5 - (m_p.x < 0. ? 1. : 0.)) - 2, 
			static_cast<long>((m_p.y / 2.0) + 0.5 - (m_p.y < 0. ? 1. : 0.)) - 1
			);
}

void Firework::Particle::DrawSelf(ATD::Ansi::Image &target) const
{
	switch (m_state) {
		case State::BRIGHT:
			{
				ATD::Ansi::Image headImage = HEAD_IMG;
				headImage.SetForeground(m_mainColor);
				headImage.SetMode(ATD::Ansi::Glyph::M_REVERSED);

				target.Draw(
						m_drawPos + ATD::Vector2L(2, 1), 
						headImage, 
						ATD::RectL(
							::rand() % HEAD_FRAME_NUM * HEAD_FRAME_SIZE.x, 
							0, 
							HEAD_FRAME_SIZE.x, 
							HEAD_FRAME_SIZE.y
							)
						);
			}
			break;

		case State::BURN:
			{
				ATD::Ansi::Image bodyImage = BODY_IMG;
				bodyImage.SetForeground(m_burnColor);
				ATD::Vector2L bodyPos(
					(m_v.x < -VELOCITY_INIT * 0.3) ? 2 : 
					(m_v.x > VELOCITY_INIT * 0.3) ? 0 : 
					1, 
					(m_v.y < 0) ? 1 : 0
					);

				target.Draw(
						m_drawPos + bodyPos, 
						bodyImage, 
						ATD::RectL(
							::rand() % BODY_FRAME_NUM * BODY_FRAME_SIZE.x, 
							0, 
							BODY_FRAME_SIZE.x, 
							BODY_FRAME_SIZE.y
							)
						);

				ATD::Ansi::Image headImage = HEAD_IMG;
				headImage.SetForeground(m_mainColor);

				target.Draw(
						m_drawPos + ATD::Vector2L(2, 1), 
						headImage, 
						ATD::RectL(
							::rand() % HEAD_FRAME_NUM * HEAD_FRAME_SIZE.x, 
							0, 
							HEAD_FRAME_SIZE.x, 
							HEAD_FRAME_SIZE.y
							)
						);
			}
			break;

		case State::BLINK:
			{
				ATD::Ansi::Image tailImage = TAIL_IMG;
				tailImage.SetForeground(m_mainColor);
				tailImage.SetMode(m_remain > 4 ? 0 : ATD::Ansi::Glyph::M_DIM);

				target.Draw(
						m_drawPos + ATD::Vector2L(1, 0), 
						tailImage, 
						ATD::RectL(
							::rand() % TAIL_FRAME_NUM * TAIL_FRAME_SIZE.x, 
							0, 
							TAIL_FRAME_SIZE.x, 
							TAIL_FRAME_SIZE.y)
						);
			}
			break;

		default:
			{}
			break;
	}
}


/* Firework constants */

const ATD::Ansi::Image Firework::MISSILE_IMG = ATD::Ansi::Image(
	" + " " + " " + " "\n"
	"+G+" "+@+" "+0+" "\n"
	" . " " . " " * " "\n"
	" * " " . " "   " "\n"
	"   " " * " " . "
	);

const ATD::Vector2S Firework::MISSILE_FRAME_SIZE = ATD::Vector2S(3, 5);

const size_t Firework::MISSILE_FRAME_NUM = 
	Firework::MISSILE_IMG.Size().x / 
	Firework::MISSILE_FRAME_SIZE.x;

const ATD::Ansi::Image Firework::EXPLOSION_IMG = ATD::Ansi::Image(
	"         " "         " "   ...   " "\n"
	"   .·.   " "   A7A   " "  +   +  " "\n"
	"   +X+   " "  I+O+I  " " :  X  : " "\n"
	"   ·.·   " "   VHV   " "  +   +  " "\n"
	"         " "         " "   '''   "
	);

const ATD::Vector2S Firework::EXPLOSION_FRAME_SIZE = ATD::Vector2S(9, 5);

const size_t Firework::EXPLOSION_FRAME_NUM = 
	Firework::EXPLOSION_IMG.Size().x / 
	Firework::EXPLOSION_FRAME_SIZE.x;

/**/

Firework::Firework(long x, 
		size_t h, 
		size_t hScreen, 
		unsigned char mainColor, 
		unsigned char burnColor)
	: m_pos(x, hScreen)
	, m_mainColor(mainColor)
	, m_burnColor(burnColor)
	, m_particles()
	, m_state(State::MISSILE)
	, m_frameNo(::rand() % 3)
	, m_remain(h)
{
	Update();
}

void Firework::Update()
{
	/* Update state/remain */
	switch (m_state) {
		case State::MISSILE:
			{
				if (m_remain) {
					m_remain--;
					m_frameNo = (m_frameNo + 1) % MISSILE_FRAME_NUM;
					m_pos -= ATD::Vector2L(0, 1);
				} else {
					m_state = State::EXPLOSION;
					m_remain = 0;
					m_frameNo = 0;
				}
			}
			break;

		case State::EXPLOSION:
			{
				if (m_frameNo < 2) {
					m_frameNo++;
				} else {
					for (int aIter = 0; aIter < 12; aIter++) {
						for (int vIter = 0; vIter < 2; vIter++) {
							if (::rand() % 5 < 4) {
								float angle = 
									(vIter % 2 ? 0. : 1. / 24.) + 
									1. / 12. * static_cast<float>(aIter);

								float velocity = 
									0.7 + 
									0.3 * static_cast<float>(vIter);

								m_particles.push_back(
										Particle(
											ATD::Vector2L(
												m_pos.x, 
												m_pos.y * 2), 
											velocity, 
											angle, 
											m_mainColor, 
											m_burnColor));
							}
						}
					}

					m_state = State::FINAL;
				}
			}

		default:
			{}
			break;
	}

	for (auto pIter = m_particles.begin(); pIter != m_particles.end();) {
		if (pIter->Finished()) {
			pIter = m_particles.erase(pIter);
		} else {
			pIter->Update();
			pIter++;
		}
	}
}

void Firework::DrawSelf(ATD::Ansi::Image &target) const
{
	switch (m_state) {
		case State::MISSILE:
			{
				ATD::Ansi::Image missileImage = MISSILE_IMG;
				missileImage.SetForeground(m_mainColor);

				target.Draw(
						m_pos - ATD::Vector2L(1, 1), 
						missileImage, 
						ATD::RectL(
							(m_frameNo % MISSILE_FRAME_NUM) * 
							MISSILE_FRAME_SIZE.x, 
							0, 
							MISSILE_FRAME_SIZE.x, 
							MISSILE_FRAME_SIZE.y
							)
						);
			}
			break;

		case State::EXPLOSION:
			{
				ATD::Ansi::Image explosionImage = EXPLOSION_IMG;
				explosionImage.SetForeground(
						m_frameNo ? m_mainColor : 
						ATD::Ansi::Glyph::FOREGROUND_DEFAULT);

				target.Draw(
						m_pos - ATD::Vector2L(4, 2), 
						explosionImage, 
						ATD::RectL(
							(m_frameNo % EXPLOSION_FRAME_NUM) * 
							EXPLOSION_FRAME_SIZE.x, 
							0, 
							EXPLOSION_FRAME_SIZE.x, 
							EXPLOSION_FRAME_SIZE.y
							)
						);
			}
			break;

		default:
			{}
			break;
	}

	for (auto &p : m_particles) {
		p.DrawSelf(target);
	}
}


