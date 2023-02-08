#pragma once

#include "core.h"
#include "texture.h"

namespace GRender {

class InteractiveImage {
public:
	InteractiveImage(const fs::path& filepath);
	InteractiveImage(void) = default;
	~InteractiveImage(void) = default;

	// We don't want it to be copied
	InteractiveImage(const InteractiveImage&) = delete;
	InteractiveImage& operator=(const InteractiveImage&) = delete;

	// But it can be moved, no problem
	InteractiveImage(InteractiveImage&&) noexcept;
	InteractiveImage& operator=(InteractiveImage&&) noexcept;

	// Creates a ImGui Window and display attached Texture
	void display(const std::string& windowName = "InteractiveImage");

	void open(void) { m_View = true; }
	void close(void) { m_View = false; }


	// Checks if the window is opened to view by user
	bool isOpened(void) const { return m_View; }
	bool isFocused(void) const { return m_IsFocused; }
	bool isHovered(void) const { return m_IsHovered; }

private:
	bool m_View = false;
	bool m_IsFocused = false;
	bool m_IsHovered = false;

	glm::vec2 m_Zoom = {1.0f, 1.0f};

	// Texture coordinates to use with ImGui::Image
	glm::vec2 m_PosMin = {0.0f, 0.0f};
	glm::vec2 m_PosMax = {1.0f, 1.0f};

	// Used to centralize image
	glm::vec2 m_Padding = {0.0f, 0.0f};
	glm::vec2 m_AspectRatio = {1.0f, 1.0f};

	Texture m_Texture;
};

} // namespace GRender
