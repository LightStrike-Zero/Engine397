Chase(target, deltaTime)
Patrol(area/path, deltaTime)
Attack(target, deltaTime)
Evade(target, deltaTime)
Defeated()

--behavior parameters
PatrolRadius = 100
--patrol waypoint setup
PatrolSpeed = 10
ChaseSpeed = 15
AlarmRadius = 100


-- Generic NPC: patrol, detect/alert, chase, attack, defend, flee, support teammates.

local transform, mover, combat, sensor, health
local state = "Patrol"
local patrolPoints = { Vector3(5,0,5), Vector3(-5,0,5), Vector3(-5,0,-5), Vector3(5,0,-5) }
local targetIndex  = 1
local target       = nil

function OnInit(entity)
transform = entity:GetComponent("Transform")
mover     = entity:GetComponent("Mover")
combat    = entity:GetComponent("Combat")   -- primary/secondary attacks
sensor    = entity:GetComponent("Sensor")   -- vision/hearing
health    = entity:GetComponent("Health")
state     = "Patrol"
end

function OnUpdate(entity, dt)

    -- 1) Perception: look for player or alerts
    local intruder = sensor:Detect("Player")
    if intruder then
        target = intruder
        state  = "Chase"
        -- let nearby allies know
        Game.SendEvent("NPCAlerted", { sender=entity, pos=transform:GetPosition() })
    end

    -- 2) State machine
    if state == "Patrol" then
        Patrol(dt)

    elseif state == "Chase" then
        Chase(dt)

    elseif state == "Attack" then
        Attack(dt)
    end
end



    function Patrol(dt)
        mover:MoveToward(patrolPoints[targetIndex], dt)
        if transform:DistanceTo(patrolPoints[targetIndex]) < 1 then
            targetIndex = (targetIndex % #patrolPoints) + 1
        end
    end

    function Chase(dt)
        if not target or target:IsDestroyed() then
            state = "Patrol"; target = nil; return
        end
        local pos = target:GetComponent("Transform"):GetPosition()
        mover:MoveToward(pos, dt)

        local dist = transform:DistanceTo(pos)
        if dist <= combat.Range then
            state = "Attack"
        elseif dist > sensor.MaxRange then
            state = "Patrol"; target = nil
        end
    end

    function Attack(dt)
        if not target or target:IsDestroyed() then
            state = "Patrol"; target = nil; return
        end
        -- face and fire
        local pos = target:GetComponent("Transform"):GetPosition()
        transform:LookAt(pos)

        if combat:CanUsePrimary() then
            combat:PerformPrimary()
            Audio.Play("AttackSfx")
        end


        -- disengage if too far
        if transform:DistanceTo(pos) > combat.Range * 1.5 then
            state = "Chase"
        end
    end
