#ifndef CUT_MAP_H
#define CUT_MAP_H

#include "NavCore.h"
#include "imgui.h"

#include <thread>

namespace Navigator {

	struct NAV_API CutParams
	{
        CutParams() {}
		CutParams(const std::string& n, const std::string& x, const std::string& y = "None") :
			name(n), x_par(x), y_par(y)
		{
		}

		std::string name = "None";
		std::string x_par = "None";
		std::string y_par = "None";
	};

	class NAV_API Cut
	{
	public:
        
		Cut(const CutParams& params) :
			m_params(params)
		{
		}

		virtual ~Cut() {}

		virtual bool IsInside() const = 0;
		virtual void Draw() const = 0;
		virtual bool Is1D() const = 0;
		virtual bool Is2D() const = 0;
		virtual std::vector<double> GetXValues() const = 0;
		virtual std::vector<double> GetYValues() const = 0;

		inline const std::string& GetName() const { return m_params.name; }
		inline const std::string& GetXParameter() const { return m_params.x_par; }
		inline const std::string& GetYParameter() const { return m_params.y_par;  }
        inline const CutParams& GetCutParams() const { return m_params; }
	protected:
		CutParams m_params;
	};

	class NAV_API Cut1D : public Cut
	{
	public:
		Cut1D(const CutParams& params, double min, double max);
		virtual ~Cut1D();
		virtual bool IsInside() const override;
		virtual void Draw() const override;
		virtual bool Is1D() const override { return true; }
		virtual bool Is2D() const override { return false; }
		virtual std::vector<double> GetXValues() const override { return std::vector<double>({ m_minVal, m_maxVal }); }
		virtual std::vector<double> GetYValues() const override { return std::vector<double>(); }

	private:
		double m_minVal, m_maxVal;
	};

	class NAV_API Cut2D : public Cut
	{
	public:
		Cut2D(const CutParams& params, const std::vector<double>& xpoints, const std::vector<double>& ypoints);
		virtual ~Cut2D();
		virtual bool IsInside() const override;
		virtual void Draw() const override;
		virtual bool Is1D() const override { return false; }
		virtual bool Is2D() const override { return true; }
		virtual std::vector<double> GetXValues() const override { return m_xpoints; }
		virtual std::vector<double> GetYValues() const override { return m_ypoints; }

	private:
		std::vector<double> m_xpoints;
		std::vector<double> m_ypoints;
        const ImVec4 colorVec = {1.0, 0.0, 0.0, 0.5};
	};

	class NAV_API CutMap
	{
	public:
		CutMap();
		~CutMap();

		inline static CutMap& GetInstance() { return *s_instance; }

		inline void AddCut(const CutParams& params, double min, double max)
		{
			m_map[params.name].reset(new Cut1D(params, min, max));
		}
		inline void AddCut(const CutParams& params, const std::vector<double>& xpoints, const std::vector<double>& ypoints)
		{
			m_map[params.name].reset(new Cut2D(params, xpoints, ypoints));
		}
		inline void RemoveCut(const std::string& name)
		{
			m_map.erase(name);
		}

		void DrawCut(const std::string& name);
		bool IsInsideCut(const std::string& name);
		std::vector<double> GetCutXPoints(const std::string& name);
		std::vector<double> GetCutYPoints(const std::string& name);
		std::vector<CutParams> GetListOfCutParams();

	private:
		std::mutex m_cutMutex;
		std::unordered_map<std::string, std::shared_ptr<Cut>> m_map;

		static CutMap* s_instance;
	};
}

#endif
