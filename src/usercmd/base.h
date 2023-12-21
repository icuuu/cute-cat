#pragma once

#include <string>

#include <dpp/dpp.h>

#include "../context.h"

namespace UserCommand
{
	class Base
	{
	public:
		Base() {};
		
		virtual void execute(Context& context, dpp::cluster& bot, const dpp::user_context_menu_t& event) = 0;
		virtual dpp::slashcommand apiObj(dpp::cluster& bot, std::string name) = 0;
	};
}
