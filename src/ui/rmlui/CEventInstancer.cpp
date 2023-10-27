#include "CEventInstancer.h"

Rml::EventPtr ui::CEventInstancer::InstanceEvent(Rml::Element* target, Rml::EventId id, const Rml::String& name, const Rml::Dictionary& parameters, bool interruptible)
{
	const auto ev = new Rml::Event(target, id, name, parameters, interruptible);
	
	return Rml::EventPtr(ev);
}

void ui::CEventInstancer::ReleaseEvent(Rml::Event* event)
{
	delete event;
}
