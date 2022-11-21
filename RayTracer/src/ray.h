#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Ray {
public:
    void create(const glm::vec3& origin, const glm::vec3& direction) {
        this->origin = origin;
        this->direction = direction;
    };
    glm::vec3 getOrigin() { return origin; };
    glm::vec3 getDirection() { return direction; };

private:
    glm::vec3 origin;
    glm::vec3 direction;
};
