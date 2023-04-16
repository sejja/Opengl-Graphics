//
//	Event.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/03/23
//	Copyright � 2023. All Rights reserved
//

#include "Event.h"

namespace Core {
	namespace Events {
		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION
		*/ // ---------------------------------------------------------------------
		Event::~Event() {}

		// ------------------------------------------------------------------------
		/*! Handle
		*
		*   Handles an event, which subsecuntly will call the required object
		*/ // ---------------------------------------------------------------------
		void EventHandler::Handle(const Event& event) {
			auto it = mHandlerCollection.find(typeid(event).name());

			if (it != mHandlerCollection.end())
				it->second->Handle(event);
		}

		// ------------------------------------------------------------------------
		/*! Handle
		*
		*	Calls the event' subscribed function
		*/ // ---------------------------------------------------------------------
		void HandlerFunction::Handle(const Event& concrete_event) {
			Call(concrete_event);
		}
	}
}