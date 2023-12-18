#pragma once

#include "base.h"

namespace MessageCommand
{
	class Quote : public Base
	{
	public:
		void execute(Context& context, dpp::cluster& bot, const dpp::message_context_menu_t& event) override;
		dpp::slashcommand apiObj(dpp::cluster& bot, std::string name) override;
	};
}
