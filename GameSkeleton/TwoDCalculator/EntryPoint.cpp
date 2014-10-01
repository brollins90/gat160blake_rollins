#include "RenderUI.h"
#include "Engine.h"
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

glm::vec2 leftVector, rightVector, resultVector;

void myBasicVectorEquationCallback(const BasicVectorEquationInfo& data)
{
	leftVector.x = (data.scalar1 * data.x1);
	rightVector.x = (data.scalar2 * data.x2);
	leftVector.y = (data.scalar1 * data.y1);
	rightVector.y = (data.scalar2 * data.y2);
	resultVector = (data.add) ? leftVector + rightVector : leftVector - rightVector;
}

glm::vec2 orignalVector, normalVector, cwPerpendicularVector, ccwPerpendicularVector;

void myPerpendicularDataCallback(const PerpendicularData& data)
{
	orignalVector.x = data.x;
	orignalVector.y = data.y;
	normalVector = glm::normalize(orignalVector);
	cwPerpendicularVector = glm::vec2(orignalVector.y, (-1) * orignalVector.x);
	ccwPerpendicularVector = glm::vec2((-1) * orignalVector.y, orignalVector.x);
}

//glm::vec2 leP, leN, leD;
//
//void myLineEquationDataCallback(const LineEquationData& data)
//{
//	// TODO
//	leD.x = data.d;
//	leN.x = data.n_i;
//	leN.y = data.n_j;
//	leP.x = data.p_x;
//}

glm::vec2 vector1, vector2, projectionVector, rejectionVector;

void myDotProductDataCallback(const DotProductData& data)
{
	vector1.x = data.v1i;
	vector1.y = data.v1j;
	vector2.x = data.v2i;
	vector2.y = data.v2j;
	
	glm::vec2& left = (data.projectOntoLeftVector) ? vector1 : vector2;
	glm::vec2& right = (data.projectOntoLeftVector) ? vector2 : vector1;

	projectionVector = (glm::normalize(left) * ((glm::dot(left, right)) / glm::length(left)));
	rejectionVector = right - projectionVector;
}

glm::vec2 aVector, bVector, aMinusBVector, aVectorLerpPortion, bVectorLerpPortion, lerpResultVector;

void myLerpDataCallback(const LerpData& data)
{
	aVector.x = data.a_i;
	aVector.y = data.a_j;
	bVector.x = data.b_i;
	bVector.y = data.b_j;
	aMinusBVector = bVector - aVector;
	aVectorLerpPortion = aVector * (1.0F - data.beta);
	bVectorLerpPortion = bVector * data.beta;

	lerpResultVector = aVectorLerpPortion + bVectorLerpPortion;
}

glm::vec2 resultVector2, vBasic;
glm::mat2 mBasic;
void myLinearTransformationCallback(const LinearTransformationData& data)
{
	mBasic = glm::mat2(
		data.m00, data.m10,
		data.m01, data.m11
		);
	//mBasic[0].x = data.m00; // a  | a  b |
	//mBasic[0].y = data.m01; // b  | c  d |
	//mBasic[1].x = data.m10; // c
	//mBasic[1].y = data.m11; // d
	vBasic.x = data.v0;
	vBasic.y = data.v1;

	resultVector2 = mBasic * vBasic;
}

glm::vec3 resultVectors[5];
glm::mat3 mAffine;

void myAffineTransformationCallback(const AffineTransformationData& data)
{
	mAffine = glm::mat3(
		data.data[0], data.data[3], data.data[6],
		data.data[1], data.data[4], data.data[7],
		data.data[2], data.data[5], data.data[8]
		);

	for (int i = 0; i < 5; i++) 
	{
		resultVectors[i] = mAffine * glm::vec3(data.data[9 + (i * 3)], data.data[10 + (i * 3)], data.data[11 + (i * 3)]);
	}

}

const float span = .3f;

glm::vec2 bottomLeft(-span, -span);
glm::vec2 topLeft(-span,span);
glm::vec2 topRight(span,span);
glm::vec2 bottomRight(span,-span);
glm::vec2 roofTop(0, span + .25);

glm::vec2 linesMatrixTransform[] = {
	bottomLeft, topLeft,
	topLeft, topRight,
	topRight, bottomRight,
	bottomRight, bottomLeft,
	topLeft, roofTop,
	topRight, roofTop
};

int numLinesMatrixTransform = sizeof(linesMatrixTransform) / (sizeof(*linesMatrixTransform) * 2);

glm::mat3 matrices[20], currentTransform;
int numMatrices = 1;

void myMatrixTransformCallback2D(const MatrixTransformData2D& data)
{
	numMatrices = (data.selectedMatrix > (numMatrices - 1)) ? data.selectedMatrix + 1 : numMatrices;
	
	glm::mat4 tempMatrix = glm::mat4();

	tempMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(data.scaleX, data.scaleY, 1));
	//temp1 = temp1 * temp1.ScaleX(data.scaleX);
	//temp1 = temp1 * temp1.ScaleY(data.scaleY);
	
	tempMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(data.translateX, data.translateY, 0));
	//temp1 = temp1 * temp1.Translation(data.translateX, data.translateY);
	
	tempMatrix *= glm::rotate(glm::mat4(1.0f), glm::degrees(data.rotate), glm::vec3(0,0,1));
	//temp1 = temp1 * temp1.Rotation(data.rotate);

	
	//glm::mat4 tempMatrix = glm::mat4(1.0);
	//glm::mat4 tempMatrix = rotateMatrix * translateMatrix * scaleMatrix;

	matrices[data.selectedMatrix] = glm::mat3(tempMatrix);

	//glm::mat3 tempMatrix3 = glm::mat3(
	//	tempMatrix[0].x, tempMatrix[0].y, tempMatrix[0].z,
	//	tempMatrix[1].x, tempMatrix[1].y, tempMatrix[1].z,
	//	tempMatrix[2].x, tempMatrix[2].y, tempMatrix[2].z);
	//matrices[data.selectedMatrix] = tempMatrix3;

	currentTransform = glm::mat3();

	for (int i = 0; i < numMatrices; i++ )
	{
		currentTransform = currentTransform * matrices[i];
	}
}


glm::mat4 matrices3D[20], fullTransform;
int num3DMatrices = 1;

void myMatrixTransformCallback3D(const MatrixTransformData3D& data)
{
	num3DMatrices = (data.selectedMatrix > (num3DMatrices - 1)) ? data.selectedMatrix + 1 : num3DMatrices;
	
	glm::mat4 tempMatrix = glm::mat4();

	tempMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(data.scaleX, data.scaleY, data.scaleZ));
	tempMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(data.translateX, data.translateY, data.translateZ));
	tempMatrix *= glm::rotate(glm::mat4(1.0f), glm::degrees(data.rotateX), glm::vec3(1,0,0));
	tempMatrix *= glm::rotate(glm::mat4(1.0f), glm::degrees(data.rotateY), glm::vec3(0,1,0));
	tempMatrix *= glm::rotate(glm::mat4(1.0f), glm::degrees(data.rotateZ), glm::vec3(0,0,1));
	matrices3D[data.selectedMatrix] = tempMatrix;

	// Add them all up
	fullTransform = glm::mat4(1.0f);
	for (int i = 0; i < num3DMatrices; i++ )
	{
		fullTransform = fullTransform * matrices3D[i];
	}
}


int main(int argc, char* argv[])
{
	//Engine::Init();

	RenderUI renderUI;

	// Vector Basics
	renderUI.setBasicVectorEquationData(myBasicVectorEquationCallback,
		(float*)&leftVector,
		(float*)&rightVector,
		(float*)&resultVector);

	// Perpendiculars
	renderUI.setPerpendicularData((float*)&orignalVector,
		(float*)&normalVector,
		(float*)&cwPerpendicularVector,
		(float*)&ccwPerpendicularVector,
		myPerpendicularDataCallback);

	//// Line Equation
	//renderUI.setLineEquationData((float*)&leP,
	//	(float*)&leN,
	//	(float*)&leD,
	//	myLineEquationDataCallback);

	// Dot Product
	renderUI.setDotProductData((float*)&vector1,
		(float*)&vector2,
		(float*)&projectionVector,
		(float*)&rejectionVector,
		myDotProductDataCallback);

	// LERP
	renderUI.setLerpData((float*)&aVector,
		(float*)&bVector,
		(float*)&aMinusBVector,
		(float*)&aVectorLerpPortion,
		(float*)&bVectorLerpPortion,
		(float*)&lerpResultVector,
		myLerpDataCallback);

	// Basic Matrix Multiply
	renderUI.setLinearTransformationData((float*)&resultVector2,
		myLinearTransformationCallback);

	// Affine Transformations
	renderUI.setAffineTransformationData((float*)&resultVectors,
		myAffineTransformationCallback);

	// Matrix Transformations
	renderUI.set2DMatrixVerticesTransformData((float*)&linesMatrixTransform,
		numLinesMatrixTransform,
		(float*)&matrices,
		(float*)&currentTransform,
		myMatrixTransformCallback2D);
	
	// 3D Matrix stuff
	renderUI.set3DMatrixCallback((float*)&matrices3D,
		(float*)&fullTransform,
		myMatrixTransformCallback3D);
	

	if( ! renderUI.initialize(argc, argv))
		return -1;
	return renderUI.run();
}