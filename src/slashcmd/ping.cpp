#include "ping.h"

namespace SlashCommand
{
	void Ping::execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event)
	{
		std::string latencyStr = std::to_string(bot.rest_ping * 1000);
		event.reply(":stopwatch: " + latencyStr.substr(0, latencyStr.find('.')) + "ms");
	}

	dpp::slashcommand Ping::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands;

		dpp::slashcommand obj(name, "Shows latency", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_chat_input);

		return obj;
	}
}
