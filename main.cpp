// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Object.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void init();
void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void special(int, int, int);
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y);
void MyMouseMove(GLint X, GLint Y);
glm::vec3 GetOGLPos(int x, int y);

GLuint		    program;
GLint			loc_a_vertex;
GLint			loc_u_pvm_matrix;
GLint			loc_u_mv_matrix;
GLint			loc_u_normal_matrix;
GLint           loc_a_normal;
GLint           loc_u_material_ambient;
GLint           loc_u_material_diffuse;
GLint           loc_u_material_specular;
GLint           loc_u_material_shininess;
GLint           loc_a_color;
GLint			loc_u_mouse_click;
GLint			loc_u_mouse_points;
GLint           light_position_handle;
GLint           light_ambient_handle;
GLint           light_diffuse_handle;
GLint           light_specular_handle;
GLint			brush_size;

GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY; //마우스의 좌표값을 저장하는 변수

glm::mat4 mat_PVM_Desk;
glm::mat4 mat_PVM_Fan;
glm::mat4 mat_PVM_Sofa;
glm::mat4 mat_PVM_TV;
glm::mat4 mat_Rotation_Desk;
glm::mat4 mat_Translation_model;
glm::mat4 mat_PVM, mat_VM, m;
glm::mat4   mat_Proj, mat_View, mat_Model;
glm::mat3 normal_matrix;
glm::vec3 mousePoints;

float light_position[4] = {30.0f, 30.0f, 30.0f, 1.0f};
float light_ambient[4] = {0.1f, 0.1f, 0.1f, 0.5f};
float light_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float light_specular[4] = {0.3f, 0.3f, 0.3f, 0.3f};
float rotateValue = 0.0f;
float deltaTime = 0.0f;
float timeSinceStart = 0.0f;
float oldTimeSinceStart = 0.0f;
float rotate_fan_value = 0;
float light_x = 0.0f, light_y = 0.0f, light_z = 0.0f;
float loc_brush_size=0.05f;
static float fan_speed = 0.005f;

bool identify = true;
bool click = true;
bool leftClick = false;

std::string g_filename;
Object		  g_model;      // object
Camera		  g_camera;			// viewer (you)


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);  
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);  
  glutCreateWindow("Physical light converting");
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMouseFunc(MyMouseClick);
  glutMotionFunc(MyMouseMove);

  glutPostRedisplay();
  if (glewInit() != GLEW_OK) 
  {
     	std::cerr << "failed to initialize glew" << std::endl;
     	return -1;
  }
  	
  init();
  
  glutMainLoop();

  return 0;
}


void settingObject()
{
	
}
void init()
{
  // g_model.load_simple_obj(g_filename);

  g_model.load_simple_obj("./makeCam.obj");
  //g_model.print();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for filled polygon rendering  
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  // TODO: 1. Implement vertex/fragment shader codes for Phong shading model
  program = Shader::create_program("./simple.vert", "./simple.frag");
	
  // TODO: 2. Get locations of shader variables in shader program
	loc_u_pvm_matrix	= glGetUniformLocation(program, "mvp_matrix");
	loc_u_mv_matrix = glGetUniformLocation(program, "mv_matrix");
	loc_u_normal_matrix = glGetUniformLocation(program, "normal_matrix");
	
	loc_a_vertex			= glGetAttribLocation(program, "a_vertex");
	loc_a_normal                 = glGetAttribLocation(program, "a_normal");
	
	loc_u_material_ambient = glGetUniformLocation(program, "material_ambient");
	loc_u_material_diffuse=glGetUniformLocation(program, "material_diffuse");
	loc_u_material_specular=glGetUniformLocation(program, "material_specular");
	loc_u_material_shininess=glGetUniformLocation(program, "material_shininess");
	
	light_position_handle=glGetUniformLocation(program,"light_position");
	light_ambient_handle=glGetUniformLocation(program,"light_ambient");
	light_diffuse_handle=glGetUniformLocation(program,"light_diffuse");
	light_specular_handle=glGetUniformLocation(program,"light_specular");


	loc_u_mouse_click = glGetUniformLocation(program, "mouse_click");
	loc_u_mouse_points = glGetUniformLocation(program, "mouse_points");

	loc_a_color = glGetAttribLocation(program, "a_color");
	brush_size = glGetUniformLocation(program, "brush_size");


}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	// Camera setting
	// camera extrinsic param
	mat_View = glm::lookAt(
		g_camera.position(),				// eye position
		g_camera.center_position(), // center position
		g_camera.up_direction()			// up direction 
		);

	// camera intrinsic param
	mat_Proj  = glm::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);  	
	

  mat_Translation_model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 100, -100));
  mat_Rotation_Desk = glm::rotate(glm::mat4(1.0f), 3.1f + rotateValue, glm::vec3(0.0f, 0.0f, 1.0f));
//  mat_Rotation_Desk = glm::rotate(glm::mat4(1.0f), 3.1f + rotateValue, glm::vec3(0.0f, 0.0f, 1.0f));

  mat_PVM = mat_Proj*mat_View*mat_Translation_model*mat_Rotation_Desk;
  
  mat_VM = mat_View*mat_Rotation_Desk;
  float light_pos[3] = { 2.0f + light_x,25.0f + light_y,55.0f + light_z };

  m= mat_Rotation_Desk;

  normal_matrix[0][0]=m[0][0];
  normal_matrix[0][1]=m[0][1]; 
  normal_matrix[0][2]=m[0][2];
  normal_matrix[1][0]=m[1][0]; 
  normal_matrix[1][1]=m[1][1];
  normal_matrix[1][2]=m[1][2];
  normal_matrix[2][0]=m[2][0];
  normal_matrix[2][1]=m[2][1]; 
  normal_matrix[2][2]=m[2][2];

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix4fv(loc_u_mv_matrix, 1, false,  glm::value_ptr(mat_VM));
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(normal_matrix));

  glUniform1f(brush_size, loc_brush_size);
  glUniform3f(light_position_handle, light_pos[0], light_pos[1], light_pos[2]);
  glUniform4f(light_ambient_handle,  light_ambient[0], light_ambient[1], light_ambient[2], light_ambient[3]);
  glUniform4f(light_diffuse_handle,  light_diffuse[0], light_diffuse[1], light_diffuse[2], light_diffuse[3] );
  glUniform4f(light_specular_handle, light_specular[0], light_specular[1], light_specular[2], light_specular[3]);


  glUniform3f(loc_u_mouse_points, mousePoints.x, mousePoints.y, mousePoints.z);
  //std::cout << mousePoints.x<< " "<<mousePoints.y<< " " <<mousePoints.z << std::endl;
  if(leftClick==true) glUniform1f(loc_u_mouse_click,10.0f );
  else glUniform1f(loc_u_mouse_click, 0.0f);


  // TODO: 3. Extend Object::draw function to pass variables to the shader program
  
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  g_model.draw(loc_a_vertex, loc_a_normal, loc_u_material_ambient,
  loc_u_material_diffuse, loc_u_material_specular, loc_u_material_shininess,leftClick,mousePoints, loc_a_color, mat_PVM, loc_brush_size);

  glUseProgram(0);

  Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	if ('a' == key || 'A' == key)
	{
		g_camera.rotate_right(0.1);
	}
	else if ('d' == key || 'D' == key)
	{
		g_camera.rotate_left(0.1);
	}
	else if ('r' == key)
	{
		rotateValue = rotateValue + 0.1f;
	}
	else if ('j' == key || 'J' == key)
	{
		light_x += 1.0f;
		// std::cout<<light_x<<" "<<light_y<<" "<<light_z<<std::endl;
	}
	else if ('l' == key || 'L' == key)
	{
		light_x -= 1.0f;
		// std::cout<<light_x<<" "<<light_y<<" "<<light_z<<std::endl;
	}
	else if ('i' == key || 'I' == key)
	{
		light_y += 1.0f;
		// std::cout<<light_x<<" "<<light_y<<" "<<light_z<<std::endl;
	}
	else if ('k' == key || 'K' == key)
	{
		light_y -= 1.0f;
		// std::cout<<light_x<<" "<<light_y<<" "<<light_z<<std::endl;
	}
	else if ('u' == key || 'U' == key)
	{
		light_z += 1.0f;
		// std::cout<<light_x<<" "<<light_y<<" "<<light_z<<std::endl;
	}
	else if ('p' == key || 'P' == key)
	{
		light_z -= 1.0f;
		// std::cout<<light_x<<" "<<light_y<<" "<<light_z<<std::endl;
	}
	else if ('=' == key || '+' == key)
	{
		loc_brush_size += 0.01f;
	}
	else if ('-' == key || '_'==key)
	{
		loc_brush_size -= 0.01f;
	}

  glutPostRedisplay();
}

void special(int key, int x, int y)
{	
	switch (key)
	{
	case GLUT_KEY_UP:
		g_camera.move_forward(10.0);
		break;
	case GLUT_KEY_DOWN:
		g_camera.move_backward(10.0);
		break;
	case GLUT_KEY_LEFT:
		g_camera.move_left(10.0);
		break;
	case GLUT_KEY_RIGHT:
		g_camera.move_right(10.0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}



void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y)
{
	GLfloat SelectX = X / 640.0;
	GLfloat SelectY = (640 - Y) / 640.0;

	if (Button == 0) { //leftClick

		leftClick = true;
		mousePoints = GetOGLPos(X, Y);
		
	}
	else
	{
		leftClick = false;
		if (State == GLUT_UP) {
			if (Button == 3) //whill up
			{
				g_camera.move_forward(10.0f);
				identify = true;
				glutPostRedisplay();
			}


			else if (Button == 4) { //whill down
				g_camera.move_backward(10.0f);
				identify = true;
				glutPostRedisplay();
			}
		}
		else
		{
			if (Button == GLUT_RIGHT_BUTTON && State == GLUT_DOWN)
			{
				TopLeftX = X;
				TopLeftY = Y;
			}
			if (Button == GLUT_RIGHT_BUTTON && State == GLUT_UP)     //마우스를 떼고 다시 다른곳에 누를때 
			{
				identify = true;
			}
		}
	}

}

void MyMouseMove(GLint X, GLint Y)
{
	BottomRightX = X;
	BottomRightY = Y;      //마우스의 현 좌표를 bottom x, y에 저장
	if (identify == true)
	{
		TopLeftX = BottomRightX;  //처음 마우스를 눌렀을 때 top x, y에 현 좌표를 저장.
		TopLeftY = BottomRightY;

	}


	if (identify == false && leftClick == false)
	{
		g_camera.rotate_left((TopLeftX*0.002f - X*0.002f));
		g_camera.rotate_up((TopLeftY*0.002f - Y*0.002f));

	}

	identify = false; //마우스를 누르고 있는 시점
	click = false;

	mousePoints = GetOGLPos(X, Y);
	glutPostRedisplay();
	TopLeftX = BottomRightX;
	TopLeftY = BottomRightY;

}

glm::vec3 GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return glm::vec3(posX, posY, posZ);
}
