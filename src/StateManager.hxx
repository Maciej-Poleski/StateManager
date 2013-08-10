/*
 * Copyright 2013  Maciej Poleski
 */

#ifndef STATEMANAGER_HXX
#define STATEMANAGER_HXX

#include "Types.hxx"

#include <boost/filesystem/path.hpp>

#include "EventCategoryManager.hxx"

class StateManager
{
public:
    StateManager(const boost::filesystem::path &path);

private:
    boost::filesystem::path _basePath;
    EventCategoryManager _categoryManager;

    GlobalState _globalState;
};

#endif // STATEMANAGER_HXX
