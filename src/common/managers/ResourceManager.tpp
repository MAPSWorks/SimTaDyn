// -*- c++ -*- Coloration Syntaxique pour Emacs
//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef RESOURCE_MANAGER_TPP_
#  define RESOURCE_MANAGER_TPP_

#  include "Singleton.hpp"
#  include "Resource.tpp"
#  include "Logger.hpp"
//#  include <map>
#  include <mutex>

// **************************************************************
//! \brief
// **************************************************************
//template <class T, class ID>
template <class T>
struct ResourceHolder
{
  //  std::map<ID, Resource<T>*> m_resources;
  std::map<std::string, Resource<T>*> m_resources;
};

// **************************************************************
//! \brief
// **************************************************************
class ResourceManager
  : public Singleton<ResourceManager>,
    public CScatteredHierarchy<ResourceList, ResourceHolder>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  friend class Singleton<ResourceManager>;

  //------------------------------------------------------------------
  //! \brief Allow the resource to call methods like remove()
  //------------------------------------------------------------------
  // friend class Resource<T, ID>;

public:

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  ResourceManager() { }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  virtual ~ResourceManager()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    LOGI("Destroying the ResourceManager");
    /*if (ResourceHolder<T>::m_resources.empty())
      return ;

    for (auto it: ResourceHolder<T>::m_resources)
      {
        auto resource = it.second;
        uint32_t n = resource->owners();
        if (0U == n)
          {
            // Created but not used by someone
            // so can be released.
            resource->dispose();
          }
        else
          {
            CPP_LOG(logger::Warning)
              << "  ==> The resource "
              << resource->object().id()
              << " is still aquired by "
              << n << " owners\n";
          }
          }*/
    // FIXME: mettre ca dans Resource ????
  }

public:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  template <class T>
  Resource<T> *add()
  {
    Resource<T> *r = create<T>();
    if (add(r))
      {
        return r;
      }
    else
      {
        delete r;
        return nullptr;
      }
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is needed.
  //! \param id the unique identifier of the resource.
  //! \return the adress of the resource if it exists, else return
  //! nullptr.
  //------------------------------------------------------------------
  template <class T>
  inline T *acquire(std::string const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    LOGI("Acquiring the resource #%s", id.c_str());
    auto it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        it->second->acquire();
        return &(it->second->object());
      }

    CPP_LOG(logger::Error)
      << "Trying to acquire a non-existent resource "
      << id << ". This current action is ignored !\n";
    return nullptr;
  }

  template <class T>
  inline T *acquire(Resource<T> &resource)
  {
    resource.acquire();
    return resource.object();
  }

  //------------------------------------------------------------------
  //! Get the resource address in read only access.
  //------------------------------------------------------------------
  template <class T>
  inline Resource<T> *find(std::string const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        return it->second;
      }
    return nullptr;
  }

  //------------------------------------------------------------------
  //! \brief Call it when the resources is no longer needed. If someone
  //! is still using it, the resource is not destroyed. If nobody uses
  //! it, the resource is destroyed.
  //------------------------------------------------------------------
  template <class T>
  void dispose(std::string const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        uint32_t owners = it->second->dispose();
        LOGI("Disposing of the resource #%s (%u owner(s))",
             id.c_str(), owners);
        if (0U == owners)
          {
            // Note: compared to
            // http://loulou.developpez.com/tutoriels/moteur3d/ this
            // line was added because only the ResourceManager can
            // dispose of the resource.
            LOGI("Destroying the resource #%s", id.c_str());
            ResourceHolder<T>::m_resources.erase(it);
          }
      }
    else
      {
        CPP_LOG(logger::Warning)
          << "Trying to dispose a non-existent resource #"
          << id << ". This current action is ignored !\n";
        return ;
      }
  }

  template <class T>
  void dispose(Resource<T> &resource)
  {
    resource.dispose();
  }

  //------------------------------------------------------------------
  //! \brief Return the number of resources currently stored.
  //------------------------------------------------------------------
  template <class T>
  inline uint32_t size()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    return ResourceHolder<T>::m_resources.size();
  }

protected:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  template <class T>
  Resource<T> *create()
  {
    return new Resource<T>();
  }

  //------------------------------------------------------------------
  //! \brief Insert an allocated resource in the list of resources.
  //------------------------------------------------------------------
  template <class T>
  bool add(Resource<T> *resource)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (nullptr == resource)
      {
        LOGF("Trying to add a resource with a NULL pointer");
        return false;
      }

    const std::string sid = std::to_string(resource->object().id());
    if (ResourceHolder<T>::m_resources.find(sid) != ResourceHolder<T>::m_resources.end())
      {
        CPP_LOG(logger::Warning)
          << "Trying to add a duplicated resource #"
          << sid << ". This current action is ignored !\n";
        return false;
      }

    CPP_LOG(logger::Info)
      << "Added the resource #" << sid
      << " to the ResourceManager\n";
    ResourceHolder<T>::m_resources[sid] = resource;
    return true;
  }

  //------------------------------------------------------------------
  //! \brief Remove an allocated resource in the list of resources.
  //------------------------------------------------------------------
  template <class T>
  void remove(std::string const& id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = ResourceHolder<T>::m_resources.find(id);
    if (it != ResourceHolder<T>::m_resources.end())
      {
        LOGI("Destroying the resource #%s", id.c_str());
        ResourceHolder<T>::m_resources.erase(it);
      }
    else
      {
        CPP_LOG(logger::Warning)
          << "Trying to remove a non-existent resource "
          << id << ". This current action is ignored !\n";
      }
  }

  //------------------------------------------------------------------
  //! \brief Allow several threads to access to the manager.
  //-----------------------------------------------------------------
  std::mutex m_mutex;
};

#endif /* RESOURCE_MANAGER_TPP_ */
