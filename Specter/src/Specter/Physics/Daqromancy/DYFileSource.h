#ifndef DY_FILE_SOURCE_H
#define DY_FILE_SOURCE_H

#include "DaqGrimoire.h"
#include "Specter/Physics/DataSource.h"
#include <filesystem>

namespace Specter {

	class DYFileSource : public DataSource
	{
	public:
		DYFileSource(const std::string& directory, int channels_per_board = 16);
		virtual ~DYFileSource();

		virtual const SpecData& GetData() override;

	private:
		void CollectFiles();
		bool GetNextHit();

		std::filesystem::path m_directory;
		static constexpr std::string_view s_extension = ".dybin";
		int m_channelsPerBoard;

		std::vector<DaqGrimoire::DYFileReader> m_files;
		DaqGrimoire::DYListData m_dyHit;
		std::size_t m_startIndex;

		uint64_t m_totalDataHits;
	};
}

#endif