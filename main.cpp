#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/* shader functions defined in shader.c */
extern void shaderAttachFromFile(GLuint, GLenum, const char *);

// Shader macro
#define GLSL(src) "#version 410 core\n" #src

// Vertex shader + vec4(sin(time)/1,cos(time)/1,0,1);
const GLchar* vertexShaderSrc = GLSL(
    in vec2 pos;
    in float type;
    in float sides;
    in vec2 size;

    uniform float time;

    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;

    out int vType;
    out float vTime;
    out int vSides;
    out vec2 vSize;

    void main() {
        gl_Position = vec4(pos, 0.0, 1.0); //model * view * projection * 
        vType = int(type);
        vSides = int(sides);
        vTime = time;
        vSize = size;
    }
);

// Geometry shader
const GLchar* geometryShaderSrc = GLSL(
    layout(points) in;
    layout(triangle_strip, max_vertices = 265) out;
  
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;

    
    in vec2 vSize[];
    in int vType[];
    in float vTime[];
    in int vSides[];
    //in vec4 color[];

    out vec4 gColor;
    vec4 aux = gl_in[0].gl_Position;
    const float PI = 3.1415926;

    void makeCube(float size_x,float size_y,float size_z){
                aux =  gl_in[0].gl_Position;// - vec4(0.0,0.0,0.5,0.0);

                gColor = vec4(0.5,0.2,size_x,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 1
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 2
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 4
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 1
                EmitVertex();

                EndPrimitive();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 3
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 1
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 4
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 3
                EmitVertex();

                EndPrimitive();

                gColor = vec4(1,1,1,1.0);

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 8
                EmitVertex();

                EndPrimitive();

                gColor = vec4(0.4,0.4,1,1.0);

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();

                gColor = vec4(0.6,0.3,0.5,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();


                gColor = vec4(0.1,0.1,0.1,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();


                gColor = vec4(0.1,0.4,0.1,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();
    }

    void makeCyl(float width, float height, int sides){
                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = (PI * 2.0) / sides * i;
                    //float ang = (PI * 2.0) / vSides[0] * i;
                    //ang -= vTime[0];
                    //if(i%2==0)
                    //    gl_Position = aux * model * view * projection;
                    //    EmitVertex();
                    
                    gColor = vec4(0.1,ang/10,0.1,1.0);
                    
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = model * view * projection * vec4(cos(ang) * width, -height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    offset = model * view * projection * vec4(cos(ang) * width, height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    
                }
                EndPrimitive();

                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = (PI * 2.0) / sides * i;
                    if(i%2==(sides%2))
                        gl_Position = aux + model * view * projection * vec4(0.0f,-height,0.0f,0.0f);
                        EmitVertex();

                    gColor = vec4(ang/7,ang/10,0.1,1.0);
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = model * view * projection * vec4(cos(ang) * width, -height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = (PI * 2.0) / sides * i;
                    if(i%2==(sides%2))
                        gl_Position = aux + model * view * projection * vec4(0.0f,height,0.0f,0.0f);
                        EmitVertex();

                    gColor = vec4(ang/7,0.1,ang/10,1.0);
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = model * view * projection * vec4(cos(ang) * width, height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
    }

    void makeSphere(float size, float sides){
        // Safe, GLfloats can represent small integers exactly
        for (int i = 0; i <= sides; i++) {
            float ang = (PI * 2.0) / sides * i;
            // Angle between each side in radians
            for (int j = 0; j <= sides; j++)
            {
            float ang2 = (PI * 2.0) / sides * j;
            float ang3 = (PI * 2.0) / sides * (j+1);
            gColor = vec4(0.1,ang/10,0.1,1.0);
            
            // Offset from center of point (0.3 to accomodate for aspect ratio)
            vec4 offset = model * view * projection * vec4(sin(ang)*cos(ang2) * 0.1, sin(ang)*sin(ang2) * 0.1, cos(ang) * 0.1, 0.0);
            gl_Position = aux + offset;
            EmitVertex();
//            offset = model * view * projection * vec4(sin(ang)*cos(ang2) * 0.1, sin(ang)*sin(ang2) * 0.1, cos(ang) * 0.1, 0.0);
//            gl_Position = aux + offset;
//            EmitVertex();

            }
            EndPrimitive();
        }
        EndPrimitive();



    }

    void main() {
            gColor = vec4(0.1,0.67,0.3,1.0);
        switch (vType[0]){

            case 1:
            case 2:
                makeCube(vSize[0].x,vSize[0].y,0.1f);
            
            //break;

            
            //    makeSphere(vSize[0].x,10);
            break;

            case 3:
                makeCyl(vSize[0].x,vSize[0].y,int(vSides[0]));
            break;

            default:
                gl_Position = aux + model * view * projection * vec4(0.1, -0.1, 0.0, 0.0);
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-0.1, -0.1, 0.0, 0.0);
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-0.1, 0.1, 0.0, 0.0);
                EmitVertex();
                EndPrimitive();
        }

    }
);

// Geometry shader
const GLchar* geometryShaderSrc2 = GLSL(
    layout(points) in;
    layout(line_strip, max_vertices = 265) out;
  
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;

    
    in vec2 vSize[];
    in int vType[];
    in float vTime[];
    in int vSides[];
    //in vec4 color[];

    out vec4 gColor;
    vec4 aux = gl_in[0].gl_Position;
    const float PI = 3.1415926;

    void makeCube(float size_x,float size_y,float size_z){
                aux =  gl_in[0].gl_Position;// - vec4(0.0,0.0,0.5,0.0);

                gColor = vec4(0.5,0.2,size_x,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 1
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 2
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 4
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 1
                EmitVertex();

                EndPrimitive();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 3
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 1
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 4
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 3
                EmitVertex();

                EndPrimitive();

                gColor = vec4(1,1,1,1.0);

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0);      //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0);    //* vec4(sin(vTime[0]),cos(vTime[0]),0,1) // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0);     //* vec4(cos(vTime[0]),sin(vTime[0]),0,1) // 8
                EmitVertex();

                EndPrimitive();

                gColor = vec4(0.4,0.4,1,1.0);

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();

                gColor = vec4(0.6,0.3,0.5,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();


                gColor = vec4(0.1,0.1,0.1,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();


                gColor = vec4(0.1,0.4,0.1,1.0);

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, size_z, 0.0) ; //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 5
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //*vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0) ; //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();
                
                EndPrimitive();
                
                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, -size_z, 0.0); //*  vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 6
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-size_x, -size_y, -size_z, 0.0) ; //* vec4(sin(vTime[0]),cos(vTime[0]),0,1);    // 7 
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(size_x, -size_y, size_z, 0.0); //* vec4(cos(vTime[0]),sin(vTime[0]),0,1);    // 8
                EmitVertex();

                EndPrimitive();
    }

    void makeCyl(float width, float height, int sides){
                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = (PI * 2.0) / sides * i;
                    //float ang = (PI * 2.0) / vSides[0] * i;
                    //ang -= vTime[0];
                    //if(i%2==0)
                    //    gl_Position = aux * model * view * projection;
                    //    EmitVertex();
                    
                    gColor = vec4(0.1,ang/10,0.1,1.0);
                    
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = model * view * projection * vec4(cos(ang) * width, -height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    offset = model * view * projection * vec4(cos(ang) * width, height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                    
                }
                EndPrimitive();

                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = (PI * 2.0) / sides * i;
                    if(i%2==(sides%2))
                        gl_Position = aux + model * view * projection * vec4(0.0f,-height,0.0f,0.0f);
                        EmitVertex();

                    gColor = vec4(ang/7,ang/10,0.1,1.0);
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = model * view * projection * vec4(cos(ang) * width, -height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
                // Safe, GLfloats can represent small integers exactly
                for (int i = 0; i <= sides; i++) {
                    // Angle between each side in radians
                    float ang = (PI * 2.0) / sides * i;
                    if(i%2==(sides%2))
                        gl_Position = aux + model * view * projection * vec4(0.0f,height,0.0f,0.0f);
                        EmitVertex();

                    gColor = vec4(ang/7,0.1,ang/10,1.0);
                    // Offset from center of point (0.3 to accomodate for aspect ratio)
                    vec4 offset = model * view * projection * vec4(cos(ang) * width, height, -sin(ang) * 0.1, 0.0);
                    gl_Position = aux + offset;
                    EmitVertex();

                }
                EndPrimitive();
    }

    void makeSphere(float size, float sides){
        // Safe, GLfloats can represent small integers exactly
        for (int i = 0; i <= sides; i++) {
            float ang = (PI * 2.0) / sides * i;
            // Angle between each side in radians
            for (int j = 0; j <= sides; j++)
            {
            float ang2 = (PI * 2.0) / sides * j;
            float ang3 = (PI * 2.0) / sides * (j+1);
            gColor = vec4(0.1,ang/10,0.1,1.0);
            
            // Offset from center of point (0.3 to accomodate for aspect ratio)
            vec4 offset = model * view * projection * vec4(sin(ang)*cos(ang2) * 0.1, sin(ang)*sin(ang2) * 0.1, cos(ang) * 0.1, 0.0);
            gl_Position = aux + offset;
            EmitVertex();
//            offset = model * view * projection * vec4(sin(ang)*cos(ang2) * 0.1, sin(ang)*sin(ang2) * 0.1, cos(ang) * 0.1, 0.0);
//            gl_Position = aux + offset;
//            EmitVertex();

            }
            EndPrimitive();
        }
        EndPrimitive();



    }

    void makeBox(float size_x, float size_y){
        makeCyl(size_x,size_y,4);
    }

    void main() {
            gColor = vec4(0.1,0.67,0.3,1.0);
        switch (vType[0]){

            case 1:
                makeBox(vSize[0].x,vSize[0].y);
            
            break;

            case 2:
                makeSphere(vSize[0].x,10);
            break;

            case 3:
                makeCyl(vSize[0].x,vSize[0].y,int(vSides[0]));
            break;

            default:
                gl_Position = aux + model * view * projection * vec4(0.1, -0.1, 0.0, 0.0);
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-0.1, -0.1, 0.0, 0.0);
                EmitVertex();

                gl_Position = aux + model * view * projection * vec4(-0.1, 0.1, 0.0, 0.0);
                EmitVertex();
                EndPrimitive();
        }

    }
);

// Fragment shader
const GLchar* fragmentShaderSrc = GLSL(
	in vec2 vPos;
    in vec4 gColor;

    out vec4 outColor;

	float rand(vec2 co){
		return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
	}

    //vec4(gColor[0]); //
    void main() {
        outColor = gColor;//vec4(rand(vPos), rand(vPos), rand(vPos), 1.0);
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

    glShaderSource(shader, 1, &src, nullptr);
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

#define VALUES_PER_POINT 6
#define N_POINTS 1000

float* buildPoints(int n){
    float* ret = (float *)malloc(sizeof(float)*VALUES_PER_POINT*n);
    for (int i = 0; i < n; ++i)
    {
            // pos-x, pos-y, type, n_sides, size-x, size-y
            ret[i*VALUES_PER_POINT+0] = ((float) rand() / (RAND_MAX/3)) - 1.0f;
            ret[i*VALUES_PER_POINT+1] = ((float) rand() / (RAND_MAX/3)) - 1.0f;
            ret[i*VALUES_PER_POINT+2] = (i % 3)+ 1;
            ret[i*VALUES_PER_POINT+3] = ((float) (rand() % 30 ));
            ret[i*VALUES_PER_POINT+4] = ((float) rand() / (RAND_MAX))/10;
            ret[i*VALUES_PER_POINT+5] = ((float) rand() / (RAND_MAX))/10;
    }


    return ret;
}


int main() {
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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GL_DEPTH_BITS, 49);
    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_STICKY_KEYS, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL",  glfwGetPrimaryMonitor(), nullptr); // Windowed

    glfwMakeContextCurrent(window);
 

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
//    glEnable(GL_CULL_FACE);                                          // Enable face culling
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

    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    
    GLuint tessEvalShader = createShader(GL_TESS_EVALUATION_SHADER, tesselationEvalShaderSrc);
    GLuint tessCtrlShader = createShader(GL_TESS_CONTROL_SHADER, tesselationCtrlShaderSrc);

    GLuint shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
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
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat a=0.45f, m_a=-0.45f;


    // pos-x, pos-y, type, n_sides, size-x, size-y
    /*GLfloat points[] = {
    	m_a,  a, 3.0f, 5.0f, 0.2f,0.1f,
        a,  a, 2.0f, 7.0f, 0.1f,0.1f,
        a, m_a, 3.0f, 20.0f, 0.1f,0.3f,
		m_a, m_a, 3.0f, 4.0f, 0.1f,0.3f,
        0.0f, 0.0f, 1.0f, 3.0f, 0.1f,0.1f
    };*/

    GLfloat* points = buildPoints(N_POINTS);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*VALUES_PER_POINT*N_POINTS, points, GL_STATIC_DRAW);

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

    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    // Specify layout of point data
    posAttrib = glGetAttribLocation(shaderProgram, "type");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    // Specify layout of point data
    posAttrib = glGetAttribLocation(shaderProgram, "sides");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));

    // Specify layout of point data
    posAttrib = glGetAttribLocation(shaderProgram, "size");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (4 * sizeof(GLfloat)));

    GLenum shader1 = GL_TRUE;

    GLint   timeLoc = glGetUniformLocation(shaderProgram, "time"); /* Uniform index for variable "time" in shader */
    GLfloat timeValue; /* Application time */
    while(!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
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
        

        
        // Create transformations
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 0.1f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
//        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

//        model = glm::mat4(1.0f);
//        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);

        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection"); 
        
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
        
        a+=0.01;
        timeValue = a;
        
        glUniform1f(timeLoc, timeValue);
        

        //glPatchParameteri(GL_PATCH_VERTICES, 4);       // tell OpenGL that every patch has 16 verts
        //glDrawArrays(GL_PATCHES, 0, 5); 
        glDrawArrays(GL_POINTS, 0, N_POINTS);

        glfwSwapBuffers(window);

    }
    glfwTerminate();

    // Render loop
    /*
     while (window.isOpen()) {
        // Handle events
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                break;
            }
        }



        window.display();
    }
	*/
    return 0;
}
