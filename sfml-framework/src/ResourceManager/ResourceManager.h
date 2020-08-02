#pragma once

#include <unordered_map>
#include <string>
#include "../Util/Log.h"

/**
    Holds all the resources of the game
*/
template<typename Resource>
class ResourceManager
{
    public:
        ResourceManager (const std::string& folder, const std::string& extension)
        :   m_folder    (ASSETS_PREFIX + folder + "/")
        ,   m_extension ("." + extension)
        { }

        const Resource& get(const std::string& name)
        {
            if (!exists(name)) {
                add(name);
            }

            return m_resources.at(name);
        }

        bool exists(const std::string& name) const
        {
            return m_resources.find(name) != m_resources.end();
        }

        void add(const std::string& name)
        {
            Resource r;

            std::string full_filename = getFullFilename(name);

            //if the resource fails to load, then it adds a default "fail" resource
            if(!r.loadFromFile(getFullFilename(full_filename))) {
                Resource fail;
                LOGE("Failed to add resource '%s' from '%s'", name.c_str(), full_filename.c_str());
                fail.loadFromFile(m_folder + "_fail_" + m_extension);
                m_resources.insert(std::make_pair(name, fail));
            }
            else {
                LOGV("Adding resource '%s' from '%s'", name.c_str(), full_filename.c_str());
                m_resources.insert(std::make_pair(name, r));
            }
        }

    private:
        std::string getFullFilename(const std::string& name)
        {
            return m_folder + name + m_extension;
        }

        const std::string m_folder;
        const std::string m_extension;

        std::unordered_map<std::string, Resource> m_resources;
};