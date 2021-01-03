/**
 * @file
 * @brief     Internal window data implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Window/WindowInternal.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Matrix3.hpp>
#include <ATD/Core/Printf.hpp>
#include <ATD/Graphics/Gl.hpp>
#include <ATD/Graphics/GlCheck.hpp>

#include <ATD/Window/Keyboard.hpp>
#include <ATD/Window/Mouse.hpp>


/* ATD::Window::WindowInternal auxiliary: 
 * Control maps for ProcessX11Events: */

static const std::map<unsigned, ATD::Mouse::ButtonId> MOUSE_BUTTON_MAP = {
	{Button1, ATD::Mouse::LEFT}, 
	{Button2, ATD::Mouse::WHEEL}, 
	{Button3, ATD::Mouse::RIGHT}, 
	{8, ATD::Mouse::EXTRA1}, 
	{9, ATD::Mouse::EXTRA2}, 
};

static const std::map<unsigned, ATD::Vector2I> MOUSE_SCROLL_MAP = {
	{Button4, ATD::Vector2I(0, 1)}, 
	{Button5, ATD::Vector2I(0, -1)}, 
	{6, ATD::Vector2I(1, 0)}, 
	{7, ATD::Vector2I(-1, 0)}
};

static const std::map<unsigned, ATD::Key::Code> KEY_MAP = {
	/* English letters: */
	{XK_a, ATD::Key::A}, 
	{XK_b, ATD::Key::B}, 
	{XK_c, ATD::Key::C}, 
	{XK_d, ATD::Key::D}, 
	{XK_e, ATD::Key::E}, 
	{XK_f, ATD::Key::F}, 
	{XK_g, ATD::Key::G}, 
	{XK_h, ATD::Key::H}, 
	{XK_i, ATD::Key::I}, 
	{XK_j, ATD::Key::J}, 
	{XK_k, ATD::Key::K}, 
	{XK_l, ATD::Key::L}, 
	{XK_m, ATD::Key::M}, 
	{XK_n, ATD::Key::N}, 
	{XK_o, ATD::Key::O}, 
	{XK_p, ATD::Key::P}, 
	{XK_q, ATD::Key::Q}, 
	{XK_r, ATD::Key::R}, 
	{XK_s, ATD::Key::S}, 
	{XK_t, ATD::Key::T}, 
	{XK_u, ATD::Key::U}, 
	{XK_v, ATD::Key::V}, 
	{XK_w, ATD::Key::W}, 
	{XK_x, ATD::Key::X}, 
	{XK_y, ATD::Key::Y}, 
	{XK_z, ATD::Key::Z}, 

	/* Numbers: */
	{XK_0, ATD::Key::N_0}, 
	{XK_1, ATD::Key::N_1}, 
	{XK_2, ATD::Key::N_2}, 
	{XK_3, ATD::Key::N_3}, 
	{XK_4, ATD::Key::N_4}, 
	{XK_5, ATD::Key::N_5}, 
	{XK_6, ATD::Key::N_6}, 
	{XK_7, ATD::Key::N_7}, 
	{XK_8, ATD::Key::N_8}, 
	{XK_9, ATD::Key::N_9}, 

	/* Symbols from letter block: */
	{XK_bracketleft, ATD::Key::L_BRACKET}, 
	{XK_bracketright, ATD::Key::R_BRACKET}, 
	{XK_semicolon, ATD::Key::SEMICOLON}, 
	{XK_apostrophe, ATD::Key::QUOTE}, 
	{XK_comma, ATD::Key::COMMA}, 
	{XK_period, ATD::Key::DOT}, 
	{XK_slash, ATD::Key::SLASH}, 
	{XK_backslash, ATD::Key::B_SLASH}, 
	{XK_grave, ATD::Key::TILDE}, 
	{XK_minus, ATD::Key::HYPHEN}, 
	{XK_equal, ATD::Key::EQUAL}, 

	/* Controls: */
	{XK_Shift_L, ATD::Key::L_SHIFT}, 
	{XK_Shift_R, ATD::Key::R_SHIFT}, 
	{XK_Control_L, ATD::Key::L_CTRL}, 
	{XK_Control_R, ATD::Key::R_CTRL}, 
	{XK_Alt_L, ATD::Key::L_ALT}, 
	{XK_Alt_R, ATD::Key::R_ALT}, 
	{XK_Super_L, ATD::Key::L_SUPER}, 
	{XK_Super_R, ATD::Key::R_SUPER}, 
	{XK_Escape, ATD::Key::ESC}, 
	{XK_Tab, ATD::Key::TAB}, 
	{XK_space, ATD::Key::SPACE}, 
	{XK_Return, ATD::Key::ENTER}, 

	/* Arrows: */
	{XK_Left, ATD::Key::LEFT}, 
	{XK_Right, ATD::Key::RIGHT}, 
	{XK_Up, ATD::Key::UP}, 
	{XK_Down, ATD::Key::DOWN}, 

	/* Numpad: */
	{XK_KP_Insert, ATD::Key::NP_0}, 
	{XK_KP_End, ATD::Key::NP_1}, 
	{XK_KP_Down, ATD::Key::NP_2}, 
	{XK_KP_Page_Down, ATD::Key::NP_3}, 
	{XK_KP_Left, ATD::Key::NP_4}, 
	{XK_KP_Begin, ATD::Key::NP_5}, 
	{XK_KP_Right, ATD::Key::NP_6}, 
	{XK_KP_Home, ATD::Key::NP_7}, 
	{XK_KP_Up, ATD::Key::NP_8}, 
	{XK_KP_Page_Up, ATD::Key::NP_9}, 
	{XK_KP_Divide, ATD::Key::NP_SLASH}, 
	{XK_KP_Multiply, ATD::Key::NP_STAR}, 
	{XK_KP_Subtract, ATD::Key::NP_HYPHEN}, 
	{XK_KP_Add, ATD::Key::NP_PLUS}, 
	{XK_KP_Delete, ATD::Key::NP_DOT}, 
	{XK_KP_Enter, ATD::Key::NP_ENTER}, 

	/* Page navigation: */
	{XK_Prior, ATD::Key::PG_UP}, 
	{XK_Next, ATD::Key::PG_DOWN}, 
	{XK_Home, ATD::Key::HOME}, 
	{XK_End, ATD::Key::END}
};


/* ATD::Window::WindowInternal::FbResize: */

class ATD::Window::WindowInternal::FbResize
{
public:
	/* Weird name, because 'Status' is taken by X11 define. */
	enum FbrsStatus {
		NONE = 0, 
		PENDING, 
		COMPLETE
	};


	inline FbResize()
		: m_status(NONE)
		, m_sizeNew()
	{}

	inline void Update(const Vector2S &n_sizeNew)
	{
		if (m_status == NONE) {
			m_status = PENDING;
			m_sizeNew = n_sizeNew;
		}
	}

	inline bool ApplyIfPending(ATD::FrameBuffer::Ptr &frameBufferPtr, 
			const ATD::Align &alignX, 
			const ATD::Align &alignY, 
			ATD::VertexBuffer2D::Ptr &verticesPtr) {
		if (m_status == PENDING) {
			/* Replace FrameBuffer. */
			FrameBuffer::Ptr frameBufferPtrNew(
					new FrameBuffer(
						*frameBufferPtr, 
						m_sizeNew, 
						alignX, 
						alignY));

			frameBufferPtr = frameBufferPtrNew;

			/* Replace vertices. */
			VertexBuffer2D::Ptr verticesPtrNew(
					new VertexBuffer2D(
						RectL(frameBufferPtr->Size()), 
						frameBufferPtr->Size()));

			verticesPtr = verticesPtrNew;

			m_status = COMPLETE;
			return true;
		}
		return false;
	}

	inline bool ReportIfComplete(std::list<Window::Event> &eventsResult)
	{
		if (m_status == COMPLETE) {
			Window::Event event(Window::Event::RESIZE);

			Vector2U *sizePtr = 
				reinterpret_cast<Vector2U *>(&event.data[0]);

			*sizePtr = static_cast<Vector2U>(m_sizeNew);
			eventsResult.push_back(event);

			m_status = NONE;
			return true;
		}
		return false;
	}


private:
	FbrsStatus m_status;
	Vector2S m_sizeNew;
};


/* ATD::Window::WindowInternal: */

ATD::Window::WindowInternal::WindowInternal(const ATD::Vector2S &size, 
		size_t n_pixelSize)
	: hasFocus(true)
	, isClosed(false)
	, pixelSize(n_pixelSize)
	, alignX(Align::LOWER)
	, alignY(Align::LOWER)
	, frameBufferPtr(new FrameBuffer(size))
	, verticesPtr(
			new VertexBuffer2D(
				RectL(frameBufferPtr->Size()), 
				frameBufferPtr->Size()))

	, shader(ATD::Shader2D::PLAIN_VERTEX_SOURCE, 
			ATD::Shader2D::PLAIN_FRAGMENT_SOURCE)

	, transform()
	, fbResizePtr(new FbResize())
{
	transform.SetScale(Vector2D(pixelSize, pixelSize));

	/* Important! */
	shader.SetUniform("unfProject", Projection2D().GetMatrix());
	shader.SetUniform("unfTransform", transform.GetMatrix());

	/* IPRINTF("", "Window shader initialized"); // DEBUG */

	/* Window is not shown until Expose event is not processed! */
}

ATD::Window::WindowInternal::~WindowInternal()
{
	delete fbResizePtr;
}

void ATD::Window::WindowInternal::ProcessX11Events(
		std::list<X11::XEvent> &eventsX11, 
		std::list<ATD::Window::Event> &eventsResult, 
		ATD::Window::WindowX11 &winX11)
{
	/* Erase RELEASE-PRESS key sequences. */
	{
		/* Need a special structure to store iterators. */
		typedef std::vector<std::list<X11::XEvent>::iterator> IterVector;
		std::map<Key::Code, IterVector> keyChangeMap;

		/* Collect iterators. */
		for (auto xEvtIter = eventsX11.begin(); xEvtIter != eventsX11.end(); 
				xEvtIter++) {
			if (xEvtIter->type == KeyPress || xEvtIter->type == KeyRelease) {
				unsigned xKeySym = NoSymbol;
				for (int xksIter = 0; xksIter < 4 && xKeySym == NoSymbol; 
						xksIter++) {
					xKeySym = X11::XLookupKeysym(&xEvtIter->xkey, xksIter);
				}
				if (xKeySym != NoSymbol) {
					auto kmIter = KEY_MAP.find(xKeySym);
					if (kmIter != KEY_MAP.end()) {
						keyChangeMap[kmIter->second].push_back(xEvtIter);
					}
				}
			}
		}

		/* Erase unwanted events, using iterators collected. */
		for (auto &kcPair : keyChangeMap) {
			for (size_t ivIter = 0; ivIter + 1 < kcPair.second.size();) {
				if ((kcPair.second[ivIter]->type == KeyRelease) && 
						(kcPair.second[ivIter + 1]->type == KeyPress)) {
					eventsX11.erase(kcPair.second[ivIter]);
					eventsX11.erase(kcPair.second[ivIter + 1]);

					ivIter += 2;
				} else {
					ivIter += 1;
				}
			}
		}
	}

	/* Honestly process other events. */
	while (!eventsX11.empty()) {
		X11::XEvent xEvt = eventsX11.front();
		eventsX11.pop_front();
		switch (xEvt.type) {
			case ConfigureNotify: /* RESIZE */
				{
					/* Size or position has changed.
					 * FIXME: what can change else? */

					Vector2S sizeNew(xEvt.xconfigure.width, 
							xEvt.xconfigure.height);

					Vector2L positionNew(xEvt.xconfigure.x, 
							xEvt.xconfigure.y);

					if (sizeNew != winX11.size) {
						/* Actual resize. */
						Vector2S fbSizeNew = sizeNew / pixelSize;
						if (fbSizeNew != frameBufferPtr->Size()) {
							/* Resize event hit pixelized frameBufferPtr. 
							 * Game should be notified. */

							fbResizePtr->Update(fbSizeNew);
						}

						winX11.size = sizeNew;
						/* TODO: Redesign, according to alignX and alignY
						 *
						 * transform.SetOffset(
								ATD::Vector2D(
									winX11.size.x % pixelSize / 2, 
									winX11.size.y % pixelSize / 2)); */
					}

					if (positionNew != winX11.position) {
						/* Changing position is nothing special. */
						winX11.position = positionNew;
					}
				}
				break;

			case ClientMessage:
				{
					/* TODO: Use for handling CLOSE event. */
				}
				break;

			case FocusIn: /* FOCUS_IN */
				{
					ATD::Window::Event event(ATD::Window::Event::FOCUS_IN);
					eventsResult.push_back(event);

					hasFocus = true;
				}
				break;

			case FocusOut: /* FOCUS_OUT */
				{
					ATD::Window::Event event(ATD::Window::Event::FOCUS_OUT);
					eventsResult.push_back(event);

					hasFocus = false;
				}
				break;

			case EnterNotify: /* MOUSE_IN */
				{
					ATD::Mouse::Event event(ATD::Window::Event::MOUSE_IN);
					eventsResult.push_back(
							static_cast<ATD::Window::Event>(event));
				}
				break;

			case LeaveNotify: /* MOUSE_OUT */
				{
					ATD::Mouse::Event event(ATD::Window::Event::MOUSE_OUT);
					eventsResult.push_back(
							static_cast<ATD::Window::Event>(event));
				}
				break;

			case MotionNotify: /* MOUSE_MOVE */
				{
					ATD::Vector2I mousePosition = static_cast<Vector2I>(
							transform.ApplyReverse(
								Vector2D(
									xEvt.xbutton.x, 
									xEvt.xbutton.y)));

					ATD::Mouse::Event event(
							ATD::Window::Event::MOUSE_MOVE, 
							mousePosition);

					eventsResult.push_back(
							static_cast<ATD::Window::Event>(event));
				}
				break;

			case ButtonPress: /* MOUSE_PRESS */
				{
					unsigned button = xEvt.xbutton.button;
					ATD::Vector2I mousePosition = static_cast<Vector2I>(
							transform.ApplyReverse(
								Vector2D(
									xEvt.xbutton.x, 
									xEvt.xbutton.y)));

					auto bmIter = MOUSE_BUTTON_MAP.find(button);
					if (bmIter != MOUSE_BUTTON_MAP.end()) {
						ATD::Mouse::Event event(
								ATD::Window::Event::MOUSE_PRESS, 
								mousePosition);
						event.Button() = bmIter->second;

						eventsResult.push_back(
								static_cast<ATD::Window::Event>(event));
					}
				}
				break;

			case ButtonRelease: /* MOUSE_RELEASE or MOUSE_SCROLL */
				{
					unsigned button = xEvt.xbutton.button;
					ATD::Vector2I mousePosition = static_cast<Vector2I>(
							transform.ApplyReverse(
								Vector2D(
									xEvt.xbutton.x, 
									xEvt.xbutton.y)));

					auto bmIter = MOUSE_BUTTON_MAP.find(button);
					if (bmIter != MOUSE_BUTTON_MAP.end()) {
						ATD::Mouse::Event event(
								ATD::Window::Event::MOUSE_RELEASE, 
								mousePosition);
						event.Button() = bmIter->second;

						eventsResult.push_back(
								static_cast<ATD::Window::Event>(event));
					}

					auto smIter = MOUSE_SCROLL_MAP.find(button);
					if (smIter != MOUSE_SCROLL_MAP.end()) {
						ATD::Mouse::Event event(
								ATD::Window::Event::MOUSE_SCROLL, 
								ATD::Vector2I(xEvt.xbutton.x, 
									xEvt.xbutton.y));
						event.Scroll() = smIter->second;

						eventsResult.push_back(
								static_cast<ATD::Window::Event>(event));
					}
				}
				break;

			case KeyPress: /* KEY_PRESS */
				{
					unsigned xKeySym = NoSymbol;
					for (int xksIter = 0; xksIter < 4 && xKeySym == NoSymbol; 
							xksIter++) {
						xKeySym = X11::XLookupKeysym(&xEvt.xkey, xksIter);
					}
					if (xKeySym != NoSymbol) {
						auto kmIter = KEY_MAP.find(xKeySym);
						if (kmIter != KEY_MAP.end()) {
							ATD::Keyboard::Event event(
									ATD::Window::Event::KEY_PRESS, 
									kmIter->second);

							eventsResult.push_back(
									static_cast<ATD::Window::Event>(event));
						} else {
							WPRINTF("", "unknown key symbol %u", xKeySym);
						}
					}
				}
				break;

			case KeyRelease: /* KEY_RELEASE */
				{
					unsigned xKeySym = NoSymbol;
					for (int xksIter = 0; xksIter < 4 && xKeySym == NoSymbol; 
							xksIter++) {
						xKeySym = X11::XLookupKeysym(&xEvt.xkey, xksIter);
					}
					if (xKeySym != NoSymbol) {
						auto kmIter = KEY_MAP.find(xKeySym);
						if (kmIter != KEY_MAP.end()) {
							ATD::Keyboard::Event event(
									ATD::Window::Event::KEY_RELEASE, 
									kmIter->second);

							eventsResult.push_back(
									static_cast<ATD::Window::Event>(event));
						} else {
							WPRINTF("", "unknown key symbol %u", xKeySym);
						}
					}
				}
				break;

			default:
				{}
				break;
		}
	}

	/* Apply & report resize events. */
	fbResizePtr->ApplyIfPending(frameBufferPtr, alignX, alignY, verticesPtr);
	fbResizePtr->ReportIfComplete(eventsResult);
}

ATD::Matrix3F ATD::Window::WindowInternal::Coords2DTransformMatrix(
		const ATD::Window::WindowX11 &winX11) const
{
	Transform2D transformCoords;

	transformCoords.SetScale(
			ATD::Vector2D(
				2.f / static_cast<float>(winX11.size.x), 
				2.f / static_cast<float>(winX11.size.y)));

	transformCoords.SetOffset(ATD::Vector2D(-1., -1.));

	Matrix3F matrix = transformCoords.GetMatrix();
	matrix[1][0] *= -1.f;
	matrix[1][1] *= -1.f;
	matrix[1][2] *= -1.f;

	return matrix;
}

void ATD::Window::WindowInternal::Display(ATD::Window::WindowX11 &winX11)
{
	gl._Clear(Gl::COLOR_BUFFER_BIT | Gl::DEPTH_BUFFER_BIT);

	{
		/* IPRINTF("", "FB size: %lu %lu", 
				frameBufferPtr->Size().x, 
				frameBufferPtr->Size().y); // DEBUG */

		shader.SetUniform("unfTransform", 
				Coords2DTransformMatrix(winX11) * 
				transform.GetMatrix());

		Shader::Usage useShader(shader);
		Texture::Usage useTexture(*frameBufferPtr->GetColorTexture());

		gl._Viewport(0, 0, winX11.size.x, winX11.size.y);

		verticesPtr->DrawSelfInternal(shader.GetAttrIndices());
	}

	X11::glXSwapBuffers(winX11.displayPtr, winX11.window);
}


