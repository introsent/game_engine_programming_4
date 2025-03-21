#pragma once
#include <string>

#include "Component.h"

namespace dae
{
	class Plot final
	{
	public:
		Plot(std::string&& name);

		void RenderPlot() const;

		void AddData(const std::vector<float>& newData);
		void ClearData();

		bool IsEmpty() const;


	private:
		std::string m_plotName;

		float m_maxY {};

		std::vector<float> m_dataX;
		std::vector<std::vector<float>> m_dataY;
	

	};
}

