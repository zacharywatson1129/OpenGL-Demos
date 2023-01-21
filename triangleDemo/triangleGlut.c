/*
    gcc triangleGlut.c -lglut -lGL -lGLU -o triangleGlut
*/

#include <stdio.h>
#include <GL/glut.h>

void display();

// Vertex shader
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader
const char *fragmentShaderSourceRed = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

unsigned int shaderProgram, VAO;


int main(int argc, char** argv) 
{
    /* 
    Regular GLUT set-up, except before we call glutMainLoop()
    we need to do the modern OpenGL stuff, which is quite a few
    steps. We have to create a vertex shader and a fragment shader,
    as well as bind stuff using a Vertex
    */
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //glutInitWindowSize(500, 500);
    //glutInitWindowPosition(0, 0);
    glutCreateWindow("Triangle");
    glutDisplayFunc(display); // specify function which will redraw stuff.
    
    // Triangle data.
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    // Set up the vertex shader and compile it, report any errors.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        printf("%s\n", infoLog);
    }

    // Set up the fragment shader and compile it, report any errors.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceRed, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        printf("%s\n", infoLog);
    }

    // -------- Put the shaders together ----------
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Finally, start the render loop, as we normally do.
    glutMainLoop();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void display()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFlush(); // Do NOT forget this. Otherwise, you get the weird draggy effect ^_^
}
