/*
* LICENCE
* copyright 2014 ~ ****
* Some rights reserved.
* Author: HUFANGYUAN
* Released under CC BY-NC
*/
#pragma once
#include "Header.h"
#include "Camera.h"


class Controller {
public:
	Controller() {
		f_scaleVel = 0.01f;
		f_panVel = 0.01f;

		m_scale = m_initScale = glm::vec3(1.f);
		m_rotation = m_initRotation = glm::angleAxis<float>(-glm::pi<float>() * 0.f, glm::vec3(1, 0, 0));
		m_pan = m_initPan = glm::vec3(0.f, 0.f, 0.f);

		m_initCameraPosition = glm::vec3(0.f, 0.f, 5.f);
		m_initCameraRotation = glm::angleAxis<float>(glm::pi<float>() * 0.f, glm::vec3(1, 0, 0));

		f_near = 0.01f;
		f_far = 1000.f;

		b_dirty = true;
		b_stop = true;
		b_leave = false;
		u_width = 1024;
		u_height = 800;

		i_mouseButton = 0;

		m_camera.SetPosition(m_initCameraPosition);
		m_camera.SetRotation(m_initCameraRotation);
		m_camera.SetProjectionRH(45.0f, float(u_width) / float(u_height), f_near, f_far);
	}
	~Controller() {}

	void clickMouse(int button, int state, int x, int y) {
		m_mousePos = glm::ivec2(x, y);

		switch (button) {
		case 1:
		{
			i_mouseButton = 1;
			break;
		}
		case 2:
		{
			i_mouseButton = 2;
			break;
		}
		case 3:
		{
			i_mouseButton = 3;
			break;
		}
		}
	}
	void moveMouse(int x, int y) {
		glm::ivec2 mousePos = glm::ivec2(x, y);
		glm::vec2 delta = glm::vec2(mousePos - m_mousePos);
		m_mousePos = mousePos;

		switch (i_mouseButton) {
		case 1:
		{
			glm::quat rotX = glm::angleAxis<float>(glm::radians(delta.y) * 0.5f, glm::vec3(1, 0, 0));
			glm::quat rotY = glm::angleAxis<float>(glm::radians(delta.x) * 0.5f, glm::vec3(0, 1, 0));
			m_rotation = (rotX * rotY) * m_rotation;
			break;
		}
		case 2:
		{
			m_pan += glm::vec3(f_panVel*delta.x, -f_panVel*delta.y, 0.0f);
			break;
		}
		case 3:
		{
			m_scale += glm::vec3(delta.y * f_scaleVel);
			m_scale = glm::max(m_scale, glm::vec3(0.f, 0.f, 0.f));
			break;
		}
		}
		b_dirty = true;
	}
	void rollMouse(int button, int dir, int x, int y) {
		m_scale *= dir * f_scaleVel;
		b_dirty = true;
	}

	void reshapeWindow() {
		b_dirty = true;
	}

	void pressKey(unsigned char key, int a, int b) {
		switch (key) {
		case 0x1b: //esc
			break;
		case 0x0d: //enter
			break;
		case 0x70: //p
			break;
		case 0x20: //space
			m_scale = m_initScale;
			m_rotation = m_initRotation;
			m_pan = m_initPan;
			break;
		case 0x2c: //,
			break;
		case 0x2e: //.
			break;
		case 0x53: //S
			break;
		case 0x73: //s
			break;
		case 0x10: //ctrl p
			break;
		case 0x31: //1
			break;
		case 0x32: //2
			break;
		case 0x5d: // ]
			break;
		case 0x5b: // [
			break;
		default:
			break;
		}
		b_dirty = true;
	}

public:
	Camera      m_camera;
	glm::ivec2  m_mousePos;
	glm::quat   m_rotation;
	glm::vec3   m_scale;
	glm::vec3   m_pan;
	glm::mat4   m_mvp;
	glm::mat4   m_mvpInv;
	glm::mat4   m_modelMat;
	glm::mat4   m_viewMat;
	glm::mat4   m_projectionMat;
	glm::mat4   m_viewModelMat;
	glm::mat4   m_projectionMatInv;

	GLfloat     f_near;
	GLfloat     f_far;
	GLuint      u_width;
	GLuint      u_height;

	bool        b_init;
	bool        b_dirty;
	bool        b_stop;
	bool        b_leave;
	int         i_mouseButton;


private:
	float       f_scaleVel;
	float       f_panVel;
	glm::vec3   m_initCameraPosition;
	glm::quat   m_initCameraRotation;
	glm::quat   m_initRotation;
	glm::vec3   m_initScale;
	glm::vec3   m_initPan;
};


