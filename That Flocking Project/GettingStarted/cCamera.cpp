#include "cCamera.h"
#include "global.h"
#include "Input.h"

#include <glm\gtc\quaternion.hpp>

cCamera::cCamera()
{
	mWidth = 1000;
	mHeight = 1000;

	mZoomSpeed = 20.f;
	mDistance = 70.f;
	mMinDistance = 5.f;
	mMaxDistance = 300.f;
	mOffset = glm::vec3(0.f, 10.f, -10.f);
	mOffset = glm::normalize(mOffset);

	m_yaw = 0.f;
	m_minYaw = -0.7f;
	m_maxYaw = 0.7f;
	m_yawSpeed = 1.5f;

	m_pitch = 0.f;
	m_minPitch = -0.7f;
	m_maxPitch = 0.7f;
	m_pitchSpeed = 1.5f;

	//m_rotation = glm::quat(0, 0, 0, 0);

	
}
cCamera::~cCamera()
{

}

void cCamera::GetProjectionMatrix(glm::mat4& projOut)
{
	// set up the projection matrix
	projOut = glm::perspective(0.8f, (float)mWidth / (float)mHeight, 0.1f, 100000.f);

	
}
void cCamera::GetViewMatrix(glm::mat4& viewOut)
{
	viewOut = mViewMatrix;
}

void cCamera::GetEyePosition(glm::vec4& eyeOut)
{
	eyeOut.x = mViewMatrix[3].x;
	eyeOut.y = mViewMatrix[3].y;
	eyeOut.z = mViewMatrix[3].z;
	eyeOut.w = 1.f;
}

void cCamera::WindowResize(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

void cCamera::Update(float dt)
{
	bool pressUp = nInput::IsKeyPressed::UP();
	bool pressDown = nInput::IsKeyPressed::DOWN();
	bool pressLeft = nInput::IsKeyPressed::LEFT();
	bool pressRight = nInput::IsKeyPressed::RIGHT();
	bool pressShift = nInput::IsKeyPressed::LEFTSHIFT();
	//bool pressUp = glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS;
	//bool pressDown = glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS;

	if (pressDown != pressUp)	//if pressing both do nothing, if pressing neither do nothing
	{
		if (pressShift)
		{
			float zoomSpeed = mZoomSpeed;
			if (pressUp) zoomSpeed *= -1.f;
			mDistance += zoomSpeed * dt;
			if (mDistance < mMinDistance) mDistance = mMinDistance;
			if (mDistance > mMaxDistance) mDistance = mMaxDistance;
		}
		else
		{
			float pitchSpeed = m_pitchSpeed;
			if (pressUp) pitchSpeed *= -1.f;
			m_pitch += pitchSpeed * dt;
			m_pitch = glm::clamp(m_pitch, m_minPitch, m_maxPitch);
		}
	}
	else
	{
		if (m_pitch != 0.0f)
		{
			if (m_pitch > 0.f)
			{
				m_pitch -= m_pitchSpeed * dt;
				if (m_pitch < 0.f) m_pitch = 0.0f;
			}
			else
			{
				m_pitch += m_pitchSpeed * dt;
				if (m_pitch > 0.f) m_pitch = 0.0f;
			}
		}
	}
	//m_rotation = glm::rotate(m_rotation, m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	/*if (pressRight != pressLeft)
	{
		float panSpeed = mPanSpeed;
		if (pressRight) panSpeed *= -1.f;
		mPanDistance += panSpeed * dt;
		if (mPanDistance < mPanMin) mPanDistance = mPanMin;
		if (mPanDistance > mPanMax) mPanDistance = mPanMax;
	}*/

	if (pressRight != pressLeft)
	{
		float yawSpeed = m_yawSpeed;
		if (pressRight) yawSpeed *= -1.f;
		m_yaw += yawSpeed * dt;
		m_yaw = glm::clamp(m_yaw, m_minYaw, m_maxYaw);
	}
	else
	{
		if (m_yaw != 0.0f)
		{
			if (m_yaw > 0.f)
			{
				m_yaw -= m_yawSpeed * dt;
				if (m_yaw < 0) m_yaw = 0.f;
			}
			else
			{
				m_yaw += m_yawSpeed * dt;
				if (m_yaw > 0.f) m_yaw = 0.f;
			}
		}
	}
	glm::quat qYaw = glm::angleAxis(m_yaw, glm::vec3(0.f, 1.f, 0.f));
	glm::quat qPitch = glm::angleAxis(m_pitch, glm::vec3(mTargetTransform[0]));

	eulerRot = glm::vec3(m_pitch, m_yaw, 0.f);

	//m_rotation = glm::quat(eulerRot);
	m_rotation = qPitch * qYaw;
	UpdateView();
}

void cCamera::UpdateView()
{
	glm::vec3 up(0.f, 1.f, 0.f);
	glm::vec3 right(1.0f, 0.0f, 0.0f);

	glm::vec4 eye4(mTargetTransform[3].x, mTargetTransform[3].y, mTargetTransform[3].z, 1.f);
	// set up the view matrix
	glm::vec3 target(mTargetTransform[3].x, mTargetTransform[3].y, mTargetTransform[3].z);
	glm::vec4 offset(mOffset.x, mOffset.y, mOffset.z, 0.f);
	glm::mat4 rotationMat = glm::mat4_cast(m_rotation);

	offset = mTargetTransform * offset;
	offset = offset * rotationMat;
	offset = glm::normalize(offset) * mDistance;	//in case of scaling renormalize

	
	glm::vec3 eye(target.x + offset.x, target.y + offset.y, target.z + offset.z);
	mViewMatrix = glm::lookAtRH(eye, target, up);
}
void cCamera::SetTargetTransform(glm::mat4& targetTransform)
{
	mTargetTransform = targetTransform;
}