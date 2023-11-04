#include "CEventInstancer.h"
#include "window/CWindow.h"

Rml::EventPtr ui::CEventInstancer::InstanceEvent(Rml::Element* target, Rml::EventId id, const Rml::String& name, const Rml::Dictionary& parameters, bool interruptible)
{
	const auto ev = new Rml::Event(target, id, name, parameters, interruptible);
	if (!m_window->SendEvent(target, name, parameters) && interruptible)
		ev->StopImmediatePropagation();
	return Rml::EventPtr(ev);
}

void ui::CEventInstancer::ReleaseEvent(Rml::Event* event)
{
	delete event;
}
