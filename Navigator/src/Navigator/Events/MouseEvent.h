/*
	KeyEvent.h
	Events related to mouse. Again, based on @TheCherno's work.

	GWM -- Feb 2022
*/
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "Navigator/NavCore.h"
#include "Event.h"

namespace Navigator {

	class NAV_API MouseMovedEvent : public Event 
	{
	public:
		MouseMovedEvent(float x, float y) :
			m_xPos(x), m_yPos(y)
		{
		}

		inline float GetXPosition() { return m_xPos; }
		inline float GetYPosition() { return m_yPos; }
		std::string ToString() const override 
		{
			std::stringstream ss;
			ss << GetName() << " to position (" << m_xPos << ", " << m_yPos << ")";
			return ss.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryMouse | EventCategoryInput)
		EVENT_TYPE_SETUP(MouseMoved)

	private:
		float m_xPos, m_yPos;
	};

	class NAV_API MouseScrolledEvent : public Event 
	{
	public:
		MouseScrolledEvent(float x, float y) :
			m_xOffset(x), m_yOffset(y)
		{
		}

		inline float GetXOffset() { return m_xOffset; }
		inline float GetYOffset() { return m_yOffset; }
		std::string ToString() const override 
		{
			std::stringstream ss;
			ss << GetName() << " to offset (" << m_xOffset << ", " << m_yOffset << ")";
			return ss.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryMouse | EventCategoryInput)
		EVENT_TYPE_SETUP(MouseScrolled)

	private:
		float m_xOffset, m_yOffset;
	};

	class NAV_API MouseButtonPressedEvent : public Event 
	{
	public:
		MouseButtonPressedEvent(int code) :
			m_buttonCode(code)
		{
		}

		inline int GetButtonCode() { return m_buttonCode; }
		std::string ToString() const override 
		{
			std::stringstream ss;
			ss << GetName() << " with button code " << m_buttonCode;
			return ss.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryMouse | EventCategoryInput)
		EVENT_TYPE_SETUP(MouseButtonPressed)

	private:
		int m_buttonCode;
	};

	class NAV_API MouseButtonReleasedEvent : public Event 
	{
	public:
		MouseButtonReleasedEvent(int code) :
			m_buttonCode(code)
		{
		}

		inline int GetButtonCode() { return m_buttonCode; }
		std::string ToString() const override 
		{
			std::stringstream ss;
			ss << GetName() << " with button code " << m_buttonCode;
			return ss.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryMouse | EventCategoryInput)
		EVENT_TYPE_SETUP(MouseButtonReleased)

	private:
		int m_buttonCode;
	};
}

#endif