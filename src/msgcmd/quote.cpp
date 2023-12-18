#include "quote.h"

namespace MessageCommand
{
	void Quote::execute(Context& context, dpp::cluster& bot, const dpp::message_context_menu_t& event)
	{
		event.reply("> " + event.get_message().content + "\n—" + event.get_message().author.global_name);
	}

	dpp::slashcommand Quote::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands;

		dpp::slashcommand obj(name, "Quotes a message", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_message);

		return obj;
	}
}
