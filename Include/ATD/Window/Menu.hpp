/**
 * @file      
 * @brief     Menu base class implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Rectangle.hpp>
#include <ATD/Graphics/FrameBuffer.hpp>
#include <ATD/Window/Keyboard.hpp>
#include <ATD/Window/KeyState.hpp>
#include <ATD/Window/Mouse.hpp>
#include <ATD/Window/Window.hpp>

#include <memory>
#include <vector>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Menu : public FrameBuffer::Drawable
{
public:

	/* Idea:
	 * This is a game menu - not graphical redactor menu.
	 * Hence I shall keep it as simple as possible and optimized as 
	 * possible.
	 *
	 * No list inside list inside list of options.
	 *
	 * Games like Terraria simply have menu layers. User input is 
	 * processed on top layer first, than on second layer, etc... 
	 * and if no layer processed the input, then it is passed to the 
	 * game itself.
	 *
	 * Assumption 1: Active items on each layer do not intersect. 
	 * E.g. there shall not be a button inside a button.
	 *
	 * Assumption 2: Active items either clickable or draggable. 
	 * Clickables are buttons, flags, selectable tools, text fields, etc..
	 * Draggables are sliders and... that's it. I don't even know, whether 
	 * I'll need drag-and-drop mechanics for any game. 
	 *
	 * Assumption 3: Active items can have focus, but the focus is named 
	 * action. Action is gained when mouse is pressed upon item, continues, 
	 * while the mouse is held (no matter, where it moves), and is lost 
	 * when mouse is released.
	 *
	 * Assumption 4: There should be idle items (e.g. frames) that do nothing 
	 * when mouse clicks/drags upon 'em, but do not pass those clicks to the 
	 * game.
	 *
	 * Assumption 5: Idle items on a single layer do not intersect.
	 *
	 * Assumption 6: Layers should have const parameter 'opaque' - whether 
	 * pass input to the next layer. And if they are not opaque, they shall 
	 * have a callback for a click, but not on any item of the layer.
	 *
	 * Assumption 7: Layers should have variable parameter 'isExposed'. 
	 * And also callbacks for gaining and losing exposure. E.g. pause and 
	 * resume game automatically.
	 *
	 * Assumption 8: Menus are assumed to be drawn on Window itself. Or 
	 * on a transparent FrameBuffer with same size and zero offset.
	 * */

	/**
	 * @brief ...
	 * @class ... */
	class Item : public ATD::FrameBuffer::Drawable
	{
	public:
		typedef std::vector<ATD::FrameBuffer::Drawable2D *> Drawables;

		static const uint32_t IS_ACTIVE_CFLAG = 0x00000001 << 0;
		static const uint32_t IS_DRAGGABLE_CFLAG = 0x00000001 << 1;


		/**
		 * @brief ...
		 * @param size      - ...
		 * @param drawables - ...
		 * @param cFlags    - ... */
		Item(const ATD::Vector2S &size, 
				const Drawables &drawables = Drawables(), 
				uint32_t cFlags = 0x0);


		/* Const parameter getters: */

		/**
		 * @brief ...
		 * @return ... */
		inline bool isActive() const
		{ return (m_cFlags & IS_ACTIVE_CFLAG); }

		/**
		 * @brief ...
		 * @return ... */
		inline bool isIdle() const
		{ return !isActive(); }

		/**
		 * @brief ...
		 * @return ... */
		inline bool isClickable() const
		{ return ((m_cFlags & IS_ACTIVE_CFLAG) && 
				(~m_cFlags & IS_DRAGGABLE_CFLAG)); }

		/**
		 * @brief ...
		 * @return ... */
		inline bool isDraggable() const
		{ return (m_cFlags & (IS_ACTIVE_CFLAG | IS_DRAGGABLE_CFLAG)); }


		/* --- */

		/**
		 * @brief ...
		 * @param position - ... */
		void setPosition(const ATD::Vector2L &position);

		/**
		 * @brief ...
		 * @return ... */
		inline const RectL &getBounds() const
		{ return m_bounds; }

		/**
		 * @brief ...
		 * @param enabled - ... */
		void setEnabled(bool enabled);

		/**
		 * @brief ...
		 * @return ... */
		inline bool isEnabled() const
		{ return m_isEnabled; }

		/**
		 * @brief ...
		 * @param target - ... */
		virtual void drawSelf(FrameBuffer &target) const override;


		/* Overridable callbacks: */

		/**
		 * @brief ...
		 * @param positionDelta - ... */
		void onShift(const Vector2L &positionDelta);

		/**
		 * @brief ... */
		void onHoverStart();

		/**
		 * @brief ... */
		void onHoverEnd();

		/**
		 * @brief ... */
		void onActionStart();

		/**
		 * @brief ... */
		void onActionUpdate();

		/**
		 * @brief ... */
		void onActionEnd();

	private:
		/* Set once in the constructor. */
		const Drawables m_drawables;
		const uint32_t m_cFlags; /* Constant flags. */

		ATD::RectL m_bounds;
		bool m_isEnabled;

		// TODO: Is visible
	};


	/**
	 * @brief ...
	 * @class ... */
	class Layer : public FrameBuffer::Drawable
	{
	public:
		/**
		 * @brief ...
		 * @param opaque - ... */
		Layer(bool opaque = false);

		/**
		 * @brief ...
		 * @return ... */
		inline bool isOpaque() const
		{ return m_opaque; }

		/**
		 * @brief ...
		 * @param item - ... */
		void add(Item *item);

		/**
		 * @brief ...
		 * @param setEnabled - ... */
		void setEnabled(bool enabled);

		/**
		 * @brief ...
		 * @return ... */
		inline bool isEnabled() const
		{ return m_isEnabled; }

		/**
		 * @brief ...
		 * @param mouseTruePosition - ...
		 * @param retCurActivePtr   - ... */
		bool probeMouse(const Vector2L &mouseTruePosition, 
				Item **retCurActivePtr);

		/**
		 * @brief ...
		 * @param target - ... */
		void drawSelf(FrameBuffer &target) const override;

		/**
		 * @brief ...
		 *
		 * Overrideable callback.
		 * Called for opaque layers only when clicked not on layer. */
		void onEmptyClick();

	private:
		const bool m_opaque;

		bool m_isEnabled;

		std::vector<Item *> m_itemsActive;
		std::vector<Item *> m_itemsInactive;
	};


	/**
	 * @brief ... */
	Menu();

	/**
	 * @brief ...
	 * @param layer - ... */
	void add(const Layer &layer);

	/* FIXME: Shall I pass time, passed since prev. update? */
	/**
	 * @brief ...
	 * @param kb                  - ...
	 * @param mouse               - ...
	 * @param reverseTransformPtr - ... */
	void update(const Keyboard &kb, 
			const Mouse &mouse, 
			const Transform2D *reverseTransformPtr = nullptr);

	/**
	 * @brief ...
	 * @return ...
	 *
	 * Shall be called after HandleMouse(...). */
	inline bool isMouseBusy() const
	{ return m_isMouseBusy; }

	/**
	 * @brief ...
	 * @param target - ... */
	void drawSelf(FrameBuffer &target) const override;

protected:
	std::vector<Layer> m_layers;
	Item *m_itemHover;
	Item *m_itemAction;

	bool m_isMouseBusy;
};

} /* namespace ATD */


