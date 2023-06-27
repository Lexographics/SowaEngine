#include "app.hxx"

#include "core/graphics.hxx"
#include "servers/input_server.hxx"
#include "servers/rendering_server.hxx"

#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef SW_WEB
#include <emscripten.h>
#endif

App::~App() {
}

Error App::Init() {
	RenderingServer::GetInstance().CreateWindow(800, 600, "Sowa Engine");

	// Initialize rendering
	ModelBuilder::Quad2D(rectModel);

	mainShader.SetVertexSource(GLSL(
		precision mediump float;

		layout(location = 0) in vec3 aPos;
		void main() {
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}));
	mainShader.SetFragmentSource(GLSL(
		precision mediump float;
		out vec4 FragColor;

		void main() {
			FragColor = vec4(1.0f, 0.6f, 0.2f, 1.0f);
			// gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		}));
	mainShader.Build();

	// Create working dir
	EM_ASM(
		FS.mkdir('/app');
		FS.mount(IDBFS, {}, '/app');
		FS.syncfs(
			true, function(err) {
				console.log('synced');
				if (err) {
					console.error("An error occured while syncing fs", err);
				}
				Module.ccall('FSSyncReady');
			}););

	if (std::filesystem::exists("/app/test4")) {
		std::cout << "File exists" << std::endl;
	} else {
		std::cout << "File doesnt exists" << std::endl;
	}

	// std::ofstream ofstream("/app/test4");
	// ofstream << "testtest" << std::endl;
	// ofstream.close();

	// std::ofstream ofstream2("/app/test5");
	// ofstream2 << "testtest" << std::endl;
	// ofstream2.close();

	for (auto &entry : std::filesystem::directory_iterator("/app")) {
		std::cout << "Got: " << entry.path() << std::endl;
	}

	return OK;
}

Error App::Run() {
#ifdef SW_WEB
	emscripten_set_main_loop_arg(mainLoopCaller, this, 0, 1);
#else
	while (!RenderingServer::GetInstance().WindowShouldClose()) {
		mainLoop();
	}
#endif
	return OK;
}

void App::mainLoop() {
	InputServer::GetInstance().ProcessInput();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	mainShader.Bind();
	rectModel.Draw();

	RenderingServer::GetInstance().SwapBuffers();
	InputServer::GetInstance().PollEvents();
}

void App::mainLoopCaller(void *self) {
	static_cast<App *>(self)->mainLoop();
}

extern "C" void FSSyncReady() {
	if (std::filesystem::exists("/app/test4")) {
		std::cout << "File exists" << std::endl;
	} else {
		std::cout << "File doesnt exists" << std::endl;
	}
}

extern "C" void Unload() {
	EM_ASM(
		FS.syncfs(
			false, function(err) {
				if (err) {
					alert("An error occured while syncing fs", err);
				} else {
					console.log("Successfully synced");
				}
			}););
}