//
// Created by xtrit on 21/10/17.
//

#include "Scene.hpp"


using namespace OVR;
using namespace std;

const float GRID_X = 0.0080f;
const float GRID_Y = 0.0032f;
const float GRID_Z = 0.0080f;

const float HALF_GRID_X = GRID_X / 2;
const float HALF_GRID_Y = GRID_Y / 2;
const float HALF_GRID_Z = GRID_Z / 2;


const vector<Vector3f> colors = {
        Vector3f(1.0f, 1.0f, 1.0f),
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
};

void engine::Scene::rotate(Quatf rotation)
{
    Quatf orientation = rotation * controllingObject.orientation;
    if(!orientation.IsNormalized()) {
        orientation.Normalize();
    }

    controllingObject.orientation = orientation;
}


void engine::Scene::moveUnit(Vector3i translation) {
    move({
         GRID_X * translation.x,
         GRID_Y * translation.y,
         GRID_Z * translation.z
    });
}

float calculateMovement(float& value, float delta, float halfGrid, float grid) {
    value += delta;
    if(value > 0) {
        if(value <= halfGrid) {
            return 0.0f;
        } else {
            value = value - grid;
            return grid;
        }
    } else {
        if(value >= -halfGrid) {
            return 0.0f;
        } else {
            value = value + grid;
            return -grid;
        }
    }
}
void engine::Scene::move(Vector3f translation) {
    Vector3f movement;
    movement.x = calculateMovement(pseudoPosition.x, translation.x, HALF_GRID_X, GRID_X);
    movement.y = calculateMovement(pseudoPosition.y, translation.y, HALF_GRID_Y, GRID_Y);
    movement.z = calculateMovement(pseudoPosition.z, translation.z, HALF_GRID_Z, GRID_Z);

    controllingObject.position += movement;
}



void engine::Scene::placeBlock() {
    objects.push_back(controllingObject);
}

engine::Scene::~Scene()
{

}
