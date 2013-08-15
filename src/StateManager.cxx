/*
 * Copyright 2013  Maciej Poleski
 */

#include "StateManager.hxx"

#include "Utility.hxx"

StateManager::StateManager(const boost::filesystem::path &path) : _basePath(path)
{
    auto impl=getImplementationForInfixEvent("do prania");
    _categoryManager.registerHandler(impl.first,impl.second);
    impl=getImplementationForInfixEvent("z prania");
    _categoryManager.registerHandler(impl.first,impl.second);
    impl=getImplementationForPrefixEvent("nowa");
    _categoryManager.registerHandler(impl.first,impl.second);
    impl=getImplementationForPrefixEvent("stara");
    _categoryManager.registerHandler(impl.first,impl.second);

    _categoryManager.setEventPriority("nowa",0);
    _categoryManager.setEventPriority("stara",0);
    _categoryManager.setEventPriority("z prania",1);
    _categoryManager.setEventPriority("do prania",1);

    _categoryManager.addInitializationEvent("nowa");
    _categoryManager.addInitializationEvent("stara");

    _categoryManager.addAcceptableStateTransition("nowa","do prania");
    _categoryManager.addAcceptableStateTransition("stara","do prania");

    _categoryManager.addAcceptableStateTransition("do prania","z prania");
    _categoryManager.addAcceptableStateTransition("z prania","do prania");


    _globalState=parseEventsForGlobalState(path/"zdarzenia",_categoryManager);
}
