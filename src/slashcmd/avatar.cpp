#include "avatar.h"

namespace SlashCommand
{
	void Avatar::execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event)
	{
		dpp::snowflake userId = event.command.get_command_interaction().get_value<dpp::snowflake>(0);
		dpp::user user = event.command.get_resolved_user(userId);

		dpp::embed embed;
		embed.set_title(user.username);
		embed.set_image(user.get_avatar_url() + "?size=1024");
		event.reply(dpp::message(event.command.channel_id, embed));
	}

	dpp::slashcommand Avatar::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands;

		dpp::slashcommand obj(name, "Gets the avatar", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_chat_input);
		obj.add_option(dpp::command_option(dpp::co_user, "user", "User to get avatar of", true));

		return obj;
	}
}
