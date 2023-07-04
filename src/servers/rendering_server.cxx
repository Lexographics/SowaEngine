#define GLFW_INCLUDE_NONE

#include "rendering_server.hxx"

#include "core/rendering/gl.hxx"

#include <iostream>
#include <stdexcept>
#include <stdio.h>

struct CallbackBridge {
	CallbackBridge() = default;

	void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
		RenderingServer::GetInstance().framebuffer_size_callback(window, width, height);
	}
};

static void CallbackWrapperFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
	CallbackBridge().FramebufferSizeCallback(window, width, height);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
	}
}

static void char_callback(GLFWwindow *window, unsigned int codepoint) {
}

RenderingServer::RenderingServer() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_pStandartCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	m_pPointerCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}

RenderingServer &RenderingServer::GetInstance() {
	static RenderingServer *server = new RenderingServer;
	return *server;
}

void RenderingServer::CreateWindow(int width, int height, const std::string &title) {
	if (nullptr != m_pWindowHandle) {
		throw std::runtime_error("Failed to create window. Window already exists");
	}

	if (width <= 0) {
		width = 640;
	}
	if (height <= 0) {
		height = 480;
	}

	m_pWindowHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (m_pWindowHandle == NULL) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
		return;
	}

	glfwMakeContextCurrent(m_pWindowHandle);

#ifndef SW_WEB
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initailize glad");
		return;
	}
#endif

	glfwSetFramebufferSizeCallback(m_pWindowHandle, CallbackWrapperFramebufferSizeCallback);
	glfwSetCharCallback(m_pWindowHandle, char_callback);
	glfwSetKeyCallback(m_pWindowHandle, key_callback);
}

bool RenderingServer::WindowShouldClose() {
	return glfwWindowShouldClose(m_pWindowHandle);
}

void RenderingServer::SwapBuffers() {
	glfwSwapBuffers(m_pWindowHandle);
}

void RenderingServer::GetWindowSize(int &width, int &height) {
	glfwGetFramebufferSize(m_pWindowHandle, &width, &height);
}

void RenderingServer::SetCursorMode(CursorMode mode) {
	if (mode == CursorMode::Normal) {
		glfwSetCursor(m_pWindowHandle, m_pStandartCursor);
	} else if (mode == CursorMode::Pointer) {
		glfwSetCursor(m_pWindowHandle, m_pPointerCursor);
	}
}

void RenderingServer::Terminate() {
	glfwTerminate();
}

void RenderingServer::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}