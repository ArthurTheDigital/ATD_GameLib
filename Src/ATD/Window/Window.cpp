/**
 * @file
 * @brief     Window class implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Window/Window.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Printf.hpp>

#include <ATD/Window/WindowX11.hpp>
#include <ATD/Window/WindowInternal.hpp>

#include <set>

#define IGNORE_UNUSED(x) (void)(x)


/* ATD::Window::Observer */

ATD::Window::Observer::Observer()
	: m_window(nullptr)
{}

ATD::Window::Observer::~Observer()
{ Detach(); }

void ATD::Window::Observer::Attach(ATD::Window *window, 
		uint32_t eventTypeMask)
{
	Detach();
	if (window) {
		std::lock_guard<std::recursive_mutex> lock(
				window->m_observerPtrsLock);

		window->m_observerPtrs[this] = eventTypeMask;

		m_window = window;
	}
}

void ATD::Window::Observer::Detach()
{
	if (m_window) {
		std::lock_guard<std::recursive_mutex> lock(
				m_window->m_observerPtrsLock);

		auto obsIter = m_window->m_observerPtrs.find(this);
		if (obsIter != m_window->m_observerPtrs.end()) {
			m_window->m_observerPtrs.erase(obsIter);
		}

		m_window = nullptr;
	}
}


/* ATD::Window */

ATD::Window::Window(const ATD::Vector2S &size, 
		const ATD::Vector2L &position, 
		const std::string &title, 
		const ATD::Window::PixelSize &pixelSize)
	: m_x11(new WindowX11(size * static_cast<unsigned>(pixelSize), title))
	, m_internal(new WindowInternal(size, static_cast<unsigned>(pixelSize)))
	, m_observerPtrs()
	, m_observerPtrsLock()
	, m_events()
{
	/* To avoid artifacts being shown. */
	Clear();
	Display();

	/* Window is not shown until Expose event is not processed! */
}

ATD::Window::~Window()
{
	delete m_x11;

	/* Detach all observers. */
	std::set<Observer *> observerPtrs;
	{
		std::lock_guard<std::recursive_mutex> lock(m_observerPtrsLock);
		for (auto &obsPair : m_observerPtrs) {
			observerPtrs.insert(obsPair.first);
		}
	}
	for (auto &observerPtr : observerPtrs) { observerPtr->Detach(); }
}

void ATD::Window::Poll(bool keepEvents)
{
	/* Prepare all Observers. */
	{
		std::lock_guard<std::recursive_mutex> lock(
						m_observerPtrsLock);

		for (auto &obsPair : m_observerPtrs) {
			obsPair.first->OnPollStart();
		}
	}

	/* Shall gather new events. */
	std::list<X11::XEvent> xEvts;
	X11::XEvent xEvt;

	/* Get only events, relevant to the current window. */
	while (X11::XCheckIfEvent(m_x11->displayPtr, &xEvt, 
				[](X11::Display *, X11::XEvent *event, 
					X11::XPointer r_window) -> Bool {
					return static_cast<Bool>(
							event->xany.window == 
							reinterpret_cast<X11::Window>(r_window));
				}, 
				reinterpret_cast<X11::XPointer>(m_x11->window)
				)) {
		xEvts.push_back(xEvt);
	}

	{
		std::list<Event> newEvents;
		m_internal->ProcessX11Events(xEvts, newEvents, *m_x11);

		/* Notify all Observers. */
		for (auto &evt : newEvents) {
			std::vector<Observer *> shallBeNotified;

			{
				std::lock_guard<std::recursive_mutex> lock(
						m_observerPtrsLock);

				for (auto &obsPair : m_observerPtrs) {
					if (static_cast<uint32_t>(evt.type) & obsPair.second) {
						shallBeNotified.push_back(obsPair.first);
					}
				}
			}

			for (auto &observerPtr : shallBeNotified) {
				observerPtr->OnNotify(evt);
			}
		}

		/* Put new events into common queue if required. */
		if (keepEvents) { m_events.splice(m_events.end(), newEvents); }
	}
}

bool ATD::Window::PollEvent(ATD::Window::Event &evt)
{
	if (m_events.empty()) { Poll(true); }
	if (!m_events.empty()) {
		evt = m_events.front();
		m_events.pop_front();
		return true;
	} else {
		return false;
	}
}

ATD::Vector2U ATD::Window::Size() const
{ return m_internal->frameBufferPtr->Size(); }

bool ATD::Window::HasFocus() const
{ return m_internal->hasFocus; }

bool ATD::Window::IsClosed() const
{ return m_internal->isClosed; }

void ATD::Window::SetShader2D(ATD::Shader2D::Ptr shader2DPtr)
{ m_internal->frameBufferPtr->SetShader2D(shader2DPtr); }

void ATD::Window::SetShader3D(ATD::Shader3D::Ptr shader3DPtr)
{ m_internal->frameBufferPtr->SetShader3D(shader3DPtr); }

void ATD::Window::SetProjection2D(const ATD::Projection2D &projection2D)
{ m_internal->frameBufferPtr->SetProjection2D(projection2D); }

void ATD::Window::SetProjection3D(const ATD::Projection3D &projection3D)
{ m_internal->frameBufferPtr->SetProjection3D(projection3D); }

void ATD::Window::Clear()
{ m_internal->frameBufferPtr->Clear(); }

void ATD::Window::Draw(const ATD::FrameBuffer::Drawable &drawable)
{ m_internal->frameBufferPtr->Draw(drawable); }

void ATD::Window::Draw(const ATD::VertexBuffer2D &vertices2D, 
		const ATD::Transform2D &transform)
{ m_internal->frameBufferPtr->Draw(vertices2D, transform); }

ATD::Texture::CPtr ATD::Window::GetColorTexture() const
{ return m_internal->frameBufferPtr->GetColorTexture(); }

void ATD::Window::Display()
{
	m_internal->Display(*m_x11);
}


