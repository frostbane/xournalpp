#include "util/SaveNameUtils.h"

#include <sstream>            // for stringstream
#include <chrono>             // for time
#include <iomanip>            // for put_time

#include "util/PathUtil.h"    // for clearExtensions


auto SaveNameUtils::parseFilenameFromWildcardString(const std::string& wildcardString, const fs::path& defaultFilePath) -> std::string {
    std::string saveString = wildcardString;
    size_t pos = saveString.find(DEFAULT_WILDCARD_START);

    // parse all wildcards until none are left
    while (pos != std::string::npos) {
        size_t wildcardStartLength = std::strlen(DEFAULT_WILDCARD_START);
        size_t endPos = saveString.find(DEFAULT_WILDCARD_END, pos + wildcardStartLength);
        if (endPos == std::string::npos) {
            break;
        }
        std::string parsedWildcard = parseWildcard(saveString.substr(pos + wildcardStartLength, endPos - pos - wildcardStartLength), defaultFilePath);
        saveString.replace(pos, endPos + 1 - pos, parsedWildcard);
        pos += parsedWildcard.size();
        pos = saveString.find(DEFAULT_WILDCARD_START, pos);
    }

    return saveString;
}

auto SaveNameUtils::parseWildcard(const std::string& wildcard, const fs::path& defaultFilePath) -> std::string {
    if (wildcard == WILDCARD_NAME) {
        fs::path path = defaultFilePath;
        Util::clearExtensions(path, ".pdf");
        return path.u8string();
    }
    if (wildcard == WILDCARD_DATE || wildcard == WILDCARD_TIME) {
        std::time_t time = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );
        std::stringstream timestring;
        timestring << std::put_time(std::localtime(&time),
                                wildcard == WILDCARD_DATE ? "%Y-%m-%d" : "%X");
        return timestring.str();
    } 
    // not a valid wildcard
    return "";
}
