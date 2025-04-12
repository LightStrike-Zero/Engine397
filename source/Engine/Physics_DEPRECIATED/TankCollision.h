//
// Created by Hugo on 4/12/2025.
//

#ifndef TANKCOLLISION_H
#define TANKCOLLISION_H
#include "Components/CollisionComponents/BoxColliderComponent.h"
#include "Components/CollisionComponents/SphereColliderComponent.h"
#include "Components/CollisionComponents/CapsuleColliderComponent.h"
#include "Components/TransformComponent.h"



bool checkBoxtoBoxCollision(const BoxColliderComponent& a, const TransformComponent& aTransform,
                            const BoxColliderComponent& b, const TransformComponent& bTransform);
// bool checkBoxtoSphereCollision(const BoxColliderComponent& a, const TransformComponent& aTransform,
//                                const SphereColliderComponent& b, const TransformComponent& bTransform);
// bool checkBoxtoCapsuleCollision(const BoxColliderComponent& a, const TransformComponent& aTransform,
//                                  const CapsuleColliderComponent& b, const TransformComponent& bTransform);
// bool checkSpheretoSphereCollision(const SphereColliderComponent& a, const TransformComponent& aTransform,
//                                    const SphereColliderComponent& b, const TransformComponent& bTransform);
// bool checkSpheretoCapsulreCollision(const SphereColliderComponent& a, const TransformComponent& aTransform,
//                                     const CapsuleColliderComponent& b, const TransformComponent& bTransform);
// bool checkCapsuletoCapsuleCollision(const CapsuleColliderComponent& a, const TransformComponent& aTransform,
//                                      const CapsuleColliderComponent& b, const TransformComponent& bTransform);



#endif //TANKCOLLISION_H
