-- State enum
State = {
    Chase = 1,
    Flee = 2
}

-- Per-enemy data (FSM state and health)
enemyData = {}

-- Helper: get normalized direction from A to B
function direction(from, to)
    local dx = to.x - from.x
    local dy = to.y - from.y
    local dz = to.z - from.z
    local length = math.sqrt(dx*dx + dy*dy + dz*dz)
    if length < 0.001 then
        return { x = 0, y = 0, z = 0 }
    end
    return { x = dx/length, y = dy/length, z = dz/length }
end

-- Helper: distance between two vec3
function distance(a, b)
    local dx = b.x - a.x
    local dy = b.y - a.y
    local dz = b.z - a.z
    return math.sqrt(dx*dx + dy*dy + dz*dz)
end

-- Initialization (run once)
function initEnemies()
    print("initEnemies called")
    print("enemies count: ", #enemies)
    for i = 1, #enemies do
        enemyData[i] = {
            state = State.Chase,
            health = 100.0
        }
    end
    print("enemyData count: ", #enemyData)
    print("enemyData[1].state: ", enemyData[1].state)
    print("enemyData[1].health: ", enemyData[1].health)
end


-- Main update loop called from C++
function Update(dt)

    --for i = 1, #enemies do
    --    local data = enemyData[i]
    --    print("data =", data)
    --    print("data.state =", data and data.state)
    --    print("State.Chase =", State.Chase)
    --
    --    if data.state == 1 then
    --        print("data.state is 1")
    --    end
    --
    --    if State.Chase == 1 then
    --        print("State.Chase is 1")
    --    end
    --
    --    if data.state == State.Chase then
    --        print("data.state == State.Chase -> CHASING")
    --    else
    --        print("data.state != State.Chase")
    --    end
    --end


    for i = 1, #enemies do
        local data = enemyData[i]
        print("enemyData[i].state = ", data.state, "State.Chase = ", State.Chase)
        print("Comparing data.state =", data.state, "vs State.Chase =", State.Chase)
        print("playerPos = ", playerPos)
        local dist = distance(enemies[i].position, playerPos)
        print("dist = ", dist)
        print("comparison result", data.state == State.Chase)

        if data.state == State.Chase then
            print("chasing")

            local dir = direction(enemies[i].position, playerPos)
            enemies[i].position.x = enemies[i].position.x + dir.x * 5 * dt
            enemies[i].position.y = enemies[i].position.y + dir.y * 5 * dt
            enemies[i].position.z = enemies[i].position.z + dir.z * 5 * dt
            --
            if dist < 10.0 then
                data.health = data.health - 10 * dt
            end

            if data.health < 50 then
                data.state = State.Flee
            end

        elseif data.state == State.Flee then
            print("fleeing")
            local dir = direction(playerPos, enemies[i].position)
            enemies[i].position.x = enemies[i].position.x + dir.x * 5 * dt
            enemies[i].position.y = enemies[i].position.y + dir.y * 5 * dt
            enemies[i].position.z = enemies[i].position.z + dir.z * 5 * dt

        end
    end
end