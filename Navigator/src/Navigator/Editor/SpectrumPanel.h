#ifndef SPECTRUM_PANEL_H
#define SPECTRUM_PANEL_H

#include "Navigator/HistogramMap.h"
#include "Navigator/ParameterMap.h"
#include "Navigator/CutMap.h"
#include "imgui.h"

namespace Navigator {

	class NAV_API SpectrumPanel
	{
	public:
		SpectrumPanel();
		~SpectrumPanel();
	
		void OnImGuiRender();
		inline const std::string& GetZoomedOnHistogram() { return m_zoomedGram;  }
		inline const bool IsZoomed() { return m_zoomedFlag;  }

	private:
		std::vector<std::string> m_selectedGrams;
		bool m_zoomedFlag;
        bool m_cutModeFlag;
		std::string m_zoomedGram;
		int m_tableSizes[2];
		int m_totalSlots;
		CutParams m_newCutParams;
		std::vector<double> m_newCutX;
		std::vector<double> m_newCutY;
	};

}

#endif
