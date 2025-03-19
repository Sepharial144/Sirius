#include "CommandLine.hpp"

#include "logger/logger.hpp"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iterator>
#include <sstream> // IWYU pragma: keep
#include <string>
#include <type_traits>

namespace cl {

    CommandLine::CommandLine(std::string description)
        : mDescription(std::move(description))
    {
        logger::debug_high("Bring up command line ...");
        logger::debug_high("Bring up command line ... complete");
    }

    void CommandLine::addArgument(
        std::vector<std::string> const& flags, Value const& value, std::string const& help)
    {
        logger::debug_high("CommandLine::addArgument");
        mArguments.emplace_back(Argument{ flags, value, help });
    }

    void CommandLine::printHelp(std::ostream& os) const
    {
        logger::debug_high("CommandLine::printHelp");
        os << mDescription << &std::endl;

        int maxFlagLength = 0;

        for (auto const& argument : mArguments) {
            int flagLength = 0;
            for (auto const& flag : argument.mFlags) {
                flagLength += static_cast<int>(flag.size()) + 2;
            }
            maxFlagLength = std::max(maxFlagLength, flagLength);
        }

        for (auto const& argument : mArguments) {
            std::string flags;
            for (auto const& flag : argument.mFlags) {
                flags += flag + ", ";
            }

            std::stringstream sstr;
            sstr << &std::left << std::setw(maxFlagLength) << flags.substr(0U, flags.size() - 2U);

            size_t spacePos = 0U;
            size_t lineWidth = 0U;
            const int maxLineWidth = 60;
            while (spacePos != std::string::npos) {
                const size_t nextspacePos = argument.mHelp.find_first_of(' ', spacePos + 1U);
                sstr << argument.mHelp.substr(spacePos, nextspacePos - spacePos);
                lineWidth += nextspacePos - spacePos;
                spacePos = nextspacePos;

                if (lineWidth > uint32_t(maxLineWidth)) {
                    os << sstr.str() << &std::endl;
                    sstr = std::stringstream();
                    sstr << &std::left << std::setw(maxFlagLength - 1) << " ";
                    lineWidth = 0U;
                }
            }
        }
    }

    auto CommandLine::setValueForArgument(
        const std::string& flag, const std::string& value, bool& valueIsSeparate) const -> bool
    {
        logger::debug_high(
            "CommandLine::setValueForArgument flag=",
            flag,
            " , value=",
            value,
            " ,valueIsSeparate=",
            (valueIsSeparate ? "true" : "false"));

        bool foundArgument = false;

        for (auto const& argument : mArguments) {
            if (std::find(argument.mFlags.begin(), argument.mFlags.end(), flag)
                != std::end(argument.mFlags)) {

                foundArgument = true;

                if (std::holds_alternative<bool*>(argument.mValue)) {
                    if (!value.empty() && (value != "true") && (value != "false")) {
                        valueIsSeparate = false;
                    }
                    *std::get<bool*>(argument.mValue) = (value != "false");
                }
                else if (std::holds_alternative<int32_t*>(argument.mValue)) {
                    const int32_t valueInt = stoi(value);
                    *std::get<int32_t*>(argument.mValue) = valueInt;
                }
                else if (std::holds_alternative<std::string*>(argument.mValue)) {
                    std::cout << value << &std::endl;
                    *std::get<std::string*>(argument.mValue) = value;
                }
                break;
            }
        }

        return foundArgument;
    }

    void CommandLine::parse(std::vector<char*> args) const
    {
        logger::debug_high("CommandLine::parse");
        bool skipFlag = false;
        for (auto arg = args.begin(); arg != args.end(); ++arg) {
            if (skipFlag) {
                skipFlag = false;
                continue;
            }
            if (arg == args.begin()) {
                continue;
            }

            std::string flag(*arg);
            std::string value;
            bool valueIsSeparate = false;

            const size_t equalPos = flag.find('=');
            if (equalPos != std::string::npos) {
                value = flag.substr(equalPos + 1U);
                flag = flag.substr(0U, equalPos);
            }
            else if ((arg + 1U) != args.end()) {
                value = *(arg + 1U);
                valueIsSeparate = true;
            }

            const bool foundArgument = setValueForArgument(flag, value, valueIsSeparate);

            if (foundArgument && valueIsSeparate) {
                skipFlag = true; // modification like ++arg cause M6-5-3 error
            }
        }
    }

} // namespace cl