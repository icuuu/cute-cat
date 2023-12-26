#include "uptime.h"

namespace SlashCommand
{
	void Uptime::execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event)
	{
		dpp::utility::uptime u = bot.uptime();
		std::string description = "```c\n" +
			std::to_string(u.days) + " days " +
			std::to_string(u.hours) + " hours " +
			std::to_string(u.mins) + " minutes " +
			std::to_string(u.secs) + " seconds```";
		
		dpp::embed embed;
		embed.set_title("Uptime");
		embed.set_description(description);
		event.reply(dpp::message(event.command.channel_id, embed));
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
