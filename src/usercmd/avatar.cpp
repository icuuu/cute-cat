#include "avatar.h"

namespace UserCommand
{
	void Avatar::execute(Context& context, dpp::cluster& bot, const dpp::user_context_menu_t& event)
	{
		dpp::embed embed;
		embed.set_title(event.get_user().username);
		embed.set_image(event.get_user().get_avatar_url() + "?size=1024");
		event.reply(dpp::message(event.command.channel_id, embed));
	}

	dpp::slashcommand Avatar::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands;

		dpp::slashcommand obj(name, "Gets the avatar", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_user);

		return obj;
	}
}
