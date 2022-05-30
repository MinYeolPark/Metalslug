#include "iOpenGL.h"

#include "iStd.h"

#include "App.h"
static HDC hDC;
HGLRC hRC;

iFBO* fbo;
GLuint vao, vbo, vbe;
iMatrix* matrixProject;
iMatrix* matrixModelview;

void loadOpenGL(void* hdc)
{
    //
    // setup OpenGL
    //
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    hDC = (HDC)hdc;
    int pixelformat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelformat, &pfd);

    hRC = wglCreateContext(hDC);////////////////////////////////
    wglMakeCurrent(hDC, hRC);

    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK == err)
    {
        printf("GLEW(%s) GL(%s) GLSL(%s)\n",
            glewGetString(GLEW_VERSION), glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION));
    }
    else
    {
        printf("Error: % s\n", glewGetErrorString(err));
    }

#if 1
    if (wglewIsSupported("WGL_ARB_create_context"))
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);

        int attr[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 2,
            WGL_CONTEXT_FLAGS_ARB, 0,
            0,
        };
        hRC = wglCreateContextAttribsARB(hDC, NULL, attr);/////////////////////////////
        wglMakeCurrent(hDC, hRC);

        const char* strGL = (const char*)glGetString(GL_VERSION);
        const char* strGLEW = (const char*)glewGetString(GLEW_VERSION);
        const char* strGLSL = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        printf("gl(%s), glew(%s), glsl(%s)\n", strGL, strGLEW, strGLSL);
    }
#endif

    //
    // initialize OpenGL
    //
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);// default, android
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);// ios, mac, window

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glFrontFace(GL_CCW);// GL_CW
    glDisable(GL_CULL_FACE);

    // Drawing Region
    devSize = iSizeMake(DEV_WIDTH, DEV_HEIGHT);
    //viewport;// resizeOpenGL

    glEnable(GL_BLEND);
    //setGlBlendFunc(iBlendFuncAlpha);
    glBlendFuncSeparate(
#if 0// Ex1
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,//rgb
#else // Ex2
        GL_ONE, GL_ONE_MINUS_SRC_ALPHA,// rgb
#endif
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//a

    // Settings of Texture
    setTexture(LINEAR, CLAMP);

    // Back or Front Buffer(Frame Buffer Object)
    fbo = new iFBO(devSize.width, devSize.height);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbe);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
    uint8 indices[6] = { 0, 1, 2,   1, 2, 3 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint8) * 6, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    matrixProject = new iMatrix;
    matrixProject->loadIdentity();
    matrixProject->ortho(0, devSize.width, devSize.height, 0, -1, 1);

    matrixModelview = new iMatrix;
    matrixModelview->loadIdentity();
}

void freeOpenGL()
{
    // setup OpenGL
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);

    delete fbo;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbe);

    delete matrixProject;
    delete matrixModelview;
}

struct iZoom
{
    iPoint c;
    float scale, scaleS, scaleE;
    float zoomDt, _zoomDt;
    float delayDt, _delayDt;

    bool order;

    iZoom()
    {
        c = iPointZero;
        scale = 1.0f;
        scaleS = 1.0f;
        scaleE = 1.0f;
        zoomDt = 0.0f;
        _zoomDt = 0.0f;
        _delayDt = 0.0f;
        delayDt = 0.0f;
        order = false;
    }

    void set(iPoint c, float s, float zDt, float dDt)
    {
        this->c = c;
        scaleS = this->scale;
        scaleE = s;
        _zoomDt = zDt;
        zoomDt = 0.000001f;
        _delayDt = dDt;
        delayDt = 0.0f;

        order = true;
    }

    float update(float dt)
    {
        if (zoomDt == 0.0f)
            return 1.0f;

        // 줌인중..
        if (zoomDt < _zoomDt)
        {
            zoomDt += dt;
            if (zoomDt > _zoomDt)
            {
                zoomDt = _zoomDt;
                if (order == false)
                {
                    zoomDt = 0.0f;// stop
                    scale = scaleS;
                    return 1.0f;
                }
            }

            if (order)
                scale = linear(zoomDt / _zoomDt, scaleS, scaleE);
            else
                scale = linear(zoomDt / _zoomDt, scaleE, scaleS);
        }
        // 줌이 된 상태
        else if (delayDt < _delayDt)
        {
            scale = scaleE;

            delayDt += dt;
            if (delayDt > _delayDt)
            {
                delayDt = _delayDt;

                zoomDt = 0.000001f;
                order = false;
            }
        }
        return scale;
    }
};
static iZoom zoom;

void setZoom(iPoint c, float scale, float zoomDt, float delayDt)
{
    zoom.set(c, scale, zoomDt, delayDt);
}

void drawOpenGL(float dt, MethodDraw m)
{
    setGlBlendFunc(iBlendFuncAlpha);
    fbo->bind();
    fbo->clear(0, 0, 0, 0);
    m(dt);// drawGame

    fbo->unbind();

    setGlBlendFunc(iBlendFuncPremultipliedAlpha);
    fbo->clear(0, 0, 0, 0);
    setRGBA(1, 1, 1, 1);

    Texture* t = fbo->tex;
    float s = zoom.update(dt);
    if (s == 1.0f)
    {
        drawImage(t, 0, 0, 1, 1,
            TOP | LEFT, 0, 0, t->width, t->height, 2, 0, REVERSE_HEIGHT);
    }
    else
    {
        iPoint p = zoom.c * (1.0f - s);
        drawImage(t, p.x, p.y, s, s,
            TOP | LEFT, 0, 0, t->width, t->height, 2, 0, REVERSE_HEIGHT);
    }

    SwapBuffers(hDC);
}

void resizeOpenGL(int width, int height)
{
    iSize wndSize = iSizeMake(width, height);
    printf("size (%d, %d)\n", width, height);

    float r0 = wndSize.width / devSize.width;
    float r1 = wndSize.height / devSize.height;
    if (r0 < r1)// 세로가 긴 화면
    {
        viewport.origin.x = 0;
        viewport.size.width = wndSize.width;

        float r = wndSize.width / devSize.width;
        viewport.size.height = devSize.height * r;
        viewport.origin.y = (wndSize.height - viewport.size.height) / 2;
    }
    else if (r0 > r1)// 가로가 긴 화면
    {
        viewport.origin.y = 0;
        viewport.size.height = wndSize.height;

        float r = wndSize.height / devSize.height;
        viewport.size.width = devSize.width * r;
        viewport.origin.x = (wndSize.width - viewport.size.width) / 2;
    }
    else// if (r0 == r1)// 정비율
    {
        viewport.origin = iPointZero;
        viewport.size = wndSize;
    }
    //printf("viewport (%.f, %.f, %.f, %.f)\n",
    //    viewport.origin.x, viewport.origin.y,
    //    viewport.size.width, viewport.size.height);
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}


void setGlBlendFunc(iBlendFunc bf)
{
    switch (bf) {

    case iBlendFuncAlpha:// defalt
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        break;

    case iBlendFuncPremultipliedAlpha:
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        break;

    case iBlendFuncAdd:
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        break;
    }
}

void checkShaderID(GLuint id)
{
    GLint result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    int length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    if (result != GL_TRUE)// length > 1
    {
        char* s = new char[length + 1];
        glGetShaderInfoLog(id, length, NULL, s);
        s[length] = 0;
        printf("checkShaderID error : [%s]\n", s);
        delete s;
    }
}

void checkProgramID(GLuint id)
{
    GLint result = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &result);

    int length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    if (result != GL_TRUE)// length > 1
    {
        char* s = new char[length + 1];
        glGetProgramInfoLog(id, length, NULL, s);
        s[length] = 0;
        printf("checkProgramID error : [%s]\n", s);
        delete s;
    }
}

GLuint createShaderID(const char* path, GLuint flag)
{
    int len;
    char* str = loadFile(path, len);

    GLuint id = glCreateShader(flag);
    glShaderSource(id, 1, &str, NULL);
    glCompileShader(id);

    delete str;

    checkShaderID(id);

    return id;
}

void freeShaderID(GLuint id)
{
    glDeleteShader(id);
}

GLuint createProgramID(const char* pathVert, const char* pathFrag)
{
    GLuint vertID = createShaderID(pathVert, GL_VERTEX_SHADER);
    GLuint fragID = createShaderID(pathFrag, GL_FRAGMENT_SHADER);

    GLuint id = glCreateProgram();
    glAttachShader(id, vertID);
    glAttachShader(id, fragID);
    glLinkProgram(id);
    glDetachShader(id, vertID);
    glDetachShader(id, fragID);

    freeShaderID(vertID);
    freeShaderID(fragID);

    checkProgramID(id);

    return id;
}

void freeProgramID(GLuint id)
{
    glDeleteProgram(id);
}

iFBO::iFBO(int width, int height)
{
    // render buffer - depth
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // color buffer
    tex = createTexture(width, height);

    // fbo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, fboBuffs);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    listTex = new Texture * [20];
    listNum = 0;
}

iFBO::~iFBO()
{
    glDeleteRenderbuffers(1, &depthBuffer);

    freeImage(tex);

    glDeleteFramebuffers(1, &fbo);

    delete listTex;
}

void iFBO::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void iFBO::bind()
{
    bind(tex);
}

void iFBO::bind(Texture* tex)
{
    if (listNum == 0)
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
    listTex[listNum] = tex;
    listNum++;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, fboBuffs);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);

    glViewport(0, 0, tex->width, tex->height);
}

void iFBO::unbind()
{
    listNum--;
    if (listNum)
    {
        bind(listTex[listNum - 1]);
        listNum--;
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
        glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
    }
}

static TextureFilter _minFilter, _magFilter;
static TextureWrap _wrap;
void setTexture(TextureFilter filter, TextureWrap wrap)
{
    _minFilter = _magFilter = filter;
    _wrap = wrap;
}
void setTexture(TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap)
{
    _minFilter = minFilter;
    _magFilter = magFilter;
    _wrap = wrap;
}

void applyTexture()
{
    GLenum e = (_wrap == CLAMP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);

    e = (_minFilter == NEAREST ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, e);

    e = (_magFilter == NEAREST ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, e);
}

void setTexture(uint32 texID, TextureFilter filter, TextureWrap wrap)
{
    setTexture(texID, filter, filter, wrap);
}
void setTexture(uint32 texID, TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap)
{
    glBindTexture(GL_TEXTURE_2D, texID);

    GLenum e = (wrap == CLAMP ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);

    e = (minFilter == NEAREST ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, e);

    e = (magFilter == NEAREST ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, e);

    glBindTexture(GL_TEXTURE_2D, 0);
}
