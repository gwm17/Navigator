//
//  ParameterMap.cpp
//  Navigator
//
//  Created by Gordon McCann on 1/7/22.
//

#include "ParameterMap.h"

namespace Navigator {

    NavParameter::NavParameter() :
        m_name(""), m_pdata(nullptr)
    {
    }

    NavParameter::NavParameter(const std::string& name) :
        m_name(name)
    {
        NAV_INFO("Making a new parameter named {0}...",name);
        m_pdata = nullptr;
        ParameterMap& map = ParameterMap::GetInstance();
        auto iter = map.find(name);
        if(iter == map.end())
        {
            NAV_INFO("Added it to the map.");
            map.AddParameter(name);
        }
        
        NAV_INFO("Setting the memory...");
        map.SetParameter(name, m_pdata);
    }

    NavParameter::~NavParameter() {}

    void NavParameter::SetParameter(const std::string& name)
    {
        ParameterMap& map = ParameterMap::GetInstance();
        auto iter = map.find(name);
        if(iter == map.end())
            map.AddParameter(name);
        map.SetParameter(name, m_pdata);
    }

    ParameterMap* ParameterMap::s_instance = new ParameterMap();

    ParameterMap::ParameterMap()
    {
    }

    ParameterMap::~ParameterMap() {}

    double ParameterMap::GetParameterValue(const std::string& name)
    {
        auto iter = m_map.find(name);
        if(iter != m_map.end())
            return iter->second->value;
        else
            return 0.0;
    }

    bool ParameterMap::IsParameterValid(const std::string& name)
    {
        auto iter = m_map.find(name);
        if(iter != m_map.end())
            return iter->second->validFlag;
        else
            return false;
    }

    void ParameterMap::InvalidateParameters()
    {
        for(auto& iter : m_map)
        {
            iter.second->validFlag = false;
            iter.second->value = 0.0;
        }
    }

    std::vector<std::string> ParameterMap::GetListOfParameters()
    {
        std::vector<std::string> list;
        list.reserve(m_map.size());
        for (auto iter : m_map)
        {
            list.push_back(iter.first);
        }

        return list;
    }
}
