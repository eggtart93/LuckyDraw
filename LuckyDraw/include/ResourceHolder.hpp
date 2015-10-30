#ifndef RESOURCEHOLDER_HPP_INCLUDED
#define RESOURCEHOLDER_HPP_INCLUDED

#include <string>
#include <memory>
#include <map>
#include <cassert>

template <typename Resource, typename ID>
class ResourceHolder
{
    public:
        void load(ID id, const std::string& filePath);

        template <typename Param>
        void load(ID id, const std::string& filePath, Param& otherParam);
        Resource& get(ID id);
        const Resource& get(ID id) const;

    private:
        void addResource(ID, std::unique_ptr<Resource> resource);

    private:
        std::map<ID, std::unique_ptr<Resource>> mResourcesPool;
};

template <typename Resource, typename ID>
void ResourceHolder<Resource,ID>::load(ID id, const std::string& filePath)
{
    std::unique_ptr<Resource> res(new Resource());
    if(!res->loadFromFile(filePath))
    {
        throw std::runtime_error("ResourceHolder::load: Failed to load "
                                 + filePath);
    }

    addResource(id, std::move(res));
}

template <typename Resource, typename ID>
template <typename Param>
void ResourceHolder<Resource,ID>::load(ID id, const std::string& filePath, Param& otherParam)
{
    std::unique_ptr<Resource> res(new Resource());
    if(!res->loadFromFile(filePath, otherParam))
    {
        throw std::runtime_error("ResourceHolder::load: Failed to load "
                                 + filePath);
    }

    addResource(id, std::move(res));
}

template <typename Resource, typename ID>
Resource& ResourceHolder<Resource, ID>::get(ID id)
{
    auto found = mResourcesPool.find(id);
    assert(found != mResourcesPool.end());
    return *(found->second);
}

template <typename Resource, typename ID>
const Resource& ResourceHolder<Resource,ID>::get(ID id) const
{
    auto found = mResourcesPool.find(id);
    assert(found != mResourcesPool.end());
    return *(found->second);
}

template <typename Resource, typename ID>
void ResourceHolder<Resource,ID>::addResource(ID id, std::unique_ptr<Resource> resource)
{
    auto inserted = mResourcesPool.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

#endif // RESOURCEHOLDER_HPP_INCLUDED
