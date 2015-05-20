#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/noise.hpp>
#include <unistd.h>

/* shader functions defined in shader.c */
extern void shaderAttachFromFile(GLuint, GLenum, const char *);

// Shader macro
#define GLSL(src) "#version 410 core\n" #src

// Vertex shader + vec4(sin(time)/1,cos(time)/1,0,1);
const GLchar* vertexShaderSrc = GLSL(
    in vec3 pos;
    in mat4 transformatrix;
    in float type;
    in float sides;
    in vec3 size;

    uniform float time;

    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;

    out int vType;
    out float vTime;
    out int vSides;
    out vec3 vSize;

    void main() {
        gl_Position = vec4(pos.x, pos.y, pos.z, 1.0); //projection * view * model * 
        vType = int(type);
        vSides = int(sides);
        vTime = time;
        vSize = size;
    }
);

// Geometry shader
const GLchar* geometryShaderSrc = GLSL(
    layout(points) in;
    layout(triangle_strip, max_vertices = 420) out;
  
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;
    uniform vec3 cameraPos;
    
    in vec3 vSize[];
    in int vType[];
    in float vTime[];
    in int vSides[];
    //in vec4 color[];

    //out vec4 gColor;
    vec4 aux = projection * view * model * gl_in[0].gl_Position;
    const float PI = 3.1415926;

   
    void makeCyl(float width, float l,float h, int sides){
                for (int i = 0; i <= sides; i++) {
                    float ang = ((PI * 2.0) / sides * i) + (PI / 4);

                    
                    //gColor = vec4(ang/15,ang/15,ang/15,1.0);
                    
                    vec4 offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, -h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    
                }
                EndPrimitive();

                for (int i = 0; i <= sides; i++) {
                    float ang = ((PI * 2.0) / sides * i) + (PI / 4);
                    if(i%2==(sides%2))
                        gl_Position = aux + projection * view * model * vec4(0.0f,0.0f,-h,0.0f);
                        EmitVertex();

                    //gColor = vec4(ang/7,ang/7,ang/7,1.0);
                    vec4 offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, -h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
                for (int i = 0; i <= sides; i++) {
                    float ang = ((PI * 2.0) / sides * i) + (PI / 4);
                    if(i%2==(sides%2))
                        gl_Position = aux + projection * view * model * vec4(0.0f,0.0f,h,0.0f);
                        EmitVertex();

                    //gColor = vec4(ang/12,ang/12,ang/12,1.0);
                    vec4 offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
    }

    void makeSphere(float size, int sides){
        for (int i = 0; i <= sides; i++) {
            float ang = (PI * 2.0) / sides * i;
            float ang1 = (PI * 2.0) / sides * (i+1);
            for (int j = 0; j <= sides; j++)
            {
                float ang2 = (PI * 2.0) / sides * j;
                float ang3 = (PI * 2.0) / sides * (j+1);
                //gColor = vec4(0.1,ang/10,0.1,1.0);
                
                vec4 offset = projection * view * model * vec4(sin(ang)*cos(ang2) * size, sin(ang)*sin(ang2) * size, cos(ang) * size, 0.0);
                gl_Position = aux + offset;
                EmitVertex();
                offset = projection * view * model * vec4(sin(ang1)*cos(ang2) * size, sin(ang1)*sin(ang2) * size, cos(ang1) * size, 0.0);
                gl_Position = aux + offset;
                EmitVertex();

            }
            EndPrimitive();
        }
        EndPrimitive();

    }

    int calcSides(int sides){
        float dist = distance(aux,vec4(cameraPos,1.0));
        if(dist > 1){
             return  int(min(32,max(4.0,sides/(dist*0.1))));
        }
        return sides;

    }

    bool notVisible(){
        return aux.x < -200.0 || aux.x > 200.0 || aux.y < -10.0 || aux.y > 1000.0;// || aux.z < -1.0 || aux.z > 10.0;
    }

    void main() {

        if(notVisible()){
//            EndPrimitive();
        }
        else{
            //gColor = vec4(0.1,0.67,0.3,1.0);
            switch (vType[0]){

                case 1:
                    makeCyl(vSize[0].x,vSize[0].y,vSize[0].z,4);            
                break;
                
                case 2:
                    makeSphere(vSize[0].x,16);
                break;

                case 3:
                    makeCyl(vSize[0].x,vSize[0].y,vSize[0].z,calcSides(int(vSides[0])));
                break;

                default:
                    gl_Position = aux + projection * view * model * vec4(0.1, -0.1, 0.0, 0.0);
                    EmitVertex();

                    gl_Position = aux + projection * view * model * vec4(-0.1, -0.1, 0.0, 0.0);
                    EmitVertex();

                    gl_Position = aux + projection * view * model * vec4(-0.1, 0.1, 0.0, 0.0);
                    EmitVertex();
                    EndPrimitive();
            }
        }

    }
);

// Geometry shader
const GLchar* geometryShaderSrc2 = GLSL(
    layout(points) in;
    layout(line_strip, max_vertices = 420) out;
  
  
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;
    uniform vec3 cameraPos;
    
    in vec3 vSize[];
    in int vType[];
    in float vTime[];
    in int vSides[];
    //in vec4 color[];

    out vec4 gColor;
    vec4 aux = projection * view * model * gl_in[0].gl_Position;
    const float PI = 3.1415926;

   
    void makeCyl(float width, float l,float h, int sides){
                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = ((PI * 2.0) / sides * i) + (PI / 4);
                    //float ang = (PI * 2.0) / vSides[0] * i;
                    //ang -= vTime[0];
                    //if(i%2==0)
                    //    gl_Position = aux * projection * view * model;
                    //    EmitVertex();
                    
                    gColor = vec4(ang/15,ang/15,ang/15,1.0);
                    
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, -h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    
                }
                EndPrimitive();

                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = ((PI * 2.0) / sides * i) + (PI / 4);
                    if(i%2==(sides%2))
                        gl_Position = aux + projection * view * model * vec4(0.0f,0.0f,-h,0.0f);
                        EmitVertex();

                    gColor = vec4(ang/7,ang/7,ang/7,1.0);
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, -h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = ((PI * 2.0) / sides * i) + (PI / 4);
                    if(i%2==(sides%2))
                        gl_Position = aux + projection * view * model * vec4(0.0f,0.0f,h,0.0f);
                        EmitVertex();

                    //gColor = vec4(ang/12,ang/12,ang/12,1.0);
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = projection * view * model * vec4(cos(ang) * width, -sin(ang) * l, h, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
    }

    void makeSphere(float size, int sides){
        // Safe, GLfloats can represent small integers exactly
        for (int i = 0; i <= sides; i++) {
            float ang = (PI * 2.0) / sides * i;
            float ang1 = (PI * 2.0) / sides * (i+1);
            // Angle between each side in radians
            for (int j = 0; j <= sides; j++)
            {
            float ang2 = (PI * 2.0) / sides * j;
            float ang3 = (PI * 2.0) / sides * (j+1);
            //gColor = vec4(0.1,ang/10,0.1,1.0);
            
            // Offset from center of point (0.3 to accomodate for aspect ratio)
            vec4 offset = projection * view * model * vec4(sin(ang)*cos(ang2) * 0.1, sin(ang)*sin(ang2) * 0.1, cos(ang) * 0.1, 0.0);
            gl_Position = aux + offset;
            EmitVertex();
            offset = projection * view * model * vec4(sin(ang1)*cos(ang2) * 0.1, sin(ang1)*sin(ang2) * 0.1, cos(ang1) * 0.1, 0.0);
            gl_Position = aux + offset;
            EmitVertex();

            }
            EndPrimitive();
        }
        EndPrimitive();

    }

    int calcSides(int sides){
        float dist = distance(aux,vec4(cameraPos,1.0));
        if(dist > 1){
             return  int(min(32,max(4.0,sides/(dist*0.1))));
        }
        return sides;

    }

    bool notVisible(){
        return aux.x < -200.0 || aux.x > 200.0 || aux.y < -10.0 || aux.y > 1000.0;// || aux.z < -1.0 || aux.z > 10.0;
    }

    void main() {

        if(notVisible()){
//            EndPrimitive();
        }
        else{
            //gColor = vec4(0.1,0.67,0.3,1.0);
            switch (vType[0]){

                case 1:
                    makeCyl(vSize[0].x,vSize[0].y,vSize[0].z,4);            
                break;
                
                case 2:
                    makeSphere(vSize[0].x,16);
                break;

                case 3:
                    makeCyl(vSize[0].x,vSize[0].y,vSize[0].z,calcSides(int(vSides[0])));
                break;

                default:
                    gl_Position = aux + projection * view * model * vec4(0.1, -0.1, 0.0, 0.0);
                    EmitVertex();

                    gl_Position = aux + projection * view * model * vec4(-0.1, -0.1, 0.0, 0.0);
                    EmitVertex();

                    gl_Position = aux + projection * view * model * vec4(-0.1, 0.1, 0.0, 0.0);
                    EmitVertex();
                    EndPrimitive();
            }
        }

    }
);

// Geometry shader
const GLchar* geometryShaderSrc3 = GLSL(
    layout(triangles) in;
    layout(triangle_strip, max_vertices = 420) out;
  
  
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;
    uniform vec3 cameraPos;
    
    in vec3 vSize[];
    in int vType[];
    in float vTime[];
    in int vSides[];
    //in vec4 color[];

    void main() {

        for (int n = 0; n < gl_in.length(); n+=2)
        {
            gl_Position =  projection * view * model * gl_in[n].gl_Position;
            EmitVertex();
        }
        EndPrimitive();

    }
);



// Fragment shader
const GLchar* fragmentShaderSrc = GLSL(
	in vec2 vPos;
    //in vec4 gColor;

    out vec4 outColor;

	float rand(vec2 co){
		return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
	}

    //vec4(gColor[0]); //
    void main() {
        outColor = vec4 (0.5, 0.5, 0.5, 1.0);
        //outColor = gColor;//vec4(rand(vPos), rand(vPos), rand(vPos), 1.0);
    }
);

// Tesselation Eval shader
const GLchar* tesselationEvalShaderSrc = GLSL(

    layout (triangles, equal_spacing, cw) in;
    //layout (vertices = 3) out;

    //out vec4 color;
    
    void main()
    {
    float u = gl_TessCoord.x;
    float omu = 1 - u; // one minus "u" 
    float v = gl_TessCoord.y;
    float omv = 1 - v; // one minus "v"
       //color = vec4(gl_TessCoord,1);
       gl_Position =
         omu * omv * gl_in[0].gl_Position +
         u * omv * gl_in[1].gl_Position +
         u * v * gl_in[2].gl_Position +
         omu * v * gl_in[3].gl_Position;
    }

);

// Tesselation Control shader
const GLchar* tesselationCtrlShaderSrc = GLSL(     
    layout(vertices = 3) out;
     
    void main(void)
    {
     gl_TessLevelOuter[0] = 2.0;
     gl_TessLevelOuter[1] = 4.0;
     gl_TessLevelOuter[2] = 6.0;
     //gl_TessLevelOuter[3] = 8.0;
     
     gl_TessLevelInner[0] = 8.0;
     gl_TessLevelInner[1] = 8.0;
     
     gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    }

);

void testError(GLchar* src){
    GLenum err = glGetError();
    //if (err != GL_NO_ERROR)
    //{
        printf("(%s) Error: %s\n",src, gluErrorString(err));
    //}
}

// Shader creation helper
GLuint createShader(GLenum type, const GLchar* src) {
    GLint length, result;
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
      char *log;

        /* get the shader info log */
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        log = (char*) malloc(length);
        glGetShaderInfoLog(shader, length, &result, log);

        /* print an error message and the info log */
        fprintf(stderr, "shaderCompileFromFile(): Unable to compile %s\n: %s\n", src,log);
        free(log);

        glDeleteShader(shader);
        return 1;
    }

    return shader;
}

#define VALUES_PER_POINT 8
int N_POINTS = 0;
int vecSize = 0;
GLfloat* points;


float* buildPoints(int n){
    float* ret = (float *)malloc(sizeof(float)*VALUES_PER_POINT*n);
    vecSize = n;
    for (int i = 0,j=0; i < n; ++i, j=0)
    {
            // pos-x, pos-y, type, n_sides, size-x, size-y
            ret[i*VALUES_PER_POINT+(j++)] = ((float) rand() / (RAND_MAX/10)) - 2.5f;
            ret[i*VALUES_PER_POINT+(j++)] = ((float) rand() / (RAND_MAX/10)) - 2.5f;
            ret[i*VALUES_PER_POINT+(j++)] = ((float) rand() / (RAND_MAX))/10;
            ret[i*VALUES_PER_POINT+(j++)] = (i % 3)+ 1;
            ret[i*VALUES_PER_POINT+(j++)] = ((float) (rand() % 40 ));
            ret[i*VALUES_PER_POINT+(j++)] = ((float) rand() / (RAND_MAX))/10+0.02f;
            ret[i*VALUES_PER_POINT+(j++)] = ((float) rand() / (RAND_MAX))/10+0.02f;
            ret[i*VALUES_PER_POINT+(j++)] = ((float) rand() / (RAND_MAX))/10+0.06f;
    }


    return ret;
}

float* buildPoint(int n,float pos_x,float pos_y,float pos_z,float type, float n_sides,float size_x,float size_y,float size_z){


    if(N_POINTS>=vecSize){
        points = (float *)realloc(points,sizeof(float)*VALUES_PER_POINT*vecSize*2);
        vecSize = 2*vecSize;
        printf("TEST %d  :  %d\n",vecSize,N_POINTS);
    }

    // pos-x, pos-y, type, n_sides, size-x, size-y
    points[n*VALUES_PER_POINT+0] = pos_x;
    points[n*VALUES_PER_POINT+1] = pos_y;
    points[n*VALUES_PER_POINT+2] = pos_z;
    points[n*VALUES_PER_POINT+3] = type;
    points[n*VALUES_PER_POINT+4] = n_sides;
    points[n*VALUES_PER_POINT+5] = size_x;
    points[n*VALUES_PER_POINT+6] = size_y;
    points[n*VALUES_PER_POINT+7] = size_z;

    return points;
}


extern "C" int init(int n){
    vecSize = n;
    points = (float *)calloc(sizeof(float),VALUES_PER_POINT*n);
    return 0;
}

extern "C" int createPoints(int in){
    N_POINTS = in;
    points = buildPoints(in);
    return 0;
}

extern "C" int box(float pos_x,float pos_y,float pos_z,float w,float l,float h){
    buildPoint(N_POINTS++,pos_x, pos_y,pos_z,1, 4,w, l,h);
    return 0;
}
extern "C" int cylinder(float pos_x,float pos_y,float pos_z,float r,float h){
    buildPoint(N_POINTS++,pos_x, pos_y,pos_z,3, 32,r, r,h);
    return 0;
}

extern "C" int sphere(float pos_x,float pos_y,float pos_z,float r){
    buildPoint(N_POINTS++,pos_x, pos_y,pos_z,2, 10,r, r,r);
    return 0;
}

float gaussiana2d(float x, float y,float sigma){
  return exp( -(pow( x/ sigma,2) + pow(y/sigma,2)) );
}

int NUMBER_OF_BUILDINGS = 0;

extern "C" void city(int size){
    float h,h2;
    for (int i = -size; i < size; ++i)
    {
        for (int j = -size; j < size; ++j)
        {   
            NUMBER_OF_BUILDINGS++;

            int end = rand()%4;
            for (int k = 0,ch=0; k < end; ++k, ch+=h)
            {
                
                h = 3*(0.4+fmax(gaussiana2d(i,j,25.2),gaussiana2d(i-30,j-30,10.2)));
                h *= std::abs(glm::simplex(glm::vec2(i / 16.f, j / 16.f)));
                //glm::perlin(glm::vec4(i / 16.f, j / 16.f, 0.5f, 0.5f));
                //std::abs(glm::vec4(glm::gaussRand(glm::vec3(0), glm::vec3(1)), 1).z);
                //h = fmax(h,4*(0.3+gaussiana2d(i-12,j+40,10.2)));
                if(h < 0.05)
                    break;
                /*if(k == 3 && ((float) rand() / (RAND_MAX))>0.5)
                    sphere(i,j,ch+h,0.5);
                else*/
                if(k > 1 && ((float) rand() / (RAND_MAX))>0.2)
                    cylinder(i,j,ch+h,0.4-(0.08*k),h);
                else
                    box(i, j, ch+h, 0.4-(0.08*k), 0.4-(0.1*k), h);

            }
        }

    }

}


// Camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);
glm::vec3 posLookAt   = glm::vec3(0.0f);
bool keys[1024];

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraPos += ((GLfloat)yoffset * 0.1f) * cameraUp;
}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}
double xpos_1, ypos_1,xpos_2, ypos_2, speed = 1;
GLenum pressed = GL_FALSE;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        pressed = GL_TRUE;
        glfwGetCursorPos(window, &xpos_1, &ypos_1);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        pressed = GL_FALSE;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
       posLookAt = glm::vec3(0.0f);
}

void do_movement(GLFWwindow* window)
{
    if (pressed){
        glfwGetCursorPos(window, &xpos_2, &ypos_2);
        posLookAt += glm::vec3((xpos_2-xpos_1)*speed,(ypos_2-ypos_1)*speed,0.0f);
        //glfwGetCursorPos(window, &xpos_1, &ypos_1);

    }
    // Camera controls
    GLfloat cameraSpeed = 0.05f;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * 10.0f;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * 10.0f;
    if (keys[GLFW_KEY_R])
        cameraPos += cameraSpeed * cameraUp;
    if (keys[GLFW_KEY_F])
        cameraPos -= cameraSpeed * cameraUp;
    cameraFront = -cameraPos + posLookAt;
}

int nbFrames = 0;
double lastTime = 0.0;
void showFPS()
{
     // Measure speed
     double currentTime = glfwGetTime();
     nbFrames++;
     if ( currentTime - lastTime >= 1.0 ){ // If last cout was more than 1 sec ago
         printf("%f\n", 1000.0/double(nbFrames));
         nbFrames = 0;
         lastTime += 1.0;
     }
}
    GLuint vbo;
float step = 0.99;
void change(){
    double currentTime = glfwGetTime();
    if ( currentTime - lastTime >= 7.0 ){ // If last cout was more than 1 sec ago
        if(step<1.0)
            step += 0.02;
        else
            step -= 0.02;
        lastTime += 7.0;
    }
    for (int i = 0; i < N_POINTS; ++i)
    {
            points[i*VALUES_PER_POINT+2] = step * points[i*VALUES_PER_POINT+2];
            points[i*VALUES_PER_POINT+VALUES_PER_POINT-1] = step * points[i*VALUES_PER_POINT+VALUES_PER_POINT-1];
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*VALUES_PER_POINT*N_POINTS, points, GL_STATIC_DRAW);

}

extern "C" int start() {
    // Window
    /*sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 4;
    settings.minorVersion = 1;

    sf::Window window(sf::VideoMode(1280, 800), "Shaders", sf::Style::Default, settings);
    settings = window.getSettings();
    printf("depth bits: %d\n", settings.depthBits);
    printf("stencil bits: %d\n", settings.stencilBits);
    printf("antialiasing level: %d\n", settings.antialiasingLevel);
    printf("version: %d.%d\n" ,settings.majorVersion ,settings.minorVersion);
     */
    const GLuint WIDTH = 1250, HEIGHT = 800;


    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GL_DEPTH_BITS, 49);
    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_STICKY_KEYS, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL",  NULL, NULL); // Windowed

    glfwMakeContextCurrent(window);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    

    // Initialize GLEW
    glewExperimental = GL_TRUE; 
    GLenum err = glewInit();
    printf("glew Error: %s\n", glewGetErrorString(err));
    printf("glu Error: %s\n", gluErrorString(err));
    testError("INIT ");

    
 //   glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                // Black Background
    glClearDepth(1.0f);                                    // Depth Buffer Setup
//    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
//   glEnable(GL_CULL_FACE);                                          // Enable face culling
//    glCullFace(GL_BACK);                                             // Cull back faces of polygons
//    glFrontFace(GL_CW);

//    glEnable(GL_MULTISAMPLE);




//    glfwSwapInterval(1);                                             // Swap buffers every frame (i.e. lock to VSync)
//    glfwSetInputMode(window, GLFW_CURSOR_DISABLED, GL_FALSE);        // Do not hide the mouse cursor
//    glfwSetWindowPos(window, 0, 0);                              // Push the top-left of the window out from the top-left corner of the screen
//    glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);     // Move the mouse cursor to the centre of the window

    //shaderAttachFromFile(shaderProgram, GL_VERTEX_SHADER, "shaders/shader.vp");
    //shaderAttachFromFile(g_program, GL_FRAGMENT_SHADER, "data/shader.fp");

    // Compile and activate shaders
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSrc);

    GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc);
    GLuint geometryShader3 = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc3);

    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    
    GLuint tessEvalShader = createShader(GL_TESS_EVALUATION_SHADER, tesselationEvalShaderSrc);
    GLuint tessCtrlShader = createShader(GL_TESS_CONTROL_SHADER, tesselationCtrlShaderSrc);

    GLuint shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    //glAttachShader(shaderProgram, geometryShader3);
    glAttachShader(shaderProgram, fragmentShader);
    //glAttachShader(shaderProgram, tessEvalShader);
    //glAttachShader(shaderProgram, tessCtrlShader);
    glLinkProgram(shaderProgram);
    
    testError("GOGO 000");

    GLuint geometryShader2 = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc2);

    GLuint shaderProgram2 = glCreateProgram();
    
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, geometryShader2);
    glAttachShader(shaderProgram2, fragmentShader);      
    glLinkProgram(shaderProgram2);





    // Create VBO with point coordinates

    glGenBuffers(1, &vbo);

    GLfloat a=0.45f, m_a=-0.45f;

    int sizeofFloat = sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeofFloat*VALUES_PER_POINT*N_POINTS, points, GL_STATIC_DRAW);

    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    testError("GOGO 111");

    // Specify layout of point data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");

    testError("GOGO 222");
    
    glEnableVertexAttribArray(posAttrib);
    
    testError("GOGO 333");

    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, VALUES_PER_POINT * sizeofFloat, 0);

    // Specify layout of point data
    posAttrib = glGetAttribLocation(shaderProgram, "type");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 1, GL_FLOAT, GL_FALSE, VALUES_PER_POINT * sizeofFloat, (void*) (3 * sizeofFloat));

    // Specify layout of point data
    posAttrib = glGetAttribLocation(shaderProgram, "sides");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 1, GL_FLOAT, GL_FALSE, VALUES_PER_POINT * sizeofFloat, (void*) (4 * sizeofFloat));

    // Specify layout of point data
    posAttrib = glGetAttribLocation(shaderProgram, "size");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, VALUES_PER_POINT * sizeofFloat, (void*) (5 * sizeofFloat));

    GLenum shader1 = GL_TRUE;

    GLint   timeLoc = glGetUniformLocation(shaderProgram, "time"); /* Uniform index for variable "time" in shader */
    GLfloat timeValue; /* Application time */

    /*glm::mat4 transformatrix = glm::rotate(glm::mat4(1.0f), 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    posAttrib = glGetAttribLocation(shaderProgram, "transformatrix");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeofFloat*VALUES_PER_POINT*N_POINTS, glm::value_ptr(transformatrix), GL_STATIC_DRAW);

    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(posAttrib+1);
    glEnableVertexAttribArray(posAttrib+2);
    glEnableVertexAttribArray(posAttrib+3);
    
    glVertexAttribPointer(posAttrib  , 4, GL_FLOAT, GL_FALSE, 0, (void*) (0 * sizeofFloat)) ;
    glVertexAttribPointer(posAttrib+1, 4, GL_FLOAT, GL_FALSE, 0, (void*) (4 * sizeofFloat)) ;
    glVertexAttribPointer(posAttrib+2, 4, GL_FLOAT, GL_FALSE, 0, (void*) (8 * sizeofFloat)) ;
    glVertexAttribPointer(posAttrib+3, 4, GL_FLOAT, GL_FALSE, 0, (void*) (12 * sizeofFloat));
    */


    // Create transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view;
    glm::mat4 projection;
    //model = glm::rotate(model, 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::rotate(model, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    //view = glm::translate(view, glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 300.0f);

    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    while(!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        do_movement(window);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            shader1 = !shader1;

        if(shader1)
            glUseProgram(shaderProgram);
        else
            glUseProgram(shaderProgram2);
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        view = glm::lookAt(cameraPos, posLookAt, cameraUp);
        //view = glm::lookAt(glm::vec3(camX, camY, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));//        model = glm::mat4(1.0f);
        //view = glm::mat4(1.0f);
        //projection = glm::mat4(1.0f);


        
        GLint camPos = glGetUniformLocation(shaderProgram, "cameraPos");
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
        glUniformMatrix4fv(camPos, 1, GL_FALSE, glm::value_ptr(cameraPos)); 
        
        a+=0.01;
        timeValue = a;
        
        glUniform1f(timeLoc, timeValue);
        

        //glPatchParameteri(GL_PATCH_VERTICES, 4);       // tell OpenGL that every patch has 16 verts
        //glDrawArrays(GL_PATCHES, 0, 5); 
        change();
        glDrawArrays(GL_POINTS, 0, N_POINTS);

        glfwSwapBuffers(window);
        //showFPS();
        //usleep(10);

    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwTerminate();

    free(points);

    return 0;
}

int CITY_SIZE = 10;
int main(){
    printf("A - S - W - D to move R - F or scrol for camera height\n");
    printf("Insert city radius (NUMBER_OF_BUILDINGS = (radius*2)^2):\n");
    scanf("%d",&CITY_SIZE);

    //createPoints(5000);
    init(CITY_SIZE*2*CITY_SIZE*2);
    city(CITY_SIZE);
    printf("NUMBER_OF_BUILDINGS=%d NUMBER_OF_BLOCKS=%d\n", NUMBER_OF_BUILDINGS, N_POINTS);

    start();
}
