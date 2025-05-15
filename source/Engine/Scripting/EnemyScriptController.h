//
// Created by Hugo on 16/05/2025.
//

#ifndef ENEMYSCRIPTCONTROLLER_H
#define ENEMYSCRIPTCONTROLLER_H
#include "ScriptController.h"
#include "Systems/EnemyActionSystem.h"


class EnemyScriptController: public ScriptController {
public:
    void registerClass(EnemyActionSystem & enemyActionSystem) {;
        m_lua.new_usertype<EnemyActionSystem>(
            "EnemyActionSystem",
            sol::constructors<EnemyActionSystem()>()
            //register methods here
            //need to decide how much control we give to Lua
            //Parameters only?
            //Entire behaviour logic?
            //Anything in between?
        );
    }
};



#endif //ENEMYSCRIPTCONTROLLER_H
