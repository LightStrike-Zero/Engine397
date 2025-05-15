//
// Created by Hugo on 15/05/2025.
//

#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H
#include <sol/sol.hpp>
#include <string>
#include <glm/vec3.hpp>


class ScriptController {
public:
    ScriptController() = default;

    virtual ~ScriptController() = default;

    void runScript(const std::string& scriptPath);
    glm::vec3 getVec3(const std::string& tableName) ;
    float getFloat(const std::string& name);
    std::string getString(const std::string& name);
    virtual void registerClass() = 0;



protected:
    sol::state m_lua;
};



#endif //SCRIPTCONTROLLER_H
