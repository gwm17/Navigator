#ifndef APP_EVENT_H
#define APP_EVENT_H

#include "Navigator/NavCore.h"
#include "Event.h"

namespace Navigator {

	class NAV_API WindowCloseEvent : public Event 
	{
	public:
		WindowCloseEvent() {};

		EVENT_CATEGORY_SETUP(EventCategoryApp)
		EVENT_TYPE_SETUP(WindowClose)
	};

	class NAV_API WindowResizeEvent : public Event 
	{
	public:
		WindowResizeEvent(int x, int y) :
			m_xSize(x), m_ySize(y)
		{
		}

		inline int GetXSize() { return m_xSize; }
		inline int GetYSize() { return m_ySize; }
		std::string ToString() const override 
		{
			std::stringstream ss;
			ss << GetName() << " to size: (" << m_xSize << ", " << m_ySize << ")";
			return ss.str();
		}

		EVENT_CATEGORY_SETUP(EventCategoryApp)
		EVENT_TYPE_SETUP(WindowResize)

	private:
		int m_xSize, m_ySize;
	};

	class NAV_API AppUpdateEvent : public Event 
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CATEGORY_SETUP(EventCategoryApp)
		EVENT_TYPE_SETUP(AppUpdate)
	};

}

#endif