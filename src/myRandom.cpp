#include <string>
#include <random>
#include <iostream>

#include "dependencies/json.hpp"

#include "myRandom.hpp"

// Constructor to initialize the random number generator with a seed.
// These are declared static so there is only one instance across all objects
// of the myRandom class.
std::random_device myRandom::rd;
std::mt19937 myRandom::gen(myRandom::rd());

int myRandom::generateRandomInt(int min, int max) {
    static std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

std::string myRandom::generateRandomString(size_t length) {
    constexpr char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    constexpr size_t chars_len = sizeof(chars) - 1;

    static std::uniform_int_distribution<> distrib(0, chars_len - 1);
    std::string randomStr;
    randomStr.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        randomStr += chars[distrib(gen)];
    }
    return randomStr;
}

std::string myRandom::generateRandomNumberString(size_t length) {
    constexpr char chars[] = "0123456789";
    constexpr size_t chars_len = sizeof(chars) - 1;
    static std::uniform_int_distribution<> distrib(0, chars_len - 1);

    std::string randomStr;
    randomStr.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        randomStr += chars[distrib(gen)];
    }
    return randomStr;
}

bool myRandom::getRandomBool() {
    static std::uniform_int_distribution<> distrib(0, 1);

    // Return true if random number is 1, else false
    return distrib(gen) == 1;
}

std::vector<std::string> getKeysFromJsonObject(const nlohmann::json& jsonObject) {
    std::vector<std::string> keys;

    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        if (!it.key().empty()) {
            keys.push_back(it.key());
        } else {
            std::cerr << "Warning: Found an invalid key in the JSON object." << std::endl;
        }
    }

    if (keys.empty()) {
        std::cerr << "Error: No keys available in JSON object." << std::endl;
        return {};
    }

    return keys;
}

std::vector<std::string> myRandom::getRandomVectorFromJSON(const nlohmann::json& jsonObject, const int& count) {
    std::vector<std::string> returnKeys;
    returnKeys.resize(count);

    if (jsonObject.empty()) {
        std::cerr << "Error: items JSON is empty!" << std::endl;
        return {};
    }

    if (!jsonObject.is_object()) {
        std::cerr << "Error: 'items' is not a valid JSON object!" << std::endl;
        return {};
    }

    // We only care about the keys of this JSON object.
    std::vector<std::string> keys = getKeysFromJsonObject(jsonObject);

    std::uniform_int_distribution<> distrib(0, keys.size() - 1);

    for (int i = 0; i < count; i++) {
        int randomIndex = distrib(gen);
        returnKeys[i] = keys[randomIndex];
    }

    return returnKeys; // Return a random key from the file
}
