#include "chess.h"

namespace SlashCommand
{
	void Chess::execute(Context& context, dpp::cluster& bot, const dpp::slashcommand_t& event)
	{
		std::string url = "https://lichess.org/api/challenge/open?variant=standard";
	
		bot.request(url, dpp::m_post, [event](const dpp::http_request_completion_t& cc) {
			try {
				nlohmann::json request = nlohmann::json::parse(cc.body);
				event.reply("[Game Link](" + request.at("challenge").at("url").get<std::string>() + ")");
			}
			catch (const std::exception& e)
			{
				event.reply(e.what());
			}
		});
	}
	
	dpp::slashcommand Chess::apiObj(dpp::cluster& bot, std::string name)
	{
		uint64_t perms = dpp::p_use_application_commands;
	
		dpp::slashcommand obj(name, "Create a Lichess game", bot.me.id);
		obj.set_default_permissions(perms);
		obj.set_type(dpp::ctxm_chat_input);

		return obj;
	}
}
