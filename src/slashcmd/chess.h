#pragma once

#include "base.h"

namespace SlashCommand
{
	class Chess : public Base
	{
	public:	
		void execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event) override;
		dpp::slashcommand apiObj(dpp::cluster& bot, std::string name) override;
	};
}
