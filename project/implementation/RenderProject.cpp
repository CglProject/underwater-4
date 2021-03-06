#include "RenderProject.h"

/* Initialize the Project */
void RenderProject::init()
{
	bRenderer::loadConfigFile("config.json");	// load custom configurations replacing the default values in Configuration.cpp

	// let the renderer create an OpenGL context and the main window
	if(Input::isTouchDevice())
		bRenderer().initRenderer(true);										// full screen on iOS
	else
		bRenderer().initRenderer(1920, 1080, false, "Underwater World");		// windowed mode on desktop
		//bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// full screen using full width and height of the screen

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void RenderProject::initFunction()
{
	// get OpenGL and shading language version
	bRenderer::log("OpenGL Version: ", glGetString(GL_VERSION));
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// initialize variables
	_offset = 0.0f;
	_randomOffset = 0.0f;
	_cameraSpeed = 40.0f;
	_running = false; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;
	_viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);

	// set shader versions (optional)
	bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
	bRenderer().getObjects()->setShaderVersionES("#version 100");
    
    // Load Shaders
    // ------------
    
    // Normals shader (for debugging): uses normals as color.
    ShaderPtr normalsShader = bRenderer().getObjects()->loadShaderFile("normals", 0, false, false, false, false, false);
    
    // Uniform shader (for debugging): shows the object in a single color
    ShaderPtr uniformShader = bRenderer().getObjects()->loadShaderFile("uniform", 0, false, false, false, false, false);
    
    ShaderPtr guyShader = bRenderer().getObjects()->loadShaderFile("guy", 0, false, false, false, false, false);
    
    ShaderPtr doriFishShader = bRenderer().getObjects()->loadShaderFile("dori_fish", 0, false, false, false, false, false);
    
    ShaderPtr leopardFishShader = bRenderer().getObjects()->loadShaderFile("leopard_fish", 0, false, false, false, false, false);
    
    ShaderPtr chestShader = bRenderer().getObjects()->loadShaderFile("chest", 0, false, false, false, false, false);
    
    ShaderPtr waterSurfaceShader = bRenderer().getObjects()->loadShaderFile("waterSurfaceShader", 0, false, false, false, false, false);
    
    ShaderPtr groundShader = bRenderer().getObjects()->loadShaderFile("groundShader", 0, false, false, false, false, false);
    
    // ShaderPtr rock1Shader = bRenderer().getObjects()->loadShaderFile("rock1", 0, false, false, false, false, false);
    
    // ShaderPtr chestShader = bRenderer().getObjects()->loadShaderFile("Chest", 0, false, false, false, false, false);
    
    
    // Additional Properties for Models
    // --------------------------------
    
    // PropertiesPtr flatFishProperties = bRenderer().getObjects()->createProperties("flatFishProperties");
    // PropertiesPtr chestProperties = bRenderer().getObjects()->createProperties("chestProperties");
    // PropertiesPtr guyProperties = bRenderer().getObjects()->createProperties("guyProperties");
    PropertiesPtr leopardFishProperties = bRenderer().getObjects()->createProperties("leopardFishProperties");
    PropertiesPtr doriFishProperties = bRenderer().getObjects()->createProperties("doriFishProperties");
    // PropertiesPtr rock1Properties = bRenderer().getObjects()->createProperties("rockProperties");
    PropertiesPtr chestProperties = bRenderer().getObjects()->createProperties("chestProperties");

    
    // Load Object Models
    // ------------------
    
    // bRenderer().getObjects()->loadObjModel("flat_fish.obj", true, true, normalsShader, flatFishProperties);
    // bRenderer().getObjects()->loadObjModel("Chest.obj", true, true, guyShader, chestProperties);
    // bRenderer().getObjects()->loadObjModel("guy.obj", false, true, guyShader, guyProperties);
    // bRenderer().getObjects()->loadObjModel("guy.obj", false, true, false, 1, true, false);
    // bRenderer().getObjects()->loadObjModel("cave.obj", true, true, false, 1, true, false);
    // bRenderer().getObjects()->loadObjModel("Chest.obj", false, true, false, 1, true, false);
    // bRenderer().getObjects()->loadObjModel("f.obj", true, true, false, 1, true, false);
    // bRenderer().getObjects()->loadObjModel("Rock1.obj", false, true, false, 1, true, false);
    
    // bRenderer().getObjects()->createLight("testLight", vmml::Vector3f(0.0f, 10.f, 0.0f), vmml::Vector3f(1.f, 1.f, 1.f), 100.0f, 0.1f, 1000.0f);
    
    // bRenderer().getObjects()->loadObjModel("Rock1.obj", false, true, false, 1, true, false);
    bRenderer().getObjects()->loadObjModel("leopard_fish.obj", false, true, leopardFishShader, leopardFishProperties);
    bRenderer().getObjects()->loadObjModel("dori_fish.obj", false, true, doriFishShader, doriFishProperties);
    bRenderer().getObjects()->loadObjModel("chest.obj", false, true, chestShader, chestProperties);
    
    
    
    
    // bRenderer().getObjects()->loadObjModel("Rock1.obj", false, true, uniformShader, rockProperties);
    
    
    // Create Sprites
    // --------------
    
    // Sprites for Menu
    // ................
    
	bRenderer().getObjects()->createSprite("menuTitle", "menu_title.png");							// create a sprite displaying the title as a texture

	// create text sprites
	FontPtr font = bRenderer().getObjects()->loadFont("MenuFont.otf", 50);
    if (Input::isTouchDevice()) {
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Double tap to start", font);
    } else {
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Press space to start", font);
    }
    if (Input::isTouchDevice()) {
        bRenderer().getObjects()->createTextSprite("wonInstructions", vmml::Vector3f(1.f, 1.f, 1.f), "You won! Double tap to restart", font);
    } else {
        bRenderer().getObjects()->createTextSprite("wonInstructions", vmml::Vector3f(1.f, 1.f, 1.f), "You won! Press space to restart", font);
    }
    if (Input::isTouchDevice()) {
        bRenderer().getObjects()->createTextSprite("lostInstructions", vmml::Vector3f(1.f, 1.f, 1.f), "You died! Double tap to retry", font);
    } else {
        bRenderer().getObjects()->createTextSprite("lostInstructions", vmml::Vector3f(1.f, 1.f, 1.f), "You died! Press space to retry", font);
    }
    
    // Sprites for Ground and Floor
    // ............................
    
    MaterialPtr waterSurfaceMaterial = bRenderer().getObjects()->createMaterial("waterSurfaceMaterial", waterSurfaceShader);								// create an empty material to assign either texture1 or texture2 to
    bRenderer().getObjects()->createSprite("waterSurfaceSprite", waterSurfaceMaterial);
    
    MaterialPtr groundMaterial = bRenderer().getObjects()->createMaterial("groundMaterial", groundShader);								// create an empty material to assign either texture1 or texture2 to
    bRenderer().getObjects()->createSprite("groundSprite", groundMaterial);
    
    // Create Camera
    // -------------
    
	//bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(-33.0, 0.f, -13.0), vmml::Vector3f(0.f, -M_PI_F / 2, 0.f));
    bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(0.0f, 0.0f, 10.0f), vmml::Vector3f(0.f, 0.0f, 0.f));
    
    
	// Create Lights
    // -------------
    
	bRenderer().getObjects()->createLight("firstLight", vmml::Vector3f(78.0f, -3.0f, 0.0f), vmml::Vector3f(0.5f, 0.5f, 1.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.4f, 100.0f);

	// Postprocessing
    // --------------
    
	bRenderer().getObjects()->createFramebuffer("fbo");					// create framebuffer object
	bRenderer().getObjects()->createTexture("fbo_texture1", 0.f, 0.f);	// create texture to bind to the fbo
	bRenderer().getObjects()->createTexture("fbo_texture2", 0.f, 0.f);	// create texture to bind to the fbo
	ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile("blurShader", 0, false, false, false, false, false);			// load shader that blurs the texture
	MaterialPtr blurMaterial = bRenderer().getObjects()->createMaterial("blurMaterial", blurShader);								// create an empty material to assign either texture1 or texture2 to
	bRenderer().getObjects()->createSprite("blurSprite", blurMaterial);																// create a sprite using the material created above
    
    // ---
    
	// Update render queue
	updateRenderQueue("camera", 0.0f, 0.0);
}

/* Draw your scene here */
void RenderProject::loopFunction(const double &deltaTime, const double &elapsedTime)
{
//	bRenderer::log("FPS: " + std::to_string(1 / deltaTime));	// write number of frames per second to the console every frame

    
    if (_running) {
        // Won?
        vmml::Vector3f diverPosition = bRenderer().getObjects()->getCamera("camera")->getPosition();
        vmml::Vector3f treasurePosition(-0.0, 5.0, -10.0);
        float distance = (diverPosition - treasurePosition).length();
        float winDistance = 7.0;
        bRenderer::log(std::string("You are at: (") + std::to_string(diverPosition.x()) + ", " + std::to_string(diverPosition.y()) + ", " + std::to_string(diverPosition.z()) + ")");
        bRenderer::log(std::string("Your distance to the treasure: ") + std::to_string(distance));
        if (distance <= winDistance) {
            _running = false;
            _won = true;
        }
    }
    
    
    
    
    
    
	//// Draw Scene and do post processing ////

	/// Begin post processing ///
	GLint defaultFBO;
	if (!_running){
		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth() / 5, bRenderer().getView()->getHeight() / 5);		// reduce viewport size
		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_texture1"), false);	// bind the fbo
	}
    
	/// Draw scene ///	
	
	bRenderer().getModelRenderer()->drawQueue(/*GL_LINES*/);
	bRenderer().getModelRenderer()->clearQueue();
	
	if (!_running){
		/// End post processing ///		
        /*** Blur ***/
		// translate
		vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
		// blur vertically and horizontally
		bool b = true;		int numberOfBlurSteps = 2;
		for (int i = 0; i < numberOfBlurSteps; i++) {
			if (i == numberOfBlurSteps - 1){
				bRenderer().getObjects()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)
				bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());								// reset vieport size
			}
			else
				bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture(b ? "fbo_texture2" : "fbo_texture1"), false);
			bRenderer().getObjects()->getMaterial("blurMaterial")->setTexture("fbo_texture", bRenderer().getObjects()->getTexture(b ? "fbo_texture1" : "fbo_texture2"));
			bRenderer().getObjects()->getMaterial("blurMaterial")->setScalar("isVertical", static_cast<GLfloat>(b));
			// draw
			bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getModel("blurSprite"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
			b = !b;
		}
	
        /*** Title ***/
        // translate and scale 
        GLfloat titleScale = 0.5f;
        vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-0.0f, 0.0f, -0.65f)) * scaling;
        // draw
		bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getModel("menuTitle"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false);

		/*** Instructions ***/
		titleScale = 0.1f;
		
		// draw
        if (_won) {
            scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
            modelMatrix = vmml::create_translation(vmml::Vector3f(-0.65f / bRenderer().getView()->getAspectRatio(), -0.6f, -0.65f)) * scaling;
            bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getTextSprite("wonInstructions"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
        } else if (_lost) {
            scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
            modelMatrix = vmml::create_translation(vmml::Vector3f(-0.65f / bRenderer().getView()->getAspectRatio(), -0.6f, -0.65f)) * scaling;
            bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getTextSprite("lostInstructions"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
        } else {
            scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
            modelMatrix = vmml::create_translation(vmml::Vector3f(-0.45f / bRenderer().getView()->getAspectRatio(), -0.6f, -0.65f)) * scaling;
            bRenderer().getModelRenderer()->drawModel(bRenderer().getObjects()->getTextSprite("instructions"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
        }
    }

	//// Camera Movement ////
	updateCamera("camera", deltaTime);

	/// Update render queue ///
	updateRenderQueue("camera", deltaTime, elapsedTime);

	// Quit renderer when escape is pressed
	if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::INPUT_PRESS)
		bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void RenderProject::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}

/* Update render queue */
void RenderProject::updateRenderQueue(const std::string &camera, const double &deltaTime, const double &elapsedTime)
{
    
    
    // vmml::Matrix4f modelMatrix4 = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.0f)) * vmml::create_scaling(vmml::Vector3f(1.0f));
    // bRenderer().getModelRenderer()->drawModel("Rock1", camera, modelMatrix4, std::vector<std::string>({ "testLight" }), true, false);
    
     vmml::Matrix4f viewMatrix = bRenderer().getObjects()->getCamera("camera")->getViewMatrix();
    
    ShaderPtr doriFishShader = bRenderer().getObjects()->getShader("dori_fish");
    
    if (doriFishShader.get())
    {
        vmml::Matrix4f modelMatrix = vmml::create_rotation((float) elapsedTime * 0.05f, vmml::Vector3f(0.0, 1.0, 0.0)) * vmml::create_translation(vmml::Vector3f(0.0, 0.0, -3.0)) * vmml::create_scaling(vmml::Vector3f(1.0f));
        doriFishShader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        doriFishShader->setUniform("ViewMatrix", viewMatrix);
        doriFishShader->setUniform("ModelMatrix", modelMatrix);
        
        vmml::Matrix3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)), normalMatrix);
        doriFishShader->setUniform("NormalMatrix", normalMatrix);
        
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        doriFishShader->setUniform("EyePos", eyePos);
        
        doriFishShader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        doriFishShader->setUniform("Ia", vmml::Vector3f(1.f));
        doriFishShader->setUniform("Id", vmml::Vector3f(1.f));
        doriFishShader->setUniform("Is", vmml::Vector3f(1.f));
        
        bRenderer().getModelRenderer()->queueModelInstance("dori_fish", "dori_fish_instance", camera, modelMatrix, std::vector<std::string>({ "firstLight" }), true, true);
        

    }
    else
    {
        bRenderer::log("No shader available (dori fish).");
    }
    
    
    
    
    
    
    
    ShaderPtr leopardFishShader = bRenderer().getObjects()->getShader("leopard_fish");
    
    if (leopardFishShader.get())
    {
        vmml::Matrix4f modelMatrix = vmml::create_rotation((float) elapsedTime * 0.5f, vmml::Vector3f(0.0, 1.0, 0.0)) * vmml::create_translation(vmml::Vector3f(0.0, 1.0, -5.0)) * vmml::create_scaling(vmml::Vector3f(1.0f));
        leopardFishShader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        leopardFishShader->setUniform("ViewMatrix", viewMatrix);
        leopardFishShader->setUniform("ModelMatrix", modelMatrix);
        
        vmml::Matrix3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)), normalMatrix);
        leopardFishShader->setUniform("NormalMatrix", normalMatrix);
        
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        leopardFishShader->setUniform("EyePos", eyePos);
        
        leopardFishShader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        leopardFishShader->setUniform("Ia", vmml::Vector3f(1.f));
        leopardFishShader->setUniform("Id", vmml::Vector3f(1.f));
        leopardFishShader->setUniform("Is", vmml::Vector3f(1.f));
        
        bRenderer().getModelRenderer()->queueModelInstance("leopard_fish", "leopard_fish_instance", camera, modelMatrix, std::vector<std::string>({ "firstLight" }), true, true);
        
    }
    else
    {
        bRenderer::log("No shader available (leopard fish).");
    }
    
    
    
    ShaderPtr chestShader = bRenderer().getObjects()->getShader("chest");
    
    if (chestShader.get())
    {
        vmml::Matrix4f modelMatrix = vmml::create_rotation(0.5f, vmml::Vector3f(0.0, 1.0, 0.0)) * vmml::create_translation(vmml::Vector3f(0.0, -7.0, 10.0)) * vmml::create_scaling(vmml::Vector3f(1.0f));
        chestShader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        chestShader->setUniform("ViewMatrix", viewMatrix);
        chestShader->setUniform("ModelMatrix", modelMatrix);
        
        vmml::Matrix3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)), normalMatrix);
        chestShader->setUniform("NormalMatrix", normalMatrix);
        
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        chestShader->setUniform("EyePos", eyePos);
        
        chestShader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        chestShader->setUniform("Ia", vmml::Vector3f(1.f));
        chestShader->setUniform("Id", vmml::Vector3f(1.f));
        chestShader->setUniform("Is", vmml::Vector3f(1.f));
        
        bRenderer().getModelRenderer()->queueModelInstance("chest", "chest_instance", camera, modelMatrix, std::vector<std::string>({ "firstLight" }), true, true);
        
    }
    else
    {
        bRenderer::log("No shader available (leopard fish).");
    }
    
    vmml::Matrix4f modelMatrix3 = vmml::create_translation(vmml::Vector3f(0.0f, 100.0f, -10.0f)) * vmml::create_scaling(vmml::Vector3f(10000.0f));
    modelMatrix3 *= vmml::create_rotation(3*M_PI_2_F, vmml::Vector3f(1.0f, 0.f, 0.f));
    bRenderer().getModelRenderer()->drawModel("waterSurfaceSprite", camera, modelMatrix3, std::vector<std::string>({}), true, false);
    
    vmml::Matrix4f modelMatrix5 = vmml::create_translation(vmml::Vector3f(0.0f, -7.0f, -10.0f)) * vmml::create_scaling(vmml::Vector3f(10000.0f));
    modelMatrix5 *= vmml::create_rotation(M_PI_2_F, vmml::Vector3f(1.0f, 0.f, 0.f));
    bRenderer().getModelRenderer()->drawModel("groundSprite", camera, modelMatrix5, std::vector<std::string>({}), true, false);
    
    /*ShaderPtr rock1Shader = bRenderer().getObjects()->getShader("rock1");
    
    if (rock1Shader.get())
    {
        vmml::Matrix4f modelMatrix = vmml::create_rotation((float) elapsedTime * 0.1f, vmml::Vector3f(0.0, 1.0, 0.0)) * vmml::create_translation(vmml::Vector3f(0.0, 1.0, -5.0)) * vmml::create_scaling(vmml::Vector3f(1.0f));
        rock1Shader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
        rock1Shader->setUniform("ViewMatrix", viewMatrix);
        rock1Shader->setUniform("ModelMatrix", modelMatrix);
        
        vmml::Matrix3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)), normalMatrix);
        rock1Shader->setUniform("NormalMatrix", normalMatrix);
        
        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 10.0f, 1.0f);
        rock1Shader->setUniform("EyePos", eyePos);
        
        rock1Shader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
        rock1Shader->setUniform("Ia", vmml::Vector3f(1.f));
        rock1Shader->setUniform("Id", vmml::Vector3f(1.f));
        rock1Shader->setUniform("Is", vmml::Vector3f(1.f));
        
        bRenderer().getModelRenderer()->queueModelInstance("Rock1", "rock1_instance", camera, modelMatrix, std::vector<std::string>({ "firstLight" }), true, true);
        
        
    }
    else
    {
        bRenderer::log("No shader available (rock 1).");
    }*/
    
    //shader->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrix));
    // bRenderer().getModelRenderer()->drawModel("guy", "camera", modelMatrix, std::vector<std::string>({ }));
    // bRenderer().getModelRenderer()->queueModelInstance("guy", "guy_instance", camera, modelMatrix, std::vector<std::string>({ "firstLight" }), true, true);
    
    // bRenderer().getModelRenderer()->queueModelInstance("f", "f_instance", camera, modelMatrix, std::vector<std::string>({ "firstLight" }), true, true);
    
    
    // bRenderer().getModelRenderer()->queueModelInstance("Chest", "chest_instance", camera, modelMatrix, std::vector<std::string>({ "firstLight" }), true, true);

    // bRenderer().getModelRenderer()->queueModelInstance("Chest", "chest_instance", camera, modelMatrix, std::vector<std::string>({}), true, false, true);
    
    // vmml::Matrix4f modelMatrix3 = vmml::create_translation(vmml::Vector3f(30.f, -24.0, 0.0)) * vmml::create_scaling(vmml::Vector3f(0.3f));
    // bRenderer().getModelRenderer()->queueModelInstance("cave", "cave_instance", camera, modelMatrix3, std::vector<std::string>({ "firstLight" }), true, true);
    
    
    
    
    
}

/* Camera movement */
void RenderProject::updateCamera(const std::string &camera, const double &deltaTime)
{
	//// Adjust aspect ratio ////
	bRenderer().getObjects()->getCamera(camera)->setAspectRatio(bRenderer().getView()->getAspectRatio());

	double deltaCameraY = 0.0;
	double deltaCameraX = 0.0;
	double cameraForward = 0.0;
	double cameraSideward = 0.0;

	/* iOS: control movement using touch screen */
	if (Input::isTouchDevice()){

		// pause using double tap
		if (bRenderer().getInput()->doubleTapRecognized()){
            if (!_running) {
                if (_won || _lost) {
                    bRenderer().getObjects()->getCamera(camera)->setPosition(vmml::Vector3f(0.0f, 0.0f, 10.0f));
                }
                _won = false;
                _lost = false;
            }
			_running = !_running;
		}

		if (_running){
			// control using touch
			TouchMap touchMap = bRenderer().getInput()->getTouches();
			int i = 0;
			for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
			{
				Touch touch = t->second;
				// If touch is in left half of the view: move around
				if (touch.startPositionX < bRenderer().getView()->getWidth() / 2){
					cameraForward = -(touch.currentPositionY - touch.startPositionY) / 100;
					cameraSideward = (touch.currentPositionX - touch.startPositionX) / 100;

				}
				// if touch is in right half of the view: look around
				else
				{
					deltaCameraY = (touch.currentPositionX - touch.startPositionX) / 2000;
					deltaCameraX = (touch.currentPositionY - touch.startPositionY) / 2000;
				}
				if (++i > 2)
					break;
			}
		}

	}
	/* Windows: control movement using mouse and keyboard */
	else{
		// use space to pause and unpause
		GLint currentStateSpaceKey = bRenderer().getInput()->getKeyState(bRenderer::KEY_SPACE);
		if (currentStateSpaceKey != _lastStateSpaceKey)
		{
			_lastStateSpaceKey = currentStateSpaceKey;
			if (currentStateSpaceKey == bRenderer::INPUT_PRESS){
				_running = !_running;
				bRenderer().getInput()->setCursorEnabled(!_running);
			}
		}

		// mouse look
		double xpos, ypos; bool hasCursor = false;
		bRenderer().getInput()->getCursorPosition(&xpos, &ypos, &hasCursor);

		deltaCameraY = (xpos - _mouseX) / 1000;
		_mouseX = xpos;
		deltaCameraX = (ypos - _mouseY) / 1000;
		_mouseY = ypos;

		if (_running){
			// movement using wasd keys
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_W) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = 2.0;
				else			cameraForward = 1.0;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_S) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = -2.0;
				else			cameraForward = -1.0;
			else
				cameraForward = 0.0;

			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_A) == bRenderer::INPUT_PRESS)
				cameraSideward = -1.0;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_D) == bRenderer::INPUT_PRESS)
				cameraSideward = 1.0;
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_UP) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_DOWN) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(-_cameraSpeed*deltaTime);
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, 0.03f*_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_RIGHT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, -0.03f*_cameraSpeed*deltaTime);
		}
	}

	//// Update camera ////
	if (_running){
		bRenderer().getObjects()->getCamera(camera)->moveCameraForward(cameraForward*_cameraSpeed*deltaTime);
		bRenderer().getObjects()->getCamera(camera)->rotateCamera(deltaCameraX, deltaCameraY, 0.0f);
		bRenderer().getObjects()->getCamera(camera)->moveCameraSideward(cameraSideward*_cameraSpeed*deltaTime);
	}	
}

/* For iOS only: Handle device rotation */
void RenderProject::deviceRotated()
{
	if (bRenderer().isInitialized()){
		// set view to full screen after device rotation
		bRenderer().getView()->setFullscreen(true);
		bRenderer::log("Device rotated");
	}
}

/* For iOS only: Handle app going into background */
void RenderProject::appWillResignActive()
{
	if (bRenderer().isInitialized()){
		// stop the renderer when the app isn't active
		bRenderer().stopRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void RenderProject::appDidBecomeActive()
{
	if (bRenderer().isInitialized()){
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
}

/* For iOS only: Handle app being terminated */
void RenderProject::appWillTerminate()
{
	if (bRenderer().isInitialized()){
		// terminate renderer before the app is closed
		bRenderer().terminateRenderer();
	}
}

/* Helper functions */
GLfloat RenderProject::randomNumber(GLfloat min, GLfloat max){
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}