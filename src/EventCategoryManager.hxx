/*
 * Copyright 2013  Maciej Poleski
 */

#ifndef CATEGORYMANAGER_HXX
#define CATEGORYMANAGER_HXX

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <functional>
#include <unordered_set>

#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>
#include <boost/functional/hash.hpp>

#include "Types.hxx"

/**
 * Contains all means of state during process of handling input data
 */
class EventCategoryManager
{
public:
    /**
     * Extracts all informations from provided file
     *
     * @param file File to extract informations from
     */
    FileSummary handleFile(const boost::filesystem::path &file) const;

    /**
     * Register function to handle files matching provided regex and return
     * FileSummary struct.
     */
    void registerHandler(const boost::regex &trigger, HandlerFunction handler);

    void setEventPriority(const std::string &event,std::size_t priority);

    std::function<bool(const std::string&,const std::string&)> eventComparator() const;

    void addInitializationEvent(const std::string &event);

    bool isInitializationEvent(const std::string &event) const;

    void addAcceptableStateTransition(const std::string &from, const std::string &to);

    bool isAcceptableStateTransition(const std::string &from, const std::string &to) const;

private:
    std::vector<std::pair<boost::regex,HandlerFunction>> _handlers;
    std::map<std::string,std::size_t> _eventPriority;
    std::vector<std::string> _initializationEvents;
    std::unordered_set<std::pair<std::string,std::string>,boost::hash<std::pair<std::string,std::string>>> _acceptableStateTransitions;
};

#endif // CATEGORYMANAGER_HXX
