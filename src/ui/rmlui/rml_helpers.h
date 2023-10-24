#pragma once
namespace ui::helpers
{
	inline Rml::Input::KeyIdentifier ConvertKey(int sdlkey)
	{
		// clang-format off
		switch (sdlkey)
		{
		case SDLK_UNKNOWN:      return Rml::Input::KI_UNKNOWN;
		case SDLK_ESCAPE:       return Rml::Input::KI_ESCAPE;
		case SDLK_SPACE:        return Rml::Input::KI_SPACE;
		case SDLK_0:            return Rml::Input::KI_0;
		case SDLK_1:            return Rml::Input::KI_1;
		case SDLK_2:            return Rml::Input::KI_2;
		case SDLK_3:            return Rml::Input::KI_3;
		case SDLK_4:            return Rml::Input::KI_4;
		case SDLK_5:            return Rml::Input::KI_5;
		case SDLK_6:            return Rml::Input::KI_6;
		case SDLK_7:            return Rml::Input::KI_7;
		case SDLK_8:            return Rml::Input::KI_8;
		case SDLK_9:            return Rml::Input::KI_9;
		case SDLK_a:            return Rml::Input::KI_A;
		case SDLK_b:            return Rml::Input::KI_B;
		case SDLK_c:            return Rml::Input::KI_C;
		case SDLK_d:            return Rml::Input::KI_D;
		case SDLK_e:            return Rml::Input::KI_E;
		case SDLK_f:            return Rml::Input::KI_F;
		case SDLK_g:            return Rml::Input::KI_G;
		case SDLK_h:            return Rml::Input::KI_H;
		case SDLK_i:            return Rml::Input::KI_I;
		case SDLK_j:            return Rml::Input::KI_J;
		case SDLK_k:            return Rml::Input::KI_K;
		case SDLK_l:            return Rml::Input::KI_L;
		case SDLK_m:            return Rml::Input::KI_M;
		case SDLK_n:            return Rml::Input::KI_N;
		case SDLK_o:            return Rml::Input::KI_O;
		case SDLK_p:            return Rml::Input::KI_P;
		case SDLK_q:            return Rml::Input::KI_Q;
		case SDLK_r:            return Rml::Input::KI_R;
		case SDLK_s:            return Rml::Input::KI_S;
		case SDLK_t:            return Rml::Input::KI_T;
		case SDLK_u:            return Rml::Input::KI_U;
		case SDLK_v:            return Rml::Input::KI_V;
		case SDLK_w:            return Rml::Input::KI_W;
		case SDLK_x:            return Rml::Input::KI_X;
		case SDLK_y:            return Rml::Input::KI_Y;
		case SDLK_z:            return Rml::Input::KI_Z;
		case SDLK_SEMICOLON:    return Rml::Input::KI_OEM_1;
		case SDLK_PLUS:         return Rml::Input::KI_OEM_PLUS;
		case SDLK_COMMA:        return Rml::Input::KI_OEM_COMMA;
		case SDLK_MINUS:        return Rml::Input::KI_OEM_MINUS;
		case SDLK_PERIOD:       return Rml::Input::KI_OEM_PERIOD;
		case SDLK_SLASH:        return Rml::Input::KI_OEM_2;
		case SDLK_BACKQUOTE:    return Rml::Input::KI_OEM_3;
		case SDLK_LEFTBRACKET:  return Rml::Input::KI_OEM_4;
		case SDLK_BACKSLASH:    return Rml::Input::KI_OEM_5;
		case SDLK_RIGHTBRACKET: return Rml::Input::KI_OEM_6;
		case SDLK_QUOTEDBL:     return Rml::Input::KI_OEM_7;
		case SDLK_KP_0:         return Rml::Input::KI_NUMPAD0;
		case SDLK_KP_1:         return Rml::Input::KI_NUMPAD1;
		case SDLK_KP_2:         return Rml::Input::KI_NUMPAD2;
		case SDLK_KP_3:         return Rml::Input::KI_NUMPAD3;
		case SDLK_KP_4:         return Rml::Input::KI_NUMPAD4;
		case SDLK_KP_5:         return Rml::Input::KI_NUMPAD5;
		case SDLK_KP_6:         return Rml::Input::KI_NUMPAD6;
		case SDLK_KP_7:         return Rml::Input::KI_NUMPAD7;
		case SDLK_KP_8:         return Rml::Input::KI_NUMPAD8;
		case SDLK_KP_9:         return Rml::Input::KI_NUMPAD9;
		case SDLK_KP_ENTER:     return Rml::Input::KI_NUMPADENTER;
		case SDLK_KP_MULTIPLY:  return Rml::Input::KI_MULTIPLY;
		case SDLK_KP_PLUS:      return Rml::Input::KI_ADD;
		case SDLK_KP_MINUS:     return Rml::Input::KI_SUBTRACT;
		case SDLK_KP_PERIOD:    return Rml::Input::KI_DECIMAL;
		case SDLK_KP_DIVIDE:    return Rml::Input::KI_DIVIDE;
		case SDLK_KP_EQUALS:    return Rml::Input::KI_OEM_NEC_EQUAL;
		case SDLK_BACKSPACE:    return Rml::Input::KI_BACK;
		case SDLK_TAB:          return Rml::Input::KI_TAB;
		case SDLK_CLEAR:        return Rml::Input::KI_CLEAR;
		case SDLK_RETURN:       return Rml::Input::KI_RETURN;
		case SDLK_PAUSE:        return Rml::Input::KI_PAUSE;
		case SDLK_CAPSLOCK:     return Rml::Input::KI_CAPITAL;
		case SDLK_PAGEUP:       return Rml::Input::KI_PRIOR;
		case SDLK_PAGEDOWN:     return Rml::Input::KI_NEXT;
		case SDLK_END:          return Rml::Input::KI_END;
		case SDLK_HOME:         return Rml::Input::KI_HOME;
		case SDLK_LEFT:         return Rml::Input::KI_LEFT;
		case SDLK_UP:           return Rml::Input::KI_UP;
		case SDLK_RIGHT:        return Rml::Input::KI_RIGHT;
		case SDLK_DOWN:         return Rml::Input::KI_DOWN;
		case SDLK_INSERT:       return Rml::Input::KI_INSERT;
		case SDLK_DELETE:       return Rml::Input::KI_DELETE;
		case SDLK_HELP:         return Rml::Input::KI_HELP;
		case SDLK_F1:           return Rml::Input::KI_F1;
		case SDLK_F2:           return Rml::Input::KI_F2;
		case SDLK_F3:           return Rml::Input::KI_F3;
		case SDLK_F4:           return Rml::Input::KI_F4;
		case SDLK_F5:           return Rml::Input::KI_F5;
		case SDLK_F6:           return Rml::Input::KI_F6;
		case SDLK_F7:           return Rml::Input::KI_F7;
		case SDLK_F8:           return Rml::Input::KI_F8;
		case SDLK_F9:           return Rml::Input::KI_F9;
		case SDLK_F10:          return Rml::Input::KI_F10;
		case SDLK_F11:          return Rml::Input::KI_F11;
		case SDLK_F12:          return Rml::Input::KI_F12;
		case SDLK_F13:          return Rml::Input::KI_F13;
		case SDLK_F14:          return Rml::Input::KI_F14;
		case SDLK_F15:          return Rml::Input::KI_F15;
		case SDLK_NUMLOCKCLEAR: return Rml::Input::KI_NUMLOCK;
		case SDLK_SCROLLLOCK:   return Rml::Input::KI_SCROLL;
		case SDLK_LSHIFT:       return Rml::Input::KI_LSHIFT;
		case SDLK_RSHIFT:       return Rml::Input::KI_RSHIFT;
		case SDLK_LCTRL:        return Rml::Input::KI_LCONTROL;
		case SDLK_RCTRL:        return Rml::Input::KI_RCONTROL;
		case SDLK_LALT:         return Rml::Input::KI_LMENU;
		case SDLK_RALT:         return Rml::Input::KI_RMENU;
		case SDLK_LGUI:         return Rml::Input::KI_LMETA;
		case SDLK_RGUI:         return Rml::Input::KI_RMETA;
			/*
			case SDLK_LSUPER:       return Rml::Input::KI_LWIN;
			case SDLK_RSUPER:       return Rml::Input::KI_RWIN;
			*/
		default: break;
		}
		// clang-format on

		return Rml::Input::KI_UNKNOWN;
	}

	inline int ConvertMouseButton(int button)
	{
		switch (button)
		{
		case SDL_BUTTON_LEFT: return 0;
		case SDL_BUTTON_RIGHT: return 1;
		case SDL_BUTTON_MIDDLE: return 2;
		default: return 3;
		}
	}

	inline int GetKeyModifierState(SDL_Keymod sdl_mods)
	{
		int retval = 0;

		if (sdl_mods & KMOD_CTRL)
			retval |= Rml::Input::KM_CTRL;

		if (sdl_mods & KMOD_SHIFT)
			retval |= Rml::Input::KM_SHIFT;

		if (sdl_mods & KMOD_ALT)
			retval |= Rml::Input::KM_ALT;

		if (sdl_mods & KMOD_NUM)
			retval |= Rml::Input::KM_NUMLOCK;

		if (sdl_mods & KMOD_CAPS)
			retval |= Rml::Input::KM_CAPSLOCK;

		return retval;
	}
}