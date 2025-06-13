
#pragma once

#include "Vertex4D.h"

class Matrix
{
public:

	
	Matrix();
	
	Matrix(const Matrix& other);
	
	~Matrix();
	// GET VALUE IN MATRIX
	float GetM(const int row, const int col) const;
	// SET VALUE IN MATRIX
	void SetM(const int row, const int col, const float val);
	
	Matrix& operator= (const Matrix &rhs);
	// MULTYPLY MATRICES
	const Matrix operator*(const Matrix &other) const;
	// MULTYPLY MATRIX AND VERTEX
	const Vertex operator*(const Vertex &other) const;

	// SET AND COPY VALUES
	void				SetZero();
	void				SetIdentity();
    void				SetMatrix(const Matrix& other);

	// SETTING VALUES FOR TRANSFORMING AND VIEWING MATRICES
	void				SetRotationX(float degrees);
	void				SetRotationY(float degrees);
	void				SetRotationZ(float degrees);
	void				SetTranslate(float tx, float ty, float tz);
	void				SetScale(float sx, float sy, float sz);
    
	void				SetViewMatrix(const Vertex& cameraPosition, const Vertex& viewVector, const Vertex& upvector);
	void				SetCamera(float xRotation, float yRotation, float zRotation, float xPosition, float yPosition, float zPosition);
	void				SetOrtho(float d);
	void				SetPerspective(float d, float aspectRatio);
	void                SetViewvolume(float left, float right, float bot, float top, float front, float rear);
	void				SetViewport(int left, int right, int top, int bot);
	

private:
	float				Mat[4][4];
};
