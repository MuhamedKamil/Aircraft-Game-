#include "Application Manager/ApplicationManager.h"
#include<mmsystem.h>
#include<thread>
void play()
{
	//PlaySound(TEXT("aircraft003.wav"), NULL, SND_SYNC);
}
int main(void)
{
	//Specify OpenGL version as 3.3
	int openGLMajorVersion = 3;
	int openGLMinorVersion = 3;
	//502PlaySound(TEXT("aircraft003.wav"), NULL, SND_SYNC);
	//std::thread th(play);
	//th.detach();
	play();
	//Unique object for the application manager
	std::unique_ptr<ApplicationManager> appManager (new ApplicationManager(openGLMajorVersion,openGLMinorVersion));
	
	// Initialize the window providing its width and height
	if(appManager->InitalizeApplication(800,600) == true)
	{
		appManager->StartMainLoop();
		appManager->CloseApplication();
	}
}