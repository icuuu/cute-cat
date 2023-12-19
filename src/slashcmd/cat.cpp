#include "cat.h"

namespace SlashCommand
{
	void Cat::execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event)
	{
		event.thinking();
		bot.request(
			"https://cataas.com/cat/cute?json=true", dpp::m_get,
			[&event](const dpp::http_request_completion_t& cc)
		{
			nlohmann::json data = nlohmann::json::parse(cc.body);

			dpp::embed embed = dpp::embed();
			embed.set_image("https://cataas.com/cat/" + data.at("_id").get<std::string>());
			embed.set_footer(data.at("_id").get<std::string>(), "");
			event.reply(dpp::message(event.command.channel_id, embed));
		});	
	}

	dpp::slashcommand Cat::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands | dpp::p_attach_files;

		dpp::slashcommand obj(name, "Sends a random cat picture", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_chat_input);

		return obj;
	}
}
