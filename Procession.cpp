#include"Procession.h"

Matrix4 Mat_Identity()
{
	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}

Matrix4 Mat_Scale(Vector3& scale)
{
	// スケーリング行列を宣言
	Matrix4 matScale;

	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 Mat_Rotation(Vector3& rotation)
{
	// 合成用回転行列を宣言
	Matrix4 matRot = Mat_Identity();

	// X,Y,Z軸回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	// X軸回転行列
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	matRotX.m[3][3] = 1;

	// Y軸回転行列z
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	matRotY.m[3][3] = 1;

	// 各軸の回転行列を合成
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;
}

Matrix4 Mat_Translation(Vector3& translation)
{
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	matTrans.m[3][3] = 1;

	return matTrans;
}

Matrix4 MatWorld(Vector3& scale, Vector3& rotation, Vector3& translation)
{
	Matrix4 procession;

	procession = Mat_Scale(scale);
	procession *= Mat_Rotation(rotation);
	procession *= Mat_Translation(translation);

	return procession;
}

Vector3 Mat_Velocity(Vector3 velocity, Matrix4 matWorld)
{
	Vector3 matvelocity;
	matvelocity.x += matWorld.m[0][0] * velocity.x;
	matvelocity.x += matWorld.m[1][0] * velocity.y;
	matvelocity.x += matWorld.m[2][0] * velocity.z;

	matvelocity.y += matWorld.m[0][1] * velocity.x;
	matvelocity.y += matWorld.m[1][1] * velocity.y;
	matvelocity.y += matWorld.m[2][1] * velocity.z;

	matvelocity.z += matWorld.m[0][2] * velocity.x;
	matvelocity.z += matWorld.m[1][2] * velocity.y;
	matvelocity.z += matWorld.m[2][2] * velocity.z;

	return matvelocity;
}

float Length(Vector3 vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector3 Vectornorm(Vector3 vec1, Vector3 vec2)
{
	Vector3 vecPos = vec1 -= vec2;

	return vecPos;
}

Vector3 Normalize(Vector3& vector)
{
	float len = Length(vector);
	if (len != 0)
	{
		vector /= len;
	}
	return vector;
}