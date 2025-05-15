Enemy = {
    state = "patrol",
    patrolPoints = {{x=0,y=0,z=0}, {x=5,y=0,z=5}},
    currentPatrolIndex = 1,
    attackRange = 2.0,
    chaseRange = 10.0
}

function Enemy:update(deltaTime)
    if self.state == "patrol" then
        self:patrolState(deltaTime)
    elseif self.state == "chase" then
        self:chaseState(deltaTime)
    elseif self.state == "attack" then
        self:attackState(deltaTime)
    end
end

function Enemy:patrolState(dt)
    local target = self.patrolPoints[self.currentPatrolIndex]
    self:moveTo(target)

    if self:atPosition(target) then
        self.currentPatrolIndex = (self.currentPatrolIndex % #self.patrolPoints) + 1
    end

    if self:seePlayer() then
        print("Enemy spotted player! Switching to chase.")
        self.state = "chase"
    end
end

function Enemy:chaseState(dt)
    local playerPos = self:getPlayerPosition()
    self:moveTo(playerPos)

    if self:distanceToPlayer() < self.attackRange then
        print("Player in range! Switching to attack.")
        self.state = "attack"
    elseif self:distanceToPlayer() > self.chaseRange then
        print("Player lost. Returning to patrol.")
        self.state = "patrol"
    end
end

function Enemy:attackState(dt)
    if self:distanceToPlayer() > self.attackRange then
        print("Player out of range. Chasing again.")
        self.state = "chase"
    else
        self:attackPlayer()
    end
end
