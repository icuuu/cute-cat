#pragma once

#include "base.h"

namespace UserCommand
{
	class Avatar : public Base
	{
	public:
		void execute(Context& context, dpp::cluster& bot, const dpp::user_context_menu_t& event) override;
		dpp::slashcommand apiObj(dpp::cluster& bot, std::string name) override;
	};
}
