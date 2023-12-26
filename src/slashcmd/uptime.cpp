#include "uptime.h"

namespace SlashCommand
{
	void Uptime::execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event)
	{
		event.reply(":timer: " + bot.uptime().to_string());
	}

	dpp::slashcommand Uptime::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands;

		dpp::slashcommand obj(name, "Shows uptime", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_chat_input);

		return obj;
	}
}
