#include <OpenGLExample.h>

#include <vector>
#include <random>

//Glew
//#define GLEW_STATIC
#include <GL/glew.h>

#if defined (WIN32)
#include <GL/wglew.h>
#endif

//#pragma comment(lib, "glew32.lib")

//GLFW
#include <GLFW/glfw3.h>

int OpenGLGraphicsEngine::GLFWCharCallback(GLFWwindow* pWindow, unsigned int pChar)
{
	return 0;
}


OpenGLGraphicsEngine::OpenGLGraphicsEngine()
{
	auto err = glGetError();

	if (!glfwInit())
		return;

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	this->mainWindow = glfwCreateWindow(512, 512, "Hello", nullptr, nullptr);

	glfwSetWindowUserPointer(this->mainWindow, this);

	// Set the keyboard callback.
	//glfwSetCharCallback(this->mainWindow, &GLFWCharCallback);

	// Make the context current.
	glfwMakeContextCurrent(this->mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return;

	glfwSwapInterval(0);

	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(1);

	this->SetupEngine();
}

struct Point
{
	float x, y, vx, vy;

	Point(float x, float y)
	{
		this->x = x;
		this->y = y;
		this->vx = 0;
		this->vy = 0;
	}
};

void OpenGLGraphicsEngine::SetupEngine()
{
	std::mt19937 random;
	std::uniform_real_distribution<float> uint_dist10(0, 50);
	std::uniform_real_distribution<float> uintd1(0, 255);

	std::vector<Point> points;
	
	for (int i = 0; i < pointCount; i++)
	{
		points.push_back(
			Point(
			uint_dist10(random),
			uint_dist10(random)));
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * points.size(), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point) * points.size(), &points[0].x);

	GLuint vertexAttributeobject = 0;
	glGenVertexArrays(1, &vertexAttributeobject);
	glBindVertexArray(vertexAttributeobject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vertexAttributeobject);
}

void OpenGLGraphicsEngine::Render()
{
	glViewport(0, 0, 512, 512);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 50, 0, 50, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1, 1, 1, 0.05f);
	glPointSize(5.0);
	
	glDrawArrays(GL_POINTS, 0, pointCount);
	
	glFlush();

	glFinish();
	
	glfwSwapBuffers(this->mainWindow);
}

void OpenGLGraphicsEngine::Update()
{
	glfwPollEvents();
}

OpenGLGraphicsEngine::~OpenGLGraphicsEngine()
{
	glfwDestroyWindow(this->mainWindow);
	glfwTerminate();
}