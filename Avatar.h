#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp> 
#include <gtx/transform.hpp>
#include <gtc\quaternion.hpp>
#include <gtx\quaternion.hpp>
#include <gtx\rotate_vector.hpp>


class Avatar
{
public:

	glm::vec3 position;
	glm::vec3 orintation;

	std::unique_ptr<Model3D> myCar;
	glm::mat4 CarMat;


	Avatar(glm::vec3 avatarPosition , glm::vec3 avatarOrintation)
	{
		position = avatarPosition;
		orintation = avatarOrintation;

		myCar = std::unique_ptr<Model3D>(new Model3D());
		myCar->LoadFromFile("data/models/Jeep/jeep1.3ds",false);
		myCar->Initialize();

		CarMat = glm::translate(position) * glm::scale(0.0f,0.0f,0.0f);
		CarMat *= glm::orientation(orintation,glm::vec3(0,0,1));
		//CarMat *= glm::rotate(-90.0f, glm::vec3(1,0,0));
	}

	void UpdateMatrix()
	{
		CarMat = glm::translate(position) * glm::scale(0.012f,0.012f,0.012f) ;
		CarMat *= glm::orientation(orintation,glm::vec3(0,0,1));
		//CarMat *= glm::rotate(-90.0f, glm::vec3(1,0,0));
	}
	void Draw( ShaderProgram shader)
	{
		myCar->Render(&shader,CarMat);
	}

	void RotateVectorArbitrary(glm::vec3 &Orintation ,  glm::vec3 vCenter, float fAngle, glm::vec3 vAxis )
	{	

		glm::vec3 vNewPosition;
		glm::vec3 vTemp = glm::vec3( Orintation.x,Orintation.y, Orintation.z );

		glm::vec3 vPos = vTemp - vCenter;

		float fSinTheta, fCosTheta;
		fSinTheta = sinf( fAngle );
		fCosTheta = cosf( fAngle );

		float fX = vAxis.x, fY = vAxis.y, fZ = vAxis.z;

		vNewPosition.x  = ( fCosTheta + ( 1 - fCosTheta ) * fX * fX )		* vPos.x;
		vNewPosition.x += ( ( 1 - fCosTheta ) * fX * fY - fZ * fSinTheta )	* vPos.y;
		vNewPosition.x += ( ( 1 - fCosTheta ) * fX * fZ + fY * fSinTheta )	* vPos.z;

		vNewPosition.y  = ( ( 1 - fCosTheta ) * fX * fY + fZ * fSinTheta )	* vPos.x;
		vNewPosition.y += ( fCosTheta + ( 1 - fCosTheta ) * fY * fY )		* vPos.y;
		vNewPosition.y += ( ( 1 - fCosTheta ) * fY * fZ - fX * fSinTheta )	* vPos.z;

		vNewPosition.z  = ( ( 1 - fCosTheta ) * fX * fZ - fY * fSinTheta )	* vPos.x;
		vNewPosition.z += ( ( 1 - fCosTheta ) * fY * fZ + fX * fSinTheta )	* vPos.y;
		vNewPosition.z += ( fCosTheta + ( 1 - fCosTheta ) * fZ * fZ )		* vPos.z;

		Orintation.x = vCenter.x + vNewPosition.x;
		Orintation.y = vCenter.y + vNewPosition.y;
		Orintation.z = vCenter.z + vNewPosition.z;
	}



};