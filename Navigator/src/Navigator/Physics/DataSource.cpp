/*
	DataSource.cpp
	Abstract data source class. In Navigator a DataSource can be either an online (live) source or an offline (file) source. By default,
	Navigator has classes to handle CAEN CoMPASS data sources (files and online); other sources may be implemented as more use cases for Navigator become
	apparent.

	GWM -- Feb 2022
*/
#include "DataSource.h"
#include "Caen/CompassRun.h"
#include "Caen/CompassOnlineSource.h"

namespace Navigator {

	//loc=either an ip address or a file location, port=address port, or unused in case of file
	DataSource* CreateDataSource(const std::string& loc, const std::string& port, DataSource::SourceType type, uint16_t bitflags)
	{
		switch(type)
		{
			case DataSource::SourceType::CompassOffline : return new CompassRun(loc);
			case DataSource::SourceType::CompassOnline : return new CompassOnlineSource(loc, port, bitflags);
			case DataSource::SourceType::None : return nullptr;
		}
		NAV_WARN("Invalid DataSourceType at CreateDataSource!");
		return nullptr;
	}

	std::string ConvertDataSourceTypeToString(DataSource::SourceType type)
	{
		switch(type)
		{
			case DataSource::SourceType::None: return "None";
			case DataSource::SourceType::CompassOnline : return "CompassOnline";
			case DataSource::SourceType::CompassOffline : return "CompassOffline";
		}

		return "None";
	}
}
