#pragma once
#include "pch.h"

namespace ui
{
	class CEventInstancer : public Rml::EventInstancer
	{
	private:

	public:

		Rml::EventPtr InstanceEvent(Rml::Element* target, Rml::EventId id, const Rml::String& name, const Rml::Dictionary& parameters, bool interruptible) override;
		void ReleaseEvent(Rml::Event* event) override;

		void Release() override {}
	};
}