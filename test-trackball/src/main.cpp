#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#if defined(__ANDROID__)
#define GLFW_EXPOSE_NATIVE_ANDROID
#include <GLFW/glfw3native.h>
#endif

#include "common.h"

#include "TrackballControls.h"
#include "ObjLoader.h"

#include <glm/gtc/matrix_transform.hpp>

static const GLchar* vertex_shader_source =
    "#version 300 es\n"
    "uniform mat4 MVP;\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
    "layout(location = 1) in vec3 vertexColor;\n"
    "out vec3 fragmentColor;\n"
    "void main() {\n"
    "    gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);\n"
    "    fragmentColor = vertexColor;\n"
    "}\n";

static const GLchar* fragment_shader_source =
    "#version 300 es\n"
    "in vec3 fragmentColor;\n"
    "out vec3 color;\n"
    "void main() {\n"
    "    color = fragmentColor;\n"
    "}\n";

static GLint get_shader_program(const char *vertex_shader_source, const char *fragment_shader_source) {
    enum Consts {INFOLOG_LEN = 512};
    GLchar infoLog[INFOLOG_LEN];
    GLint fragment_shader;
    GLint shader_program;
    GLint success;
    GLint vertex_shader;

    /* Vertex shader */
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
        LOGF("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    /* Fragment shader */
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
        LOGF("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    /* Link shaders */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, INFOLOG_LEN, NULL, infoLog);
        LOGF("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}


// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};


int main() {
    GLuint screen_width = 800;
    GLuint screen_height = 600;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Window Title", NULL, NULL);

    if (!window) {
        LOGF("Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    LOGI("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    LOGI("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

#if defined(__ANDROID__)
    struct android_app * cyborg_app = glfwGetAndroidApp(window);
    JNIEnv * cyborg_env = cyborg_app->activity->env;
    AAssetManager * cybord_ass = cyborg_app->activity->assetManager;
    setAndroidAssetManager(cybord_ass);

    screen_width = ANativeWindow_getWidth(cyborg_app->window);
    screen_height = ANativeWindow_getHeight(cyborg_app->window);
#endif

    GLuint shader_program = get_shader_program(vertex_shader_source, fragment_shader_source);
    GLint pos = glGetAttribLocation(shader_program, "position");

    std::string obj_filename = "epol.obj";
    objl::Loader<asset_istream> obj_loader;
    if (!obj_loader.LoadFile(obj_filename)) {
        LOGE("Error loading OBJ Model: '%s'", obj_filename.c_str());
    } else {
        std::vector<glm::vec3> m_verts;
        std::vector<glm::vec3> m_norms;
        std::vector<glm::vec2> m_uv;
        std::vector<std::vector<glm::ivec3> > m_faces; // this ivec3 means vertex/uv/normal

        for (unsigned int i = 0; i < obj_loader.LoadedMeshes.size(); i++) {
            // Copy one of the loaded meshes to be our current mesh
            objl::Mesh curMesh = obj_loader.LoadedMeshes[i];

            // Print Mesh Name
            LOGV("Mesh: '%s'", curMesh.MeshName.c_str());

            // Print Vertices
            LOGV("Vertices:");

            // Go through each vertex and print its number, position, normal and texture coordinate
            for (unsigned int j = 0; j < curMesh.Vertices.size(); j++) {
                LOGV("V%d: P(%f, %f, %f), N(%f, %f, %f), TC(%f, %f)", j,
                    curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z,
                    curMesh.Vertices[j].Normal.X,curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z,
                    curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y
                );

                glm::vec3 v(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
                m_verts.push_back(v);
                glm::vec3 n(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z);
                n = glm::normalize(n);
                m_norms.push_back(n);
                glm::vec2 uv(curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y);
                m_uv.push_back(uv);

            }

            // Print Indices
            LOGV("Indices:");

            // Go through every 3rd index and print the triangle that these indices represent
            for (unsigned int j = 0; j < curMesh.Indices.size(); j += 3) {
                LOGV("T%d: %d, %d, %d", j / 3, curMesh.Indices[j], curMesh.Indices[j + 1], curMesh.Indices[j + 2]);

                std::vector<glm::ivec3> f;
                for (unsigned int k = 0; k < 3; k++) {
                    glm::ivec3 tmp(
                        curMesh.Indices[j + k], // Index of the vertex position
                        curMesh.Indices[j + k], // Index of the texture coordinate (uv)
                        curMesh.Indices[j + k]  // Index of the vertex normal
                    );
                    f.push_back(tmp);
                }
                m_faces.push_back(f);
            }

            // Print Material
            LOGV("Material: '%s'", curMesh.MeshMaterial.name.c_str());
            LOGV("Ambient Color: %f, %f, %f", curMesh.MeshMaterial.Ka.X, curMesh.MeshMaterial.Ka.Y, curMesh.MeshMaterial.Ka.Z);
            LOGV("Diffuse Color:  %f, %f, %f", curMesh.MeshMaterial.Kd.X, curMesh.MeshMaterial.Kd.Y, curMesh.MeshMaterial.Kd.Z);
            LOGV("Specular Color:  %f, %f, %f", curMesh.MeshMaterial.Ks.X, curMesh.MeshMaterial.Ks.Y, curMesh.MeshMaterial.Ks.Z);
            LOGV("Specular Exponent: %f", curMesh.MeshMaterial.Ns);
            LOGV("Optical Density: %f", curMesh.MeshMaterial.Ni);
            LOGV("Dissolve: %f", curMesh.MeshMaterial.d);
            LOGV("Illumination: %d", curMesh.MeshMaterial.illum);
            LOGV("Ambient Texture Map: '%s'", curMesh.MeshMaterial.map_Ka.c_str());
            LOGV("Diffuse Texture Map: '%s'", curMesh.MeshMaterial.map_Kd.c_str());
            LOGV("Specular Color Texture Map: '%s'", curMesh.MeshMaterial.map_Ks.c_str());
            LOGV("Specular Highlight Texture Map: '%s'", curMesh.MeshMaterial.map_Ns.c_str());
            LOGV("Alpha Texture Map: '%s'", curMesh.MeshMaterial.map_d.c_str());
            LOGV("Bump Map: '%s'", curMesh.MeshMaterial.map_bump.c_str());
        }
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, screen_width, screen_height);

    glEnable(GL_DEPTH_TEST); // Enable depth test
    glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one


    // 1st attribute buffer : vertices

    GLuint vertexbuffer; // This will identify our vertex buffer
    glGenBuffers(1, &vertexbuffer); // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // The following commands will talk about our 'vertexbuffer' buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // Give our vertices to OpenGL.

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // MVP matrix

    // [Model Coordinates]
    //          x [Model Matrix] ---> [World Coordinates]
    //                                         x [View Matrix] ---> [Camera Coordinates]
    //                                                                        x [Projection Matrix] ---> [Homogeneous Coordinates]

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float)screen_height, 0.1f, 100.0f);
    //~ glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // For an ortho camera - In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);

    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(shader_program, "MVP");

    // Trackball Controls

    sasmaster::Camera3D tCam(glm::vec3(0.0f, 0.0f, 4.0f));
    sasmaster::TrackballControls* tball = &sasmaster::TrackballControls::GetInstance(&tCam, glm::vec4(0.0f, 0.0f, (float)screen_width, (float)screen_height));
    tball->Init(window); // Init GLFW callbacks

    // Main loop

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tball->Update();
        mvp = Projection * tCam.m_viewMatr * Model;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glUseProgram(shader_program);

        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);

    //~ glDetachShader(shader_program, vertex_shader);
    //~ glDetachShader(shader_program, fragment_shader);

    //~ glDeleteShader(vertex_shader);
    //~ glDeleteShader(fragment_shader);

    glfwTerminate();
}
