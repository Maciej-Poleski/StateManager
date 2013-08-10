/*
 * Copyright 2013  Maciej Poleski
 */

#include "EventCategoryManager.hxx"

FileSummary EventCategoryManager::handleFile(const boost::filesystem::path& file) const
{
    for(const auto h : _handlers)
    {
        if(boost::regex_match(file.filename().string(),h.first))
            return h.second(file);
    }
    throw std::runtime_error("There is no registered handler for file "+file.string());
}

void EventCategoryManager::registerHandler(const boost::regex& trigger, HandlerFunction handler)
{
    _handlers.push_back(std::make_pair(trigger,handler));
}

void EventCategoryManager::setEventPriority(const std::string& event, std::size_t priority)
{
    _eventPriority[event]=priority;
}

std::function<bool(const std::string&,const std::string&)>
EventCategoryManager::eventComparator() const
{
    return [this](const std::string &lhs,const std::string &rhs)
    {
        return (_eventPriority.at(lhs)<_eventPriority.at(rhs)) ||
               (_eventPriority.at(lhs)==_eventPriority.at(rhs) && lhs<rhs);
    };
}

void EventCategoryManager::addInitializationEvent(const std::string& event)
{
    _initializationEvents.push_back(event);
}

bool EventCategoryManager::isInitializationEvent(const std::string& event) const
{
    return std::find(_initializationEvents.begin(),_initializationEvents.end(),
                     event)!=_initializationEvents.end();
}
