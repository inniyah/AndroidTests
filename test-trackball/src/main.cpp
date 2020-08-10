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
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position =  MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"

    "}\0";

static const GLchar* fragment_shader_source =
    "#version 300 es\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    LOGI("GL_VERSION  : %s\n", glGetString(GL_VERSION));
    LOGI("GL_RENDERER : %s\n", glGetString(GL_RENDERER));

#if defined(__ANDROID__)
    struct android_app * cyborg_app = glfwGetAndroidApp(window);
    JNIEnv * cyborg_env = cyborg_app->activity->env;
    AAssetManager * cybord_ass = cyborg_app->activity->assetManager;
    setAndroidAssetManager(cybord_ass);

    screen_width = ANativeWindow_getWidth(cyborg_app->window);
    screen_height = ANativeWindow_getHeight(cyborg_app->window);
#endif

    LOGI("Screen Size: %d x %d\n", screen_width, screen_height);

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

    // See: https://learnopengl.com/Getting-started/Hello-Triangle
    // See: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp

    // vertex shader

    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOGF("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // fragment shader

    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOGF("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // link shaders

    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        LOGF("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up data

    float vertices[] = {
         1.0f,  1.0f,  1.0f,  // top right
         1.0f, -1.0f,  1.0f,  // bottom right
        -1.0f, -1.0f, -1.0f,  // bottom left
        -1.0f,  1.0f, -1.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object (VAO) first
    glBindVertexArray(VAO);

    // then bind and set vertex buffers (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // and finally configure vertex attributes (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, screen_width, screen_height);

    glEnable(GL_DEPTH_TEST); // Enable depth test
    glDepthFunc(GL_LESS);    // Accept fragment if it closer to the camera than the former one

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // MVP matrix

    // [Model Coordinates]
    //          x [Model Matrix] ---> [World Coordinates]
    //                                         x [View Matrix] ---> [Camera Coordinates]
    //                                                                        x [Projection Matrix] ---> [Homogeneous Coordinates]

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float)screen_height, 0.1f, 100.0f);
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // For an ortho camera - In world coordinates

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
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    // Trackball Controls

    sasmaster::Camera3D tCam(glm::vec3(0.0f, 0.0f, 4.0f));
    sasmaster::TrackballControls* tball = &sasmaster::TrackballControls::GetInstance(&tCam, glm::vec4(0.0f, 0.0f, (float)screen_width, (float)screen_height));
    tball->Init(window); // Init GLFW callbacks

    // Main loop

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tball->Update();
        mvp = Projection * tCam.m_viewMatr * Model;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // draw our first triangle
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}
