#include "program.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

#include <dpp/dpp.h>

// Command headers
#include "slashcmd/base.h"
#include "slashcmd/avatar.h"
#include "slashcmd/chess.h"
#include "slashcmd/echo.h"
// #include "slashcmd/cat.h"
#include "slashcmd/ping.h"
#include "slashcmd/uptime.h"
#include "msgcmd/base.h"
#include "msgcmd/quote.h"
#include "usercmd/base.h"
#include "usercmd/avatar.h"

Program::Program(const char* configPath)
{
	std::ifstream file(configPath);
	if (file.fail()) throw;
	this->context.config = nlohmann::json::parse(file);
	file.close();
}

void Program::run()
{
	std::unordered_map<std::string, SlashCommand::Base*> slashCommands = 
	{
		{ "avatar", new SlashCommand::Avatar },
		{ "chess", new SlashCommand::Chess },
		{ "echo", new SlashCommand::Echo },
		{ "ping", new SlashCommand::Ping },
		{ "uptime", new SlashCommand::Uptime }
		// { "cat", new SlashCommand::Cat }
	};

	std::unordered_map<std::string, MessageCommand::Base*> messageCommands =
	{
		{ "quote", new MessageCommand::Quote }
	};

	std::unordered_map<std::string, UserCommand::Base*> userCommands =
	{
		{ "avatar", new UserCommand::Avatar }
	};

	dpp::cluster bot(this->context.config.at("token"), dpp::i_all_intents);

	// bot.on_log(dpp::utility::cout_logger());

	bot.on_message_create([this, &bot](const dpp::message_create_t& event)
	{
		if (event.msg.content.contains("edward"))
			bot.message_add_reaction(event.msg, "edward:1042165132555460639");

		/*	
		if (event.msg.content.substr(0, 7) == "kitty, ")
		{
			nlohmann::json postData;
			postData["model"] = "gpt-3.5-turbo";
    			postData["prompt"] = "What is the current time?";
    			postData["temperature"] = 0;
			postData["max_tokens"] = 100;
			postData["top_p"] = 1;
			postData["frequency_penalty"] = 0.0;
			postData["presence_penalty"] = 0.0;
			postData["stop"] = {"\n"};
			bot.request("https://api.openai.com/v1/chat/completions", dpp::m_post,
				[](const dpp::http_request_completion_t& cc)
			{
				std::cout << cc.body << "\n";
			},
			postData.dump(),
			"application/json",
			{
				{"Content-Type", "application/json"},
				{"Authorization", "Bearer " + this->context.config.at("openai-key").get<std::string>()}
			});
		}
		*/
	});

	bot.on_slashcommand([this, &bot, &slashCommands](const dpp::slashcommand_t& event)
	{
		slashCommands[event.command.get_command_name()]->execute(this->context, bot, event);
	});

	bot.on_message_context_menu([this, &bot, &messageCommands](const dpp::message_context_menu_t& event)
	{
		messageCommands[event.command.get_command_name()]->execute(this->context, bot, event);	
	});

	bot.on_user_context_menu([this, &bot, &userCommands](const dpp::user_context_menu_t& event)
	{
		userCommands[event.command.get_command_name()]->execute(this->context, bot, event);
	});

	bot.on_ready([this, &bot, &slashCommands, &messageCommands, &userCommands](const dpp::ready_t& event)
	{
		printf("press 'y' to delete all commands, press anything else to disregard\n");
		if (std::cin.get() == 'y') bot.global_bulk_command_delete_sync();
		
		printf("Server, ID\n");
		for (const auto& [snowflake, guild] : bot.current_user_get_guilds_sync())
		{
			printf("%s, %s\n", guild.name.c_str(), snowflake.str().c_str());
		}

		bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, this->context.config.at("status")));

		if (dpp::run_once<struct register_bot_commands>())
        	{
			for (auto& [name, cmd] : slashCommands)
                		bot.global_command_create(cmd->apiObj(bot, name));

			for (auto& [name, cmd] : messageCommands)
				bot.global_command_create(cmd->apiObj(bot, name));
        	
			for (auto& [name, cmd] : userCommands)
				bot.global_command_create(cmd->apiObj(bot, name));
		}
	});

	bot.start(dpp::st_wait);
}
