
#ifndef UTILITY_HXX
#define UTILITY_HXX

#include <utility>

#include "Types.hxx"

#include <boost/regex.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

class EventCategoryManager;

IdSet parseIdSetFile(const boost::filesystem::path &path);

boost::gregorian::date extractDateFromPath(const boost::filesystem::path &path);

std::pair<boost::regex,HandlerFunction>
getImplementationForEvent(const std::string& event, const std::string& filenameRegex);

std::pair<boost::regex,HandlerFunction>
getImplementationForInfixEvent(const std::string& event);

std::pair<boost::regex,HandlerFunction>
getImplementationForPrefixEvent(const std::string& event);

DaySummary parseDay(const boost::filesystem::path &path,
                    const EventCategoryManager &categoryManager);

GlobalState parseEventsForGlobalState(const boost::filesystem::path &path,
                                      const EventCategoryManager &categoryManager);

void parseYearForGlobalState(const boost::filesystem::path &path,
                             const EventCategoryManager &categoryManager,
                             GlobalState &result);

void parseMonthForGlobalState(const boost::filesystem::path &monthPath,
                              const EventCategoryManager &categoryManager,
                              GlobalState &result);

void insertDaySummaryIntoGlobalState(GlobalState &globalState,
                                     const DaySummary &daySummary,
                                     const EventCategoryManager &categoryManager);

void insertCategorySummaryIntoCategoryState(CategoryState &categoryState,
        const CategorySummary &categorySummary,
        const EventCategoryManager &categoryManager,
        const boost::gregorian::date &date);

#endif // UTILITY_HXX
