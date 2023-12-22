#include "echo.h"

namespace SlashCommand
{
	void Echo::execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event)
	{
		std::string m = event.command.get_command_interaction().get_value<std::string>(0);
		event.reply(m);
	}

	dpp::slashcommand Echo::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands;

		dpp::slashcommand obj(name, "Echo a message", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_chat_input);
		obj.add_option(dpp::command_option(dpp::co_string, "message", "What to echo", true));

		return obj;
	}
}
