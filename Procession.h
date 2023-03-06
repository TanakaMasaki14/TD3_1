#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

Matrix4 Mat_Identity();

Matrix4 Mat_Scale(Vector3& scale);

Matrix4 Mat_Rotation(Vector3& rotation);

Matrix4 Mat_Translation(Vector3& translation);

Matrix4 MatWorld(Vector3& scale, Vector3& rotation, Vector3& translation);

Vector3 Mat_Velocity(Vector3 velocity, Matrix4 matWorld);

float Length(Vector3 vector);

Vector3 Vectornorm(Vector3 vec1, Vector3 vec2);

Vector3 Normalize(Vector3& vector);