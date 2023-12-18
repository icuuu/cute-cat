#include "program.h"

#include <fstream>
#include <unordered_map>

#include <dpp/dpp.h>
#include <ncurses.h>

// Command headers
#include "slashcmd/base.h"
#include "slashcmd/ping.h"
#include "msgcmd/base.h"
#include "msgcmd/quote.h"

Program::Program(const char* configPath)
{
	std::ifstream file(configPath);
	if (file.fail()) throw;
	this->context.config = nlohmann::json::parse(file);
	file.close();
}

void Program::run()
{
	/* Ncurses shiz
	initscr();
	char c;
	while (c != 'q')
	{
		refresh();
		c = getch();
	}
	endwin();
	*/

	std::unordered_map<std::string, SlashCommand::Base*> slashCommands = 
	{
		{ "ping", new SlashCommand::Ping }
	};

	std::unordered_map<std::string, MessageCommand::Base*> messageCommands =
	{
		{ "quote", new MessageCommand::Quote }
	};

	dpp::cluster bot(this->context.config.at("token"), dpp::i_all_intents);

	// bot.on_log(dpp::utility::cout_logger());

	bot.on_message_create([this, &bot](const dpp::message_create_t& event)
	{
		if (event.msg.content.contains("edward"))
			bot.message_add_reaction(event.msg, "edward:1042165132555460639");
	});

	bot.on_slashcommand([this, &bot, &slashCommands](const dpp::slashcommand_t& event)
	{
		slashCommands[event.command.get_command_name()]->execute(this->context, bot, event);
	});

	bot.on_message_context_menu([this, &bot, &messageCommands](const dpp::message_context_menu_t& event)
	{
		messageCommands[event.command.get_command_name()]->execute(this->context, bot, event);	
	});

	bot.on_ready([this, &bot, &slashCommands, &messageCommands](const dpp::ready_t& event)
	{
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
        	}
	});

	bot.start(dpp::st_wait);
}
