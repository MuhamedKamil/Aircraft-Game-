#include "Renderer.h"

#include <gtc/matrix_transform.hpp> 
#include <gtx/transform.hpp>
#include "OBJLoader/objloader.hpp"


Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Cleanup();
}

void Renderer::Initialize()
{


	//set camera pos and view point
	Camera = std::unique_ptr<ThirdCamera>(new ThirdCamera());

	avatar  =  std::unique_ptr<Avatar>(new Avatar(glm::vec3(0,-.2,0),glm::vec3(0,0,1)));
	Camera->UpdatePosition(avatar->position, avatar->orintation);

	//CreateSquare();
	RenderOrigin();

	myCamera = std::unique_ptr<EulerCamera>(new EulerCamera());
	myCamera->SetPerspectiveProjection(65.0f, 4.0f / 3.0f, 0.1f, 99999999.0f);

	myCamera->Reset(0.0, 0.0, 0.75,
		0, 0, 0,
		0, 1, 0);

	createSquare(right_sq);
	createSquare(left_sq);
	createSquare(up_sq);
	createSquare(down_sq);
	createSquare(front_sq);
	createSquare(back_sq);

	//createPlane(plane);//0000000000


	//plane_mod = glm::scale(1.0f, 1.0f, 1.0f)*glm::translate(0.0f, 0.0f, 3.0f);//00000000
	right_mod = glm::scale(5.0f, 5.0f, 5.0f) *glm::translate(1.0f, 0.0f, 0.0f)*glm::rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	left_mod = glm::scale(5.0f, 5.0f, 5.0f) *glm::translate(-1.0f, 0.0f, 0.0f)*glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	up_mod = glm::scale(5.0f, 5.0f, 5.0f) *glm::translate(0.0f, 1.0f, 0.0f)*glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	down_mod = glm::scale(5.0f, 5.0f, 5.0f) *glm::translate(0.0f, -1.0f, 0.0f)*glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));//

	front_mod = glm::scale(5.0f, 5.0f, 5.0f) *glm::translate(0.0f, 0.0f, -1.0f)*glm::rotate(00.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	back_mod = glm::scale(5.0f, 5.0f, 5.0f) *glm::translate(0.0f, 0.0f, 1.0f)*glm::rotate(00.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	//////////////////////////////////////////////////////////////////////////
	// loading and initalizing textures.
	//mTexture1 = std::unique_ptr<Texture>(new Texture("uvtemplate.bmp",0));
	//mTexture2 = std::unique_ptr<Texture>(new Texture("bricks.jpg",1));
	//////////////////////////////////////////////////////////////////////////

	mTexture1 = std::unique_ptr<Texture>(new Texture("right.jpg", 0));

	mTexture2 = std::unique_ptr<Texture>(new Texture("left.jpg", 1));
	mTexture3 = std::unique_ptr<Texture>(new Texture("top.jpg", 2));
	mTexture4 = std::unique_ptr<Texture>(new Texture("bottom.jpg", 3));
	mTexture5 = std::unique_ptr<Texture>(new Texture("front.jpg", 4));
	mTexture6 = std::unique_ptr<Texture>(new Texture("left.jpg", 5));

	//mTexture7 = std::unique_ptr<Texture>(new Texture("plane1.jpg", 6));



	initShader();


	initPointLight();

	initTransformation();
}


void Renderer::Update(double deltaTime)
{
	dt = deltaTime /  10;


	Camera->Look();
	//update the eye position uniform.
	glUniform3fv(EyePositionID,1, &Camera->mvPosition[0]);
}

void Renderer::Draw()
{		
	glClearColor(1.0,1.0,1.0,1.0f);
	shader.UseProgram();

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform

	if (first_third == 1)
	{
		glm::mat4 VP = myCamera->GetProjectionMatrix()*myCamera->GetViewMatrix();
		shader.BindVPMatrix(&VP[0][0]);
	}
	else
	{
		glm::mat4 VP = Camera->Proj * Camera->View;
		shader.BindVPMatrix(&VP[0][0]);
	}

	
	
	

	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &right_mod[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &left_mod[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &up_mod[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &front_mod[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &back_mod[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &down_mod[0][0]);


	

	//the floor
	//we need to send the model matrix to transform the normals too.
	//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &floorM[0][0]);
	//shader.BindModelMatrix(&floorM[0][0]);
	//mySquare->Draw();

	/*
	mTexture1 = std::unique_ptr<Texture>(new Texture("right.jpg", 0));

	mTexture2 = std::unique_ptr<Texture>(new Texture("left.jpg", 1));
	mTexture3 = std::unique_ptr<Texture>(new Texture("top.jpg", 2));
	mTexture4 = std::unique_ptr<Texture>(new Texture("bottom.jpg", 3));
	mTexture5 = std::unique_ptr<Texture>(new Texture("front.jpg", 4));
	mTexture6 = std::unique_ptr<Texture>(new Texture("back.jpg", 5));
	*/

	mTexture1->Bind();
	//Origin Axis
	shader.BindModelMatrix(&right_mod[0][0]);
	right_sq->Draw();
	//OriginAxis->DrawAxis();
	mTexture2->Bind();
	shader.BindModelMatrix(&left_mod[0][0]);
	left_sq->Draw();

	mTexture3->Bind();
	shader.BindModelMatrix(&up_mod[0][0]);
	up_sq->Draw();

	mTexture4->Bind();
	shader.BindModelMatrix(&down_mod[0][0]);
	down_sq->Draw();

	mTexture5->Bind();
	shader.BindModelMatrix(&front_mod[0][0]);
	front_sq->Draw();


	mTexture6->Bind();
	shader.BindModelMatrix(&back_mod[0][0]);
	back_sq->Draw();


	glUseProgram(programID);

	//send the rendering mode to the shader.
	mRenderingMode = RenderingMode::TEXTURE_ONLY;
	glUniform1i(mRenderingModeID, mRenderingMode);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	



	avatar->Draw(shader);


}

void Renderer::Cleanup()
{
	glDeleteProgram(programID);
}



void Renderer::HandleKeyboardInput(int key)
{
	const float kRotateSpeed = 0.5f;
	const float kMoveSpeed = 0.5f;

	float fRotation = 0.0f;
	glm::vec3 vMovement = glm::vec3(0,0,0);
	glm::vec3 vDirection = avatar->orintation;	
	float	kAvatarMovement = 1.0f;
	float	kAvatarRotation = 0.5f;

	switch (key)
	{
		//Moving forward
	case GLFW_KEY_DOWN:
		vMovement += vDirection * (kAvatarMovement/8);
		break;

		//Moving backword
		
	case GLFW_KEY_UP:
			vMovement -= vDirection * (kAvatarMovement/8);
		break;

		// Moving right
	case GLFW_KEY_RIGHT:
		fRotation -= kAvatarRotation;
		break;

		// Moving left
	case GLFW_KEY_LEFT:
		fRotation += kAvatarRotation;
		break;

		// Moving up
	case GLFW_KEY_I:
		Camera->mfDistance -= kMoveSpeed;
		Camera->UpdatePosition(avatar->position, avatar->orintation);
		break;

	case GLFW_KEY_K:
		Camera->mfDistance += kMoveSpeed;
		Camera->UpdatePosition(avatar->position, avatar->orintation);
		break;
	case GLFW_KEY_J:
		Camera->mfAzimuth += kRotateSpeed;
		Camera->UpdatePosition(avatar->position, avatar->orintation);
		break;
	case GLFW_KEY_L:
		Camera->mfAzimuth -= kRotateSpeed;
		Camera->UpdatePosition(avatar->position, avatar->orintation);
		break;
	case GLFW_KEY_KP_MULTIPLY:
		sRotation -= kAvatarRotation;
		break;
	case GLFW_KEY_SLASH:
		sRotation += kAvatarRotation;
		break;

	case GLFW_KEY_1:
		first_third = 1;
		break;
	case GLFW_KEY_3:
		first_third = 3;
		break;
	case GLFW_KEY_SPACE:
		vMovement.y += 0.1f;
		break;
	case GLFW_KEY_N:
		vMovement.y = -0.1f;
		break;
	default:
		break;
	}


		// Rotate the avatar if necessary.
	if (fRotation != 0.0f)
	{
		// Rotate the Orientation Vector of the avatar to face the correct direction.
		avatar->RotateVectorArbitrary(avatar->orintation , glm::vec3(0,0,0), fRotation, glm::vec3(0,1,0));
		avatar->orintation = glm::normalize(avatar->orintation);

		avatar->UpdateMatrix();

		Camera->UpdatePosition(avatar->position, avatar->orintation);

		
	}
	if (sRotation != 0.0f)
	{
		// Rotate the Orientation Vector of the avatar to face the correct direction.
		avatar->RotateVectorArbitrary(avatar->orintation, glm::vec3(0, 0, 0), sRotation, glm::vec3(0, 1, 1));
		avatar->orintation = glm::normalize(avatar->orintation);

		avatar->UpdateMatrix();

		Camera->UpdatePosition(avatar->position, avatar->orintation);
		sRotation = 0;

	}

	// Move the avatar if necessary.
	if (vMovement != glm::vec3(0,0,0))
	{
		//vMovement.y = 0.0f;		// Restrict movement to XZ plane.
		avatar->position += vMovement;
		avatar->position = checkCollsion(avatar->position);
		avatar->UpdateMatrix();

		Camera->UpdatePosition(avatar->position, avatar->orintation);

		
	}

}

void Renderer::HandleMouse(double deltaX,double deltaY)
{	
	myCamera->Yaw(deltaX);
	myCamera->Pitch(deltaY);
	myCamera->UpdateViewMatrix();
	//update the eye position uniform.
	glUniform3fv(EyePositionID,1, &myCamera->GetEyePosition()[0]);
}


void Renderer::initTransformation()
{
	//send the eye position to the shaders.
	glUniform3fv(EyePositionID,1, &Camera->mvPosition[0]);
	//////////////////////////////////////////////////////////////////////////

}

void Renderer::initShader()
{
	// Create and compile our GLSL program from the shaders
	//programID = LoadShaders( "SimpleTransformWithColor.vertexshader", "MultiColor.fragmentshader" );
	shader.LoadProgram();

	MatrixID = glGetUniformLocation(shader.programID, "MVP");
	ModelMatrixID = glGetUniformLocation(shader.programID, "ModelMatrix");

	// Use our shader
	//glUseProgram(programID);
	shader.UseProgram();
}

void Renderer::initPointLight()
{

	//////////////////////////////////////////////////////////////////////////
	// Configure the light.

	//setup the light position.
	LightPositionID = glGetUniformLocation(shader.programID,"LightPosition_worldspace");

	lightPosition = glm::vec3(0.5,0.5,0.5);

	glUniform3fv(LightPositionID,1, &lightPosition[0]);

	//setup the ambient light component.
	AmbientLightID = glGetUniformLocation(shader.programID,"ambientLight");
	ambientLight = glm::vec3(0.5,0.5,0.5);
	glUniform3fv(AmbientLightID,1, &ambientLight[0]);

	//setup the eye position.
	EyePositionID = glGetUniformLocation(shader.programID,"EyePosition_worldspace");
	//////////////////////////////////////////////////////////////////////////

}



void Renderer::initPointLightm()
{

	//////////////////////////////////////////////////////////////////////////
	// Configure the light.

	//setup the light position.
	LightPositionID = glGetUniformLocation(shader.programID, "LightPosition_worldspace");

	lightPosition = glm::vec3(0.5, 0.7, 0.5);
	glUniform3fv(LightPositionID, 1, &lightPosition[0]);

	//setup the ambient light component.
	AmbientLightID = glGetUniformLocation(shader.programID, "ambientLight");
	ambientLight = glm::vec3(0.1, 0.1, 0.1);
	glUniform3fv(AmbientLightID, 1, &ambientLight[0]);

	//setup the eye position.
	EyePositionID = glGetUniformLocation(shader.programID, "EyePosition_worldspace");
	//////////////////////////////////////////////////////////////////////////

}

void Renderer::createSquare(std::unique_ptr<Model> &Square)
{

	//drawing a square.
	Square = std::unique_ptr<Model>(new Model());

	Square->VertexData.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	Square->VertexData.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	Square->VertexData.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	Square->VertexData.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));

	Square->ColorData.push_back(glm::vec3(1.0f, 0.0f, 0.5f));
	Square->ColorData.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
	Square->ColorData.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
	Square->ColorData.push_back(glm::vec3(1.0f, 0.0f, 0.5f));

	Square->UVData.push_back(glm::vec2(0.0f, 0.0f));
	Square->UVData.push_back(glm::vec2(1.0f, 0.0f));
	Square->UVData.push_back(glm::vec2(1.0f, 1.0f));
	Square->UVData.push_back(glm::vec2(0.0f, 1.0f));
	//NormalsData
	Square->NormalsData.push_back(glm::vec3(1.0f, 0.0f, 0.1f));
	Square->NormalsData.push_back(glm::vec3(1.0f, 0.0f, 0.1f));
	Square->NormalsData.push_back(glm::vec3(1.0f, 0.0f, 0.1f));
	Square->NormalsData.push_back(glm::vec3(1.0f, 0.0f, 0.1f));

	//first triangle.
	Square->IndicesData.push_back(0);
	Square->IndicesData.push_back(1);
	Square->IndicesData.push_back(3);

	//second triangle.
	Square->IndicesData.push_back(1);
	Square->IndicesData.push_back(2);
	Square->IndicesData.push_back(3);

	Square->Initialize();

}




void Renderer::RenderOrigin()
{

	//drawing a square.
	OriginAxis = std::unique_ptr<Model>(new Model());

	OriginAxis->VertexData.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	OriginAxis->VertexData.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	OriginAxis->VertexData.push_back(glm::vec3( 0.0f,  1.0f, 0.0f));
	OriginAxis->VertexData.push_back(glm::vec3( 0.0f,  0.0f, 1.0f));


	OriginAxis->ColorData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

	//first line.
	OriginAxis->IndicesData.push_back(0);
	OriginAxis->IndicesData.push_back(1);

	//second line.
	OriginAxis->IndicesData.push_back(0);
	OriginAxis->IndicesData.push_back(2);

	//third line.
	OriginAxis->IndicesData.push_back(0);
	OriginAxis->IndicesData.push_back(3);

	OriginAxis->Initialize();

	AxisMat = glm::scale(200,100,200);
}
glm::vec3 Renderer::checkCollsion(glm::vec3 Position)
{
	float x = 3;
	if (Position.x >= x)
	{
		Position.x = x - 0.1;
	}
	if (Position.x <= -x)
	{
		Position.x = -x + 0.1;
	}
	if (Position.y >= x)
	{
		Position.y = x - 0.1;
	}
	if (Position.y <= -.25)
	{
		Position.y = -.25;
	}
	if (Position.z >= x)
	{
		Position.z = x - 0.1;
	}
	if (Position.z <= -5)
	{
		Position.z = -5 + 0.1;
	}
	return Position;
}