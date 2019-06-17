#ifndef Renderer_h__
#define Renderer_h__

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <memory>


#include "Shaders/shader.hpp"
#include "Model/Model.h"
#include "EulerCamera/EulerCamera.h"
#include "Texture/texture.h"
#include "GraphicsDevice/ShaderProgram.h"
#include "Scene/Model3D.h"
#include "ThirdCamera.h"
#include "Avatar.h"

class Renderer
{
	float sRotation = 0.0f;

	GLuint programID;
	ShaderProgram shader;

	// Transformation
	GLuint MatrixID;
	GLuint ModelMatrixID;

	GLuint AmbientLightID;
	GLuint LightPositionID;
	GLuint EyePositionID;


	glm::vec3 ambientLight;
	glm::vec3 lightPosition;


	std::unique_ptr<Model> mySquare;
	glm::mat4 floorM;

	std::unique_ptr<Model> OriginAxis;
	glm::mat4 AxisMat;


	//std::unique_ptr<EulerCamera> myCamera;
	std::unique_ptr<ThirdCamera> Camera;



	std::unique_ptr<Avatar> avatar;


	int first_third = 1;

	float dt;
	///////////////////////////////////////////////////////////////////////
	enum RenderingMode
	{
		NO_TEXTURE,
		TEXTURE_ONLY,
		BLEND
	};


	std::unique_ptr<Model> myTriangle;

	std::unique_ptr<Model> right_sq;
	std::unique_ptr<Model> left_sq;
	std::unique_ptr<Model> front_sq;
	std::unique_ptr<Model> back_sq;
	std::unique_ptr<Model> up_sq;
	std::unique_ptr<Model> down_sq;

	std::unique_ptr<Model> plane;

	glm::mat4 plane_mod;

	std::unique_ptr<EulerCamera> myCamera;

	glm::mat4 right_mod;
	glm::mat4 left_mod;
	glm::mat4 front_mod;
	glm::mat4 back_mod;
	glm::mat4 up_mod;
	glm::mat4 down_mod;


	std::unique_ptr<Texture> mTexture1;
	std::unique_ptr<Texture> mTexture2;
	std::unique_ptr<Texture> mTexture3;
	std::unique_ptr<Texture> mTexture4;
	std::unique_ptr<Texture> mTexture5;
	std::unique_ptr<Texture> mTexture6;

	//std::unique_ptr<Texture> mTexture7;//000000000

	GLuint mRenderingModeID;
	RenderingMode mRenderingMode;


	std::unique_ptr<Model> Square;
	glm::mat4 squareM;





public:
	Renderer();
	~Renderer();

	void Initialize();
	void Draw();
	void HandleKeyboardInput(int key);
	void HandleMouse(double deltaX,double deltaY);
	void Update(double deltaTime);
	void Cleanup();
	void initPointLightm();

	void initTransformation();
	void initShader();
	void initPointLight();

	void createSquare(std::unique_ptr<Model> &Square);
	void RenderOrigin();

	glm::vec3 checkCollsion(glm::vec3);

};

#endif // Renderer_h__

