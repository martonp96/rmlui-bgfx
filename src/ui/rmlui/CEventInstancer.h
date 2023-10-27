#pragma once
#include "pch.h"

namespace window
{
	class CWindow;
}

namespace ui
{
	class CEventInstancer : public Rml::EventInstancer
	{
	private:
		window::CWindow* m_window;

	public:
		CEventInstancer(window::CWindow* window) : m_window(window) {}

		Rml::EventPtr InstanceEvent(Rml::Element* target, Rml::EventId id, const Rml::String& name, const Rml::Dictionary& parameters, bool interruptible) override;
		void ReleaseEvent(Rml::Event* event) override;

		void Release() override {}
	};
}