//
// Created by Hugo on 15/05/2025.
// Try to redesign the ScriptController to be more generic
//

#include "ScriptController.h"

void ScriptController::runScript(const std::string& scriptPath) {
    try {
        m_lua.script_file(scriptPath);
    } catch (const sol::error& e) {
        std::cerr << "Error: Lua File Error: " << e.what() << std::endl;
    }
}

glm::vec3 ScriptController::getVec3(const std::string& tableName) {
    sol::table t = m_lua[tableName];
    return {
        t["x"].get_or(0.0f),
        t["y"].get_or(0.0f),
        t["z"].get_or(0.0f)
    };
}

float ScriptController::getFloat(const std::string& name) {
    sol::object value = m_lua[name];
    if (value.valid() && value.is<float>()) {
        return value.as<float>();
    } else {
        std::cerr << "[ScriptController] Warning: '" << name << "' not found or not a float. Returning 0.\n";
        return 0.0f;
    }
}

std::string ScriptController::getString(const std::string& name) {
    sol::object value = m_lua[name];
    if (value.valid() && value.is<std::string>()) {
        return value.as<std::string>();
    } else {
        std::cerr << "[ScriptController] Warning: '" << name << "' not found or not a string. Returning empty string.\n";
        return "";
    }
}