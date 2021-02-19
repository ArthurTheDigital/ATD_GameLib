/**
 * @file      
 * @brief     Menu base class implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Window/Menu.hpp>


/* ATD::Menu::Item: */

ATD::Menu::Item::Item(const ATD::Vector2S &size, 
		const ATD::Menu::Item::Drawables &drawables, 
		uint32_t cFlags)
	: FrameBuffer::Drawable()
	, m_drawables(drawables)
	, m_cFlags(cFlags)
	, m_bounds(size)
	, m_isEnabled(true)
{}

void ATD::Menu::Item::setPosition(const ATD::Vector2L &position)
{
	const Vector2L positionOld = Vector2L(m_bounds.x, m_bounds.y);
	const Vector2D positionDelta = 
		static_cast<Vector2D>(position - positionOld);

	for (size_t drawIndex = 0; drawIndex < m_drawables.size(); drawIndex++) {
		FrameBuffer::Drawable2D *drawablePtr = m_drawables[drawIndex];
		const Vector2D offsetOld = drawablePtr->transform().offset();

		drawablePtr->setOffset(offsetOld + positionDelta);
	}

	m_bounds.x = position.x;
	m_bounds.y = position.y;

	onShift(position - positionOld);
}

void ATD::Menu::Item::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
}

void ATD::Menu::Item::drawSelf(ATD::FrameBuffer &target) const
{
	for (size_t drawIndex = 0; drawIndex < m_drawables.size(); drawIndex++) {
		m_drawables[drawIndex]->drawSelf(target);
	}
}

void ATD::Menu::Item::onShift(const ATD::Vector2L &positionDelta)
{}

void ATD::Menu::Item::onHoverStart()
{}

void ATD::Menu::Item::onHoverEnd()
{}

void ATD::Menu::Item::onActionStart()
{}

void ATD::Menu::Item::onActionUpdate()
{}

void ATD::Menu::Item::onActionEnd()
{}


/* ATD::Menu::Layer: */

ATD::Menu::Layer::Layer(bool opaque)
	: FrameBuffer::Drawable()
	, m_opaque(opaque)
	, m_isEnabled(true)
	, m_itemsActive()
	, m_itemsInactive()
{}

void ATD::Menu::Layer::add(Item *item)
{
	if (item->isActive()) {
		m_itemsActive.push_back(item);
	} else {
		m_itemsInactive.push_back(item);
	}
}

void ATD::Menu::Layer::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
}

bool ATD::Menu::Layer::probeMouse(const ATD::Vector2L &mouseTruePosition, 
		Item **retCurActivePtr)
{
	Item *curInactivePtr = nullptr;
	Item *curActivePtr = nullptr;

	for (size_t inactiveIndex = 0; 
			inactiveIndex < m_itemsInactive.size(); 
			inactiveIndex++) {
		Item *item = m_itemsInactive[inactiveIndex];
		if (item->getBounds().contains(mouseTruePosition)) {
			curInactivePtr = item;
			break;
		}
	}

	for (size_t activeIndex = 0; 
			activeIndex < m_itemsActive.size(); 
			activeIndex++) {
		Item *item = m_itemsActive[activeIndex];
		if (item->getBounds().contains(mouseTruePosition)) {
			curActivePtr = item;
			break;
		}
	}

	if (curActivePtr) {
		if (curActivePtr->isEnabled()) {
			*retCurActivePtr = curActivePtr;
		}
		return true;
	}

	return (static_cast<bool>(curInactivePtr));
}

void ATD::Menu::Layer::drawSelf(ATD::FrameBuffer &target) const
{
	/* First, draw inactive items. */
	for (size_t inactiveIndex = 0; 
			inactiveIndex < m_itemsInactive.size(); 
			inactiveIndex++) {
		m_itemsInactive[inactiveIndex]->drawSelf(target);
	}

	/* Then draw active items, so they will be visible. */
	for (size_t activeIndex = 0; 
			activeIndex < m_itemsActive.size(); 
			activeIndex++) {
		m_itemsActive[activeIndex]->drawSelf(target);
	}
}

void ATD::Menu::Layer::onEmptyClick()
{}


/* ATD::Menu: */

ATD::Menu::Menu()
	: FrameBuffer::Drawable()
	, m_layers()
	, m_itemHover(nullptr)
	, m_itemAction(nullptr)
//	, m_mouseButtonId(Mouse::NONE)
//	, m_mouseButtonState()
	, m_isMouseBusy(false)
{}

void ATD::Menu::add(const ATD::Menu::Layer &layer)
{
	m_layers.push_back(layer);
}

void ATD::Menu::update(const ATD::Keyboard &kb, 
		const ATD::Mouse &mouse, 
		const ATD::Transform2D *reverseTransformPtr)
{
	/* Cache mouse state. */
	/* Mouse::ButtonId mouseButton = mouse.button(); // FIXME: Use later */
	KeyState mouseState = mouse.state();

	if (m_itemAction) {
		/* Update focused item: */
		m_isMouseBusy = true;
	   	if (!mouseState.isHeld()) {
			m_itemAction->onActionEnd();
			m_itemAction = nullptr;
		} else {
			if (m_itemAction->isDraggable()) {
				m_itemAction->onActionUpdate();
			}
		}
	} else {
		const Vector2L mousePosition = static_cast<Vector2L>(mouse.position());

		Item *curItem = nullptr;
		Layer *curLayerOpaque = nullptr;
		bool isMouseBusy = false;

		for (size_t lRIndex = 0; lRIndex < m_layers.size(); lRIndex++) {
			Layer &layer = m_layers[lRIndex];
			if(layer.isEnabled()) {
				if (layer.probeMouse(mousePosition, &curItem)) {
					isMouseBusy = true;
					break;
				} else {
					if (layer.isOpaque()) {
						curLayerOpaque = &layer;
						isMouseBusy = true;
						break;
					}
				}
			}
		}

		m_isMouseBusy = isMouseBusy;

		if (curItem) {
			if (mouseState.isHeldStart()) {
				if (m_itemHover) {
					m_itemHover->onHoverEnd();
					m_itemHover = nullptr;
				}

				m_itemAction = curItem;
				m_itemAction->onActionStart();

				/* FIXME: Separate left/right/wheel click later. */
			} else {
				if (m_itemHover) {
					if (curItem != m_itemHover) {
						m_itemHover->onHoverEnd();

						m_itemHover = curItem;
						m_itemHover->onHoverStart();
					}
				}
			}
		} else {
			if (m_itemHover) {
				m_itemHover->onHoverEnd();
				m_itemHover = nullptr;
			}

			if (mouseState.isReleased() && curLayerOpaque) {
				curLayerOpaque->onEmptyClick();
			}
		}
	}
}

void ATD::Menu::drawSelf(ATD::FrameBuffer &target) const
{
	for (size_t lIndex = 0; lIndex < m_layers.size(); lIndex++) {
		if (m_layers[lIndex].isEnabled()) {
			m_layers[lIndex].drawSelf(target);
		}
	}
}


