#include <string>
#include <fstream>
#include "core.h"
#include "render_cpu_rt.h"
#include "render_gpu_rt.h"

#define GLEW_STATIC
#include "lib/glut/include/GL/glew.h"
#include "lib/glut/include/GL/freeglut.h"

render_gpu_rt *glut_hack;

void keyboard(unsigned char key, int x, int y)
{
  if (key == VK_ESCAPE)
    exit(0);
}

void render_gpu_rt::gen_screen_triangle(void)
{
  // build triangle
  float vertices[] = {
    -1.0f, 3.0f, 0.0f,
     -1.0f, -1.0f, 0.0f,
     3.0f,  -1.0f, 0.0f
  };

  // generate buffer on GPU for vertices
  glGenBuffers(1, &vertexBuffer);

  // make it active
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  // send data from CPU to this GPU buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // describe data we send to GPU

  glVertexAttribPointer(0, // it will be first element of vertex
    3,                     // it have 3 components
    GL_FLOAT,              // with float type
    GL_FALSE,              // we don`t need any normalisation of this data so false
    3 * sizeof(float),     // space to next vertex
    (void*)0);             // offset from beginning of pointer. we don`t have any
  glEnableVertexAttribArray(0); // and enable it. 

  glBindBuffer(GL_ARRAY_BUFFER, 0); // we done so disable this buffer
}

void render_gpu_rt::load_shader(void)
{
  const int number_of_shaders = 2;
  const char *shaders_name[number_of_shaders] = {"render_gpu_rt.vert", "render_gpu_rt.frag" };
  const int shaders_type[number_of_shaders] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  int shaders_id[number_of_shaders];
  // compile each shader
  for (int i = 0; i < number_of_shaders; i++)
  {
    std::ifstream shader_file(shaders_name[i]);
    std::string content((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
    char *content_buf = (char *)content.c_str();
    shaders_id[i] = glCreateShader(shaders_type[i]);
    glShaderSource(shaders_id[i], 1, &content_buf, NULL);
    glCompileShader(shaders_id[i]);

    // check errors
    int  success;
    char infoLog[512];
    glGetShaderiv(shaders_id[i], GL_COMPILE_STATUS, &success);

    if (!success)
    {
      glGetShaderInfoLog(shaders_id[i], 512, NULL, infoLog);
      printf("%s: ERROR %s\n",shaders_name[i], infoLog);
    }
  }
  
  // compile shader program
  // generate
  shaderProgramId = glCreateProgram();
  // Add each shader
  for (int i = 0; i < number_of_shaders; i++)
    glAttachShader(shaderProgramId, shaders_id[i]);

  // link
  glLinkProgram(shaderProgramId);
  int  success;
  char infoLog[512];
  glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
    printf("LINK ERROR %s\n", infoLog);
  }

  // delete garbage
  for (int i = 0; i < number_of_shaders; i++)
    glDeleteShader(shaders_id[i]);
}

void render_gpu_rt::send_objects(void)
{
  int uniform_WindowSize = glGetUniformLocation(shaderProgramId, "WindowSize");
  glUniform2f(uniform_WindowSize, W, H);

  int uniform_Objects_size = glGetUniformLocation(shaderProgramId, "Objects_size");
  glUniform1i(uniform_Objects_size, our_core->objects.size());

  int uniform_background = glGetUniformLocation(shaderProgramId, "backgroundColor");
  glUniform3f(uniform_background, backgroundColor.getX(), backgroundColor.getY(), backgroundColor.getZ());

  {
    low_level_object ll_objs[MAX_OBJECTS_SIZE];

    for (int i = 0; i < our_core->objects.size(); i++)
      ll_objs[i] = our_core->objects[i]->gen_low_level_object();
    
    int uniform_ObjectsBlock = glGetUniformBlockIndex(shaderProgramId, "Objects_Block");
    glUniformBlockBinding(shaderProgramId, uniform_ObjectsBlock, 0);
    // Uniform buffer object for lights
    GLuint uniform_buffer;
    glGenBuffers(1, &uniform_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(low_level_object) * MAX_OBJECTS_SIZE, ll_objs, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, uniform_ObjectsBlock, uniform_buffer, 0, sizeof(low_level_object) * MAX_OBJECTS_SIZE);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  /*

  for (int i = 0; i < our_core->objects.size(); i++)
  {
    low_level_object ll_objs[i] = our_core->objects[i]->gen_low_level_object();
    std::string pref = "Object[" + std::to_string(i) + "].";
    
    std::string type = pref + "type";
    int uniform_Objects_type = glGetUniformLocation(shaderProgramId, type.c_str());
    glUniform1i(uniform_Objects_type, ll_obj.type);

    std::string arr = pref + "arr";
    int uniform_Objects_arr = glGetUniformLocation(shaderProgramId, arr.c_str());
    glUniform1dv(uniform_Objects_type, 16, ll_obj.Arr);
  }*/
}

void display(void)
{
  glClearColor(backgroundColor.getX(), backgroundColor.getY(), backgroundColor.getZ(), 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(glut_hack->shaderProgramId);
  glut_hack->send_objects();
  glBindVertexArray(glut_hack->vertexBuffer);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  /*
  glColor3d(1,1,1);
  glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
  glEnd();
  */
  glutSwapBuffers();
}

void render_gpu_rt::init(void)
{
  int a = 0;
  glutInit(&a, NULL);  
  glutInitWindowSize(W, H);
  glutCreateWindow("Render on GPU");
  glut_hack = this;
  glutKeyboardFunc(&keyboard);
  glutDisplayFunc(&display);

  glewInit();
  gen_screen_triangle();
  load_shader();
}

void render_gpu_rt::draw(void)
{
  glutMainLoop();
}

void render_gpu_rt::term(void)
{
}